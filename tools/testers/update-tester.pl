#!/bin/sh
dropdb -U postgres -h localhost test_update -p 5432
createdb -U postgres -h localhost test_update -p 5432
psql -U postgres -h localhost test_update -p 5432 <<EOF
create extension postgis;
create extension pgrouting with version '2.0.0';
select pgr_version();
alter extension pgrouting update to '2.1.0';
select pgr_version();

EOF

