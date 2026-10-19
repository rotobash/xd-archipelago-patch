# xd-archipelago-patch
Patches XD functions with custom implementations to provide in-depth AP checks

## Pre-Requisites

To build this project you will need:
- [Python 3](https://www.python.org/downloads/)
- [DevKitPPC](https://devkitpro.org/wiki/Getting_Started)

## Getting Started

(Optional) Create a virtual environment:
`python3 -m venv .venv`

Install dependencies:
`pip install -r requirements.txt`

Add your unmodified DOL binary to the assets folder:
`cp /path/to/Start.dol assets`

Build:
`./build.sh`
OR
`python -m src assets/Start.dol out GXXE`
`python create_patch_file.py assets/Start.dol out/Start.dol out`

The resulting `patch.diff` file contains just the sections of the DOL that were changed.