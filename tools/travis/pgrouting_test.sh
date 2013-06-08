#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

DBUSER="postgres"
DBNAME="pgrouting"

POSTGRESQL_VERSION="$1"
POSTGIS_VERSION="$2"

POSTGRESQL_DIRECTORY="/usr/share/postgresql/$POSTGRESQL_VERSION"

# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?"-ne 0]; then echo "Test query failed: $@"; exit 1; fi 
}

# ------------------------------------------------------------------------------
# CREATE DATABASE
# ------------------------------------------------------------------------------
run_psql -U $DBUSER -l
run_psql -U $DBUSER -c "CREATE DATABASE $DBNAME;"

# ------------------------------------------------------------------------------
# CREATE EXTENSION
# ------------------------------------------------------------------------------
if [ "$POSTGRESQL_VERSION" == "8.4" ] || [ "$POSTGRESQL_VERSION" == "9.0" ]
then
    run_psql -U $DBUSER -d $DBNAME -f `find $POSTGRESQL_DIRECTORY/contrib -name "postgis.sql"`
    run_psql -U $DBUSER -d $DBNAME -f `find $POSTGRESQL_DIRECTORY/contrib -name "spatial_ref_sys.sql"`
    run_psql -U $DBUSER -d $DBNAME -f `find $POSTGRESQL_DIRECTORY/contrib -name "pgrouting.sql"`
fi

if [ "$POSTGRESQL_VERSION" == "9.1" ]
then
    if [ "$POSTGIS_VERSION" == "1.5" ]
    then 
        run_psql -U $DBUSER -d $DBNAME -f `find $POSTGRESQL_DIRECTORY/contrib -name "postgis.sql"`
        run_psql -U $DBUSER -d $DBNAME -f `find $POSTGRESQL_DIRECTORY/contrib -name "spatial_ref_sys.sql"`
        run_psql -U $DBUSER -d $DBNAME -f `find $POSTGRESQL_DIRECTORY/contrib -name "pgrouting.sql"`
    else
        run_psql -U $DBUSER -d $DBNAME -c "CREATE EXTENSION postgis;"
        run_psql -U $DBUSER -d $DBNAME -c "CREATE EXTENSION pgrouting;"
    fi
fi

if [ "$POSTGRESQL_VERSION" == "9.2" ] || [ "$POSTGRESQL_VERSION" == "9.3" ]
then
    run_psql -U $DBUSER -d $DBNAME -c "CREATE EXTENSION postgis;"
    run_psql -U $DBUSER -d $DBNAME -c "CREATE EXTENSION pgrouting;"
fi

# ------------------------------------------------------------------------------
# Get version information
# ------------------------------------------------------------------------------
run_psql -U $DBUSER -d $DBNAME -c "SELECT postgis_full_version();"    
run_psql -U $DBUSER -d $DBNAME -c "SELECT pgr_version();"

PGROUTING_VERSION=`run_psql -U $DBUSER -A -t -d $DBNAME -c "SELECT version FROM pgr_version();"`

ls $POSTGRESQL_DIRECTORY/extension

# ------------------------------------------------------------------------------
# Test runner
# ------------------------------------------------------------------------------
./tools/test-runner.pl -pgver $POSTGRESQL_VERSION
