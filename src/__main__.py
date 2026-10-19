

from pathlib import Path
from sys import argv
from src import build_dolckit_patches


def read_dol_from_iso(in_file: Path):
    dol_file = in_file.with_name("Start.dol")


if __name__ == "__main__" and len(argv) > 3:
    in_file = argv[1]
    out_dir = argv[2]
    game_code = argv[3]
    out_dir_p = Path(out_dir)
    dol_file = Path(in_file)
    out_dir_p.mkdir(parents=True, exist_ok=True)

    build_dolckit_patches(game_code, dol_file.resolve(), out_dir_p)
