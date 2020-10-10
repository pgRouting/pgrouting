#!/bin/bash
#/*PGR-GNU*****************************************************************

# FILE: update_tester.sh

# Copyright (c) 2016 pgRouting developers
# Mail: project@pgrouting.org
#
# ------
#
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

TWEAK=""
PGPORT=5432
echo "sorry this only works on vicky's computer"
PGUSER="vicky"
DB="___pgr___test___"

function info {

    # ----------------------
    #
    echo "**pgRouting version must be installed before calling**"
    echo "$1"

    echo "EXAMPLE USAGE"
    echo "- Short execution"
    echo  "bash tools/testers/update-tester.sh $1 $2"
    echo "- For running pgtap tests:"
    echo  "bash tools/testers/update-tester.sh $1 $2 long"

}


if [[ -z  "$1" ]]; then
    echo missing version example:
    info 2.6.3 12
    exit 1
fi

FROM_PGR="$1"
CURRENT=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
LONG=$2

if [[ "$FROM_PGR" == 2.* ]];
then
    exit 0
fi


dropdb --if-exists "$DB"

cd build
cmake -DPGROUTING_DEBUG=ON -DCMAKE_BUILD_TYPE=Debug ..
make -j 4
sudo make install
cd ..



function update_test {

echo
echo
echo "Updating from $1 to $2"
echo ------------------------------------


createdb  "$DB"
psql  "$DB"  <<EOF
CREATE extension postgis;
CREATE extension pgrouting with version '$1';
EOF

OLD_VERSION=$(psql "$DB" -t -c 'SELECT * FROM pgr_version()')
echo "$OLD_VERSION"


psql "$DB" -e -c "ALTER extension pgrouting update to '$2'"


NEW_VERSION=$(psql "$DB" -t -c 'SELECT * FROM pgr_version()')

echo "$OLD_VERSION ->> $NEW_VERSION"

if [ "b$NEW_VERSION" != "b $2$TWEAK" ]
then
    echo "FAIL: Could not update from version $1 to version $2"
    dropdb "${DB}"
    exit 1
fi


DIR="sql/sigs"
FILE="$DIR/pgrouting--$2.sig"

echo "#VERSION pgrouting $2" > "$FILE"
{
    echo "#TYPES"
    psql "${DB}" -c '\dx+ pgrouting' -A | grep '^type' | cut -d ' ' -f2-
    echo "#FUNCTIONS"
    psql ${DB} -c '\dx+ pgrouting' -A | grep '^function' | cut -d ' ' -f2-
} >> "${FILE}"


DIFF=$(git diff "$FILE")
if [ -n "$DIFF" ]
then
    echo "$DIFF"
else
    echo "$2 sigunatures are OK"
fi


if [ -n "$LONG" ]
then
    sh ./tools/testers/pg_prove_tests.sh $PGUSER $PGPORT Release
fi

dropdb "$DB"

} # end of function


update_test "${FROM_PGR}" "${CURRENT}"
