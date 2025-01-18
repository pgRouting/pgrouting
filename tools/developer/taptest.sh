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
DIR=$(git rev-parse --show-toplevel)
pushd "${DIR}" > /dev/null || exit 1

DIR="$1"
shift
PGFLAGS=$*
VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)

echo "dir ${DIR}"
echo "pgflags ${PGFLAGS}"
QUIET="-v"
QUIET="-q"

PGPORT="5432"
PGUSER="youruser"

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
    sleep 3

    dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
    createdb "${PGFLAGS}" "${PGDATABASE}"

    bash setup_db.sh "${PGPORT}" "${PGDATABASE}" "${PGUSER}" "${v}"
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
        psql "${PGFLAGS}" -d "$PGDATABASE" -c "SET client_min_messages TO DEBUG3; ALTER EXTENSION pgrouting UPDATE TO '${VERSION}';"
    fi

    # run this version's test
    psql "${PGFLAGS}" -d "$PGDATABASE" -c "SELECT * FROM pgr_full_version();"


    pg_prove "$QUIET" --normalize --directives --recurse "${PGFLAGS}"  -d "${PGDATABASE}" "pgtap/${DIR}"
    #dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
done
