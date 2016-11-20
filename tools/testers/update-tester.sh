#!/bin/bash
#/*PGR-GNU*****************************************************************

# FILE: update_tester.sh

# Copyright (c) 2015 pgRouting developers
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
echo ------------------------------------
echo All pgRouting versions to be updated must be installed before calling
echo ------------------------------------
echo -
echo -
#
# USAGE
#
#  bash tools/testers/update-tester.sh
#

CURRENT=2.3.1

function update_test {
set -e

echo ------------------------------------
echo ------------------------------------
echo Updating from $1 to $2
echo ------------------------------------

INSTALLED=$(locate /usr/share/postgresql/9.3/extension/pgrouting--$1.sql)

if [ "$INSTALLED" == "/usr/share/postgresql/9.3/extension/pgrouting--$1.sql" ]
then
    echo "/usr/share/postgresql/9.3/extension/pgrouting--$1.sql found"
else
    echo "/usr/share/postgresql/9.3/extension/pgrouting--$1.sql Not found"
    exit 1
fi

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '$1';
select pgr_version();
alter extension pgrouting update to '$2';
select pgr_version();
EOF

dropdb   ___test_update
} 

#------------------------------------
### updates from 2.3.0
#------------------------------------

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

echo ------------------------------------
echo ------------------------------------
echo Updating from 2.0.0  to $CURRENT
echo ------------------------------------

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '2.0.0';
select pgr_version();
alter extension pgrouting update to '2.1.0';
select pgr_version();
alter extension pgrouting update to '$CURRENT';
select pgr_version();
EOF
dropdb   ___test_update

# CAN NOT BE Update test from 2.0.1  to $CURRENT;


