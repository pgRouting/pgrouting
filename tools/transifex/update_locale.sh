#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Update the locale files
# ------------------------------------------------------------------------------



DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" > /dev/null || exit 1

pushd build > /dev/null || exit 1
cmake -DWITH_DOC=ON -DCMAKE_BUILD_TYPE=Release -DLOCALE=ON ..

make locale
popd > /dev/null || exit 1

# List all the files that needs to be committed in build/doc/locale_changes.txt
awk '/^Update|^Create/{print $2}' build/doc/locale_changes.txt > build/doc/locale_changes_po.txt # .po files
cp build/doc/locale_changes_po.txt build/doc/locale_changes_po_pot.txt
perl -pe 's/(.*)en\/LC_MESSAGES(.*)/$1pot$2t/' < build/doc/locale_changes_po.txt >> build/doc/locale_changes_po_pot.txt  # .pot files

# Remove obsolete entries #~ from .po files
bash tools/transifex/remove_obsolete_entries.sh

popd > /dev/null || exit 1
