#!/bin/bash
# ------------------------------------------------------------------------------
# /*PGR-GNU*****************************************************************
# File: update_locale.sh
# Copyright (c) 2021 pgRouting developers
# Mail: project@pgrouting.org
# ------
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ********************************************************************PGR-GNU*/
# ------------------------------------------------------------------------------


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
