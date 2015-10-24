#!/bin/sh
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

PGUSER="postgres"
PGDATABASE="pgrouting"

POSTGRESQL_VERSION="$1"
POSTGIS_VERSION="$2"

POSTGRESQL_DIRECTORY="/usr/share/postgresql/$POSTGRESQL_VERSION"

# exit script on error
set -e 
ERROR=0

# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U $PGUSER  -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then 
        echo "Test query failed: $@"
        ERROR=1
    fi 
}

# ------------------------------------------------------------------------------
# CREATE DATABASE
# ------------------------------------------------------------------------------
#export PGUSER
#run_psql -l
#run_psql -c "CREATE DATABASE $PGDATABASE;"
#export PGDATABASE

# ------------------------------------------------------------------------------
# CREATE EXTENSION
# ------------------------------------------------------------------------------
#run_psql -d $PGDATABASE -c "CREATE EXTENSION postgis;"

# ------------------------------------------------------------------------------
# Get version information
# ------------------------------------------------------------------------------
#run_psql -d $PGDATABASE -c "SELECT version();"    
#run_psql -d $PGDATABASE -c "SELECT postgis_full_version();"    

#run_psql -d $PGDATABASE -c "CREATE EXTENSION pgrouting;"
#run_psql -d $PGDATABASE -c "SELECT pgr_version();"

#PGROUTING_VERSION=`run_psql -A -t -c "SELECT version FROM pgr_version();"`

# ------------------------------------------------------------------------------
# Test runner
# ------------------------------------------------------------------------------
# use -v -v for more verbose debuging output
# ./tools/test-runner.pl -v -v -pgver $POSTGRESQL_VERSION
#./tools/test-runner.pl -pgver $POSTGRESQL_VERSION $IGNORE 
#./tools/test-runner.pl -pgver $POSTGRESQL_VERSION $IGNORE -v -alg ksp

./tools/test-runner.pl -pgver $POSTGRESQL_VERSION -ignorenotice

if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
