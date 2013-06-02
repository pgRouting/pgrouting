#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

DBUSER="postgres"
DBNAME="pgrouting"

POSTGIS_VERSION="$1"

# CASE: PostGIS 1.5
if [[ "$POSTGIS_VERSION" == "1.5" ]]; then 

psql --quiet -U $DBUSER <<EOF 
    \set ON_ERROR_STOP TRUE 
    CREATE DATABASE $DBNAME;
    \c $DBNAME
    \i /usr/share/postgresql/9.1/contrib/postgis-1.5/postgis.sql
    \i /usr/share/postgresql/9.1/contrib/postgis-1.5/spatial_ref_sys.sql
    CREATE EXTENSION pgrouting;
EOF

if [ $? -ne 0 ]; then exit $?; fi
fi

# CASE: PostGIS 2.0
if [[ "$POSTGIS_VERSION" != "1.5" ]]; then 

psql --quiet -U $DBUSER <<EOF 
    \set ON_ERROR_STOP TRUE 
    CREATE DATABASE $DBNAME;
    \c $DBNAME
    CREATE EXTENSION postgis;
    CREATE EXTENSION pgrouting;
EOF

if [ $? -ne 0 ]; then exit $?; fi
fi

# Test runner
./tools/test-runner.pl
