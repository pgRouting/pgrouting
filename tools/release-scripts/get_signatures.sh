#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: get_signatures.sh
# Copyright (c) 2017 pgRouting developers
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
set -e

if [  "$1" = "--help" ] ; then
    echo "Usage: getSignatures.sh [DB_ARGS]"
    echo "  DB_ARGS optional"
    echo "example"
    echo "get_signatures.sql -U postgres -h localhost -p 5432 "
    echo "Exeute from the root of the repository"
    exit 0
fi

VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
MINOR=${VERSION%.*}

DB_NAME="____sigs_routing____"
DIR="sql/sigs"

# DB_ARGS are the remaining of the arguments
read -ra DB_ARGS <<< "$*"

FILE="$DIR/pgrouting--$MINOR.sig"

dropdb --if-exists "${DB_ARGS[@]}" "$DB_NAME"
createdb "${DB_ARGS[@]}" "$DB_NAME"

psql  "${DB_ARGS[@]}"  "$DB_NAME" <<EOF
SET client_min_messages = WARNING;
CREATE EXTENSION pgrouting WITH VERSION '$VERSION' CASCADE;
EOF

psql "${DB_ARGS[@]}" "$DB_NAME" -c '\dx+ pgrouting' -A | grep '^function' | cut -d ' ' -f2- | sort -d > "$FILE"
