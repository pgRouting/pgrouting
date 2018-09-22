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

# ----------------------
#
echo **All pgRouting versions to be updated must be installed before calling**
echo
echo
#
# USAGE
#
#  bash tools/testers/update-tester.sh
#

CURRENT=2.6.1

if [ ! -f build/sql/pgrouting--$CURRENT.sql ]; then
   echo "Not testing the current version, file not found:  build/lib/pgrouting--$CURRENT.sql "
   exit 1
fi

dropdb --if-exists ___test_update


cd build
sudo make install
cd ..



function update_test {

echo
echo
echo Updating from $1 to $2
echo ------------------------------------

INSTALLED=$(locate /usr/share/postgresql/9.5/extension/pgrouting--$1.sql)

if [ "$INSTALLED" == "/usr/share/postgresql/9.5/extension/pgrouting--$1.sql" ]
then
    echo "/usr/share/postgresql/9.5/extension/pgrouting--$1.sql found"
else
    echo "FATAL: /usr/share/postgresql/9.5/extension/pgrouting--$1.sql Not found"
    exit 1
fi


createdb  ___test_update
psql  ___test_update  <<EOF
SELECT version();
CREATE extension postgis;
CREATE extension pgrouting with version '$1';
EOF

OLD_VERSION=$(psql ___test_update -t -c 'SELECT version FROM pgr_version()')


if [ "b$OLD_VERSION" != "b $1" ]
then
    echo "ERROR: Version $1 not found on the system"
    dropdb ___test_update
    exit 1
fi



psql ___test_update -c "ALTER extension pgrouting update to '$2'"


NEW_VERSION=$(psql ___test_update -t -c 'SELECT version FROM pgr_version()')

echo "$OLD_VERSION ->> $NEW_VERSION"

if [ "b$NEW_VERSION" != "b $2" ]
then
    echo "FAIL: Could not update from version $1 to version $2"
    dropdb ___test_update
    exit 1
fi

dropdb ___test_update

} # end of function

#------------------------------------
### updates from 2.6
#------------------------------------

update_test 2.6.0 $CURRENT

#------------------------------------
### updates from 2.5
#------------------------------------

update_test 2.5.4 $CURRENT
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
