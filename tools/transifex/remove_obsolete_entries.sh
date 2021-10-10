#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Remove all the obsolete entries, i.e. lines starting with #~ from .po files
# ------------------------------------------------------------------------------

# For all the chapter files
for file in $(find locale/en -type f -name "*.po"); do
    if grep -q '#~' $file; then
        perl -pi -0777 -e 's/#~.*//s' $file
        git add $file
    fi
done

