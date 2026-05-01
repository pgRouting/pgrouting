#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2026-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

# run from top of repository
DIR=$(git rev-parse --show-toplevel)
pushd "${DIR}" > /dev/null || exit 1

BASE="upstream/develop"
if [ -n "$1" ]; then
    BASE="$1"
fi

ALLFILES=$(git diff-tree --no-commit-id --diff-filter=d --name-only -r "${BASE}" HEAD)
CURRENT_YEAR=$(date +%Y)

function update {
    if [ -z "$4" ]; then return; fi
    PREFIX="$1"
    SUFFIX="$2"
    LICENSE="$3"
    FILE="$4"
    CONTROL=$5
    mapfile -t FILES <<< "$FILE"
    for file in "${FILES[@]}" ; do
        name=$(basename "${file}")
        year=$(git log --follow --format=%as --date default "${file}" | tail -1 | awk -F- '{print $1}')
        license=${LICENSE/years/${year}-${CURRENT_YEAR}}
        prefix="$PREFIX"
        if [ "$CONTROL" = "4" ] ; then
            prefix=$(head -1 "${file}")"\n${PREFIX}"
        fi
        perl -i -ne "print if $. > ${CONTROL}" "${file}"
        echo -e "${prefix}${license}${SUFFIX}" | cat - "${file}" > tmp.tmp && cp tmp.tmp "${file}"
    done
}

function update_gnu {
    if [ -z "$1" ]; then return; fi
    STARTMARK="PGR-GNU$(printf '*%.0s' {1..65})\n"
    ENDMARK=" $(printf '*%.0s' {1..68})PGR-GNU"
    FILE=$1
    mapfile -t FILES <<< "$FILE"
    for file in "${FILES[@]}" ; do
        name=$(basename "${file}")
        year=$(git log --follow --format=%as --date default "${file}" | tail -1 | awk -F- '{print $1}')
        perl -pi -e "BEGIN{undef $/;} s|PGR-GNU.*\nFile.*\n|${STARTMARK}File: ${name}\n|" "${file}"
        perl -pi -e "BEGIN{undef $/;} s|This program(.*)PGR-GNU|${GPL2}\n\n${ENDMARK}|smg" "${file}"
        perl -pi -e "s/Copyright(.*)pgRouting developers/Copyright (c) ${year}-${CURRENT_YEAR} pgRouting developers/" "${file}"
    done
}

# --------------------------------------
# Creative commons
# - doc: .rst
# - docqueries: .pg .result
# - pgtap: .pg
# --------------------------------------

CCBYASA=":file: This file is part of the pgRouting project.
:copyright: Copyright (c) years pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0"

GPL3LINES="# This file is part of the pgRouting project.
# Copyright (c) years pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE"

GPL2="This program is free software; you can redistribute it and\/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA."


# CCBYASA
FILE=$(grep '\.rst' <<< "${ALLFILES}")
update "" "" "${CCBYASA}" "${FILE}" 3

FILE=$(grep -E '\.pg$' <<< "${ALLFILES}")
update "/* " " */" "${CCBYASA}" "${FILE}" 3


# GPL3LINES
FIND=("CMakeLists" "cmake\/pgr")
for ext in "${FIND[@]}" ; do
    FILE=$(grep "$ext" <<< "${ALLFILES}")
    update "" "" "${GPL3LINES}" "${FILE}" 3
done

FIND=("\.sh" "test\.conf" "\.yml")
for ext in "${FIND[@]}" ; do
    FILE=$(grep "$ext" <<< "${ALLFILES}")
    update "" "" "${GPL3LINES}" "${FILE}" 4
done

# GPL-2
FIND=("\.pl$" "\.cpp$" "\.c$" "\.h$" "\.hpp" "\.sql")
for ext in "${FIND[@]}" ; do
    FILE=$(grep "$ext" <<< "${ALLFILES}")
    update_gnu "${FILE}"
done

# ignore symlinks
git restore docqueries/pickDeliver/lc101.pg
git restore docqueries/src/sampledata.pg
git restore docqueries/pickDeliver/oneDepotData.pg
