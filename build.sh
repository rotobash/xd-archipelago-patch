#!/bin/bash

python -m src assets/Start.dol out GXXE
python create_patch_file.py assets/Start.dol out/Start.dol out