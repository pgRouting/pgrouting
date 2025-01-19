#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: tapest.sh
# Copyright (c) 2018 pgRouting developers
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

if [[ -z  $1 ]]; then
    echo "Usage:"
    echo "taptest.sh <pgtap directory> [<postgresql options>]";
    echo "Examples:"
    echo "taptest.sh withPoints/undirected_equalityDD.sql -p 5432"
    exit 1;
fi

# run from root of repository
if ! DIR=$(git rev-parse --show-toplevel 2>/dev/null); then
    echo "Error: Must be run from within the git repository" >&2
    exit 1
fi
pushd "${DIR}" > /dev/null || exit 1

DIR="$1"
shift
PGFLAGS=$*
if ! VERSION=$(grep -E '^[[:space:]]*project\(PGROUTING[[:space:]]+VERSION[[:space:]]+([^[:space:];]+)' CMakeLists.txt | sed -E 's/.*VERSION[[:space:]]+([^[:space:];]+).*/\1/'); then
    echo "Error: Failed to extract version from CMakeLists.txt" >&2
    exit 1
fi

if [[ -z "${VERSION}" ]]; then
    echo "Error: Version not found in CMakeLists.txt" >&2
    exit 1
fi

echo "dir ${DIR}"
echo "pgflags ${PGFLAGS}"
echo "VERSION ${VERSION}"
QUIET="-v"
QUIET="-q"

PGPORT="${PGPORT:-5432}"
PGUSER="${PGUSER:-$USER}"

PGDATABASE="___pgr___test___"
PGRVERSION="3.6.1 3.6.0 3.5.1 3.5.0 3.2.0 3.1.3 3.0.6"
PGRVERSION="3.7.2"


for v in ${PGRVERSION}
do
    pushd tools/testers/
    echo "--------------------------"
    echo " Running with version ${v}"
    echo " test ${DIR}"
    echo "--------------------------"
    # give time to developer to read message
    sleep 3

    dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
    createdb "${PGFLAGS}" "${PGDATABASE}"

    if ! bash setup_db.sh "${PGPORT}" "${PGDATABASE}" "${PGUSER}" "${v}"; then
        echo "Error: Database setup failed" >&2
        exit 1
    fi
    echo "--------------------------"
    echo " Installed version"
    echo "--------------------------"
    psql "${PGFLAGS}" -d "$PGDATABASE" -c "SELECT * FROM pgr_full_version();"
    popd

    if [[ "${v}" != "${VERSION}" ]]
    then
        # run tests on old version
        pg_prove "$QUIET" --normalize --directives --recurse "${PGFLAGS}"  -d "${PGDATABASE}" "pgtap/${DIR}"

        # update to this version
        echo "--------------------------"
        echo " update version"
        echo "--------------------------"
        if ! psql "${PGFLAGS}" -d "$PGDATABASE" -c "SET client_min_messages TO DEBUG3; ALTER EXTENSION pgrouting UPDATE TO '${VERSION}';"; then
            echo "Error: Failed to update pgrouting extension to version ${VERSION}" >&2
            exit 1
        fi

        psql "${PGFLAGS}" -q -t -d "$PGDATABASE" -c "SELECT extversion FROM pg_extension WHERE extname = 'pgrouting';"

        # Verify update success
        if ! psql "${PGFLAGS}" -q -t -d "$PGDATABASE" -c "SELECT extversion FROM pg_extension WHERE extname = 'pgrouting';" | grep -q "${VERSION}"; then
            echo "Error: Extension version mismatch after update" >&2
            exit 1
        fi
    fi

    # run this version's test
    psql "${PGFLAGS}" -d "$PGDATABASE" -c "SELECT * FROM pgr_full_version();"


    pg_prove "$QUIET" --normalize --directives --recurse "${PGFLAGS}"  -d "${PGDATABASE}" "pgtap/${DIR}"
    #dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
done
