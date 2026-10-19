import importlib
from pathlib import Path

def cleanup(out_dir: Path):
    for file in out_dir.glob("*.o"):
        file.unlink()
    for file in out_dir.glob("*.map"):
        file.unlink()

    patchFile = out_dir.joinpath(f"archipelago.bin")
    if patchFile.exists():
        patchFile.unlink()


def gamecode_to_flags(gamecode: str):
    flags = []
    region = gamecode[3]

    if region == "E":
        flags.append("-DREGION_US")
    elif region == "P":
        flags.append("-DREGION_EU")
    elif region == "J":
        flags.append("-DREGION_JP")

    return flags


def build_dolckit_patches(gamecode: str, in_dol: Path, out_dir: Path):
    from dol_c_kit import Project
    flags = gamecode_to_flags(gamecode)

    game = gamecode[1:3]
    if game == "XX":
        SDA_BASE = 0x804f3dc0
        SDA_BASE2 = 0x804efe20

        DOL_FREE_SPACE_OFFSET = 0x803EB8C0
        game = "xd"
    elif game == "CE":
        game = "colo"
        return

    freeSpaceOffset = DOL_FREE_SPACE_OFFSET
    proj = Project(freeSpaceOffset, True)
    proj.src_dir = f"src/{game}/"
    proj.obj_dir = str(out_dir) + "/"
    proj.set_sda_bases(SDA_BASE, SDA_BASE2)
    proj.add_linker_script_file(f"src/{game}/linkerscript.ld")

    for file in Path(f"src/{game}").glob(f"*.c"):
        proj.add_c_file(file.name, flags)

    patch_module = importlib.import_module(f".{game}", "src")
    patch_module.hook(proj)

    out_dir_file = proj.obj_dir + Path(in_dol).name
    proj.build_dol(str(in_dol), out_dir_file)
    patchFile = out_dir.joinpath(f"project.bin")
    if patchFile.exists():
        cleanup(out_dir)

        patchFile = patchFile.rename(f"{out_dir}/archipelago.bin")

    with open(out_dir_file, "rb") as f:
        dolBytes = f.read()
    # force patch to be at the free space offset
    with patchFile.open("rb") as patch:
        patchBytes = patch.read()
        with open(out_dir_file, "wb") as f:
            offset = (freeSpaceOffset - 0x80000100)
            f.write(dolBytes[:offset])
            f.flush()
            f.write(patchBytes)
            f.flush()
            f.write(dolBytes[(offset + len(patchBytes)):])
            f.flush()
