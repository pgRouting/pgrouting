#!/bin/sh

set -e



### updates from 2.2.x

# Update test from 2.2.4  to 2.3.0

function update_test {
echo $1 $2
createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '$1';
select pgr_version();
alter extension pgrouting update to '$2';
select pgr_version();
EOF

} 

update_test 2.2.4 2.3.0



# Update test from 2.2.3  to 2.3.0

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '2.2.3';
select pgr_version();
alter extension pgrouting update to '2.3.0';
select pgr_version();
EOF
dropdb   ___test_update



# Update test from 2.2.2  to 2.3.0

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '2.2.2';
select pgr_version();
alter extension pgrouting update to '2.3.0';
select pgr_version();
EOF
dropdb   ___test_update



# Update test from 2.2.1  to 2.3.0

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '2.2.1';
select pgr_version();
alter extension pgrouting update to '2.3.0';
select pgr_version();
EOF
dropdb   ___test_update


# Update test from 2.2.0  to 2.3.0

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '2.2.0';
select pgr_version();
alter extension pgrouting update to '2.3.0';
select pgr_version();
EOF
dropdb   ___test_update

#------------------------------------
### updates from 2.1.x
#------------------------------------

# Update test from 2.1.0  to 2.3.0

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '2.1.0';
select pgr_version();
alter extension pgrouting update to '2.3.0';
select pgr_version();
EOF
dropdb   ___test_update

#------------------------------------
### updates from 2.0.x
#------------------------------------

# Update test from 2.0.0  to 2.3.0

createdb  ___test_update
psql  ___test_update  <<EOF
create extension postgis;
create extension pgrouting with version '2.0.0';
select pgr_version();
alter extension pgrouting update to '2.1.0';
select pgr_version();
alter extension pgrouting update to '2.3.0';
select pgr_version();
EOF
dropdb   ___test_update

# CAN NOT BE Update test from 2.0.1  to 2.3.0


