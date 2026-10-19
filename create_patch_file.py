
import json
from pathlib import Path
from sys import argv
import bsdiff4

if __name__ == "__main__":
    orig_dol_file = argv[1]
    dol_file = argv[2]
    out_dir = argv[3]

    bsdiff4.file_diff(orig_dol_file, dol_file, out_dir + "/patch.diff")


