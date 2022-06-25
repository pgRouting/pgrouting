#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Remove all the obsolete entries, i.e. lines starting with #~ from .po files
# ------------------------------------------------------------------------------

# For all the chapter files
find locale -type f -name '*.po' -exec sh -c '
    if grep -q "#~" "$1"; then
        perl -pi -0777 -e "s/#~.*//s" "$1"
        git add "$1"
    fi
  ' sh {} \;
