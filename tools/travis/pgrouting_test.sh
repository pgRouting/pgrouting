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

# ------------------------------------------------------------------------------
# CASE: PostGIS 1.5
# ------------------------------------------------------------------------------
if [[ "$POSTGIS_VERSION" == "1.5" ]]; then 

PGROUTING_SQLFILE=$(find /usr/share/postgresql/ -name pgrouting--2.*)

psql --quiet -U $DBUSER <<EOF 
    \set ON_ERROR_STOP TRUE 
    CREATE DATABASE $DBNAME;
    \c $DBNAME
    \i /usr/share/postgresql/9.1/contrib/postgis-1.5/postgis.sql
    \i /usr/share/postgresql/9.1/contrib/postgis-1.5/spatial_ref_sys.sql
    \i $PGROUTING_SQLFILE
EOF

if [ $? -ne 0 ]; then exit $?; fi
fi

# ------------------------------------------------------------------------------
# CASE: PostGIS 2.0
# ------------------------------------------------------------------------------
if [[ "$POSTGIS_VERSION" == "2.0" ]]; then 

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
