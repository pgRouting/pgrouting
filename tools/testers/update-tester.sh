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

CURRENT=3.0.0
TWEAK="-rc1"
PGSQL_VER="9.5"
PGPORT=5432
PGUSER="vicky"
DB="___pgr___test___"

function info {

    # ----------------------
    #
    echo **pgRouting version must be installed before calling**
    echo "$1"
    echo "$2"

    echo # EXAMPLE USAGE

    echo  bash tools/testers/update-tester.sh $2

}


if [[ -z  "$1" ]]; then
    echo missing version to upgrade
    info $CURRENT 2.6.3
    exit 1
fi

FROM_PGR="$1"

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

INSTALLED=$(locate "/usr/share/postgresql/$PGSQL_VER/extension/pgrouting--$1.sql")

if [ "$INSTALLED" == "/usr/share/postgresql/$PGSQL_VER/extension/pgrouting--$1.sql" ]
then
    echo "/usr/share/postgresql/$PGSQL_VER/extension/pgrouting--$1.sql found"
else
    echo "FATAL: /usr/share/postgresql/$PGSQL_VER/extension/pgrouting--$1.sql Not found"
    exit 1
fi


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
    dropdb ___test_update
    exit 1
fi


sh ./tools/testers/pg_prove_tests.sh $PGUSER $PGPORT Release
dropdb "$DB"

} # end of function


update_test $FROM_PGR $CURRENT
exit
#------------------------------------
### updates from 2.5
#------------------------------------

update_test 2.5.3 $CURRENT
update_test 2.5.2 $CURRENT
update_test 2.5.1 $CURRENT
update_test 2.5.0 $CURRENT

#------------------------------------
### updates from 2.4
#------------------------------------

update_test 2.4.2 $CURRENT
update_test 2.4.1 $CURRENT
update_test 2.4.0 $CURRENT

#------------------------------------
### updates from 2.3.0
#------------------------------------

update_test 2.3.2 $CURRENT
update_test 2.3.1 $CURRENT
update_test 2.3.0 $CURRENT

#------------------------------------
### updates from 2.2.x
#------------------------------------

update_test 2.2.4 $CURRENT
update_test 2.2.3 $CURRENT
update_test 2.2.2 $CURRENT
update_test 2.2.1 $CURRENT
update_test 2.2.0 $CURRENT


#------------------------------------
### updates from 2.1.x
#------------------------------------

update_test 2.1.0 $CURRENT

#------------------------------------
### updates from 2.0.x
#------------------------------------

update_test 2.0.0 $CURRENT

echo Reached end of test, all tests passed
# CAN NOT BE Update test from 2.0.1  to $CURRENT;

exit 0
