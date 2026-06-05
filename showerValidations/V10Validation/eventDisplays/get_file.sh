#!/bin/bash

echo "Processing input: $1"

# 1. Removed spaces around the '=' assignment
file=$(samweb list-definition-files msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_stage1AndCaf | grep "$1-")

if [ -z "$file" ]; then
    echo "No file found for run $1"
    exit 1
fi

# 2. Wrapped $file in quotes to handle potential multi-line edge cases safely
tmp=$(samweb locate-file "$file")
tmp=${tmp#dcache:}

# 3. Cleaned up potential trailing parentheses from the 'locate-file' output
tmp=$(echo "$tmp" | cut -d'(' -f1)

echo "${tmp}/${file}"

lar -c $2 -s "${tmp}/${file}" --no-output -n1 -e $3