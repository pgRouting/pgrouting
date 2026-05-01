#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2021-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" > /dev/null || exit 1

mkdir -p build
pushd build > /dev/null || exit 1
cmake -DBUILD_HTML=OFF -DBUILD_LOCALE=ON ..

make locale
popd > /dev/null || exit 1

# List all the files that needs to be committed in build/doc/locale_changes.txt
awk '/^Update|^Create/{print $2}' build/doc/locale_changes.txt > build/doc/locale_changes_po.txt # .po files
cp build/doc/locale_changes_po.txt build/doc/locale_changes_po_pot.txt
perl -ne '/\/en\// && print' build/doc/locale_changes_po.txt | \
    perl -pe 's/(.*)en\/LC_MESSAGES(.*)/$1pot$2t/' >> build/doc/locale_changes_po_pot.txt  # .pot files

# Remove obsolete entries #~ from .po files
find locale -type f -name '*.po' -exec sh -c '
    msgattrib --no-obsolete -o "$1" "$1"
  ' sh {} \;

while read -r f; do git add "$f"; done < build/doc/locale_changes_po_pot.txt

git restore --staged locale/*/LC_MESSAGES/index.po
git restore locale/*/LC_MESSAGES/index.po

popd > /dev/null || exit 1
