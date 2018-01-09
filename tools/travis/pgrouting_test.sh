#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

if [[ "$DOCUMENTATION" == "ON" ]]
then
    exit 0
fi


PGDATABASE="pgr_test__db__test"

POSTGRESQL_VERSION="$1"
PGUSER="$2"

#POSTGIS_VERSION="$2"

POSTGRESQL_DIRECTORY="/usr/share/postgresql/$POSTGRESQL_VERSION"
echo "POSTGRESQL_VERSION $POSTGRESQL_VERSION"

# exit script on error
set -e
ERROR=0

# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U $PGUSER  -d $PGDATABASE -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
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
#run_psql -c "CREATE DATABASE ____tmp_pgdb______;"
#export PGDATABASE

# ------------------------------------------------------------------------------
# CREATE EXTENSION
# ------------------------------------------------------------------------------
run_psql  -c "CREATE EXTENSION postgis;"
run_psql  -c "CREATE EXTENSION pgrouting;"

# ------------------------------------------------------------------------------
# Get version information
# ------------------------------------------------------------------------------
run_psql -c "SELECT version();"
run_psql -c "SELECT postgis_full_version();"
run_psql -c "SELECT pgr_version();"

#PGROUTING_VERSION=`run_psql -A -t -c "SELECT version FROM pgr_version();"`

# ------------------------------------------------------------------------------
# Test runner
# ------------------------------------------------------------------------------
# use -v -v for more verbose debuging output
# ./tools/test-runner.pl -v -v -pgver $POSTGRESQL_VERSION
#./tools/test-runner.pl -pgver $POSTGRESQL_VERSION $IGNORE
#./tools/test-runner.pl -pgver $POSTGRESQL_VERSION $IGNORE -v -alg ksp

#cd ./tools/testers/
#psql -f setup_db.sql
#pg_prove ../../src/trsp/test/pgtap/*
#dropdb ___pgr___test___
#cd ../../

./tools/testers/algorithm-tester.pl -pgver $POSTGRESQL_VERSION -pguser $PGUSER

if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
