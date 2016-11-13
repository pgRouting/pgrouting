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
    PGOPTIONS='--client-min-messages=warning' psql -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then 
        echo "Test query failed: $@"
        ERROR=1
    fi 
}

# ------------------------------------------------------------------------------
# CREATE DATABASE
# ------------------------------------------------------------------------------
export PGUSER
run_psql -l
run_psql -c "CREATE DATABASE $PGDATABASE;"
export PGDATABASE

# ------------------------------------------------------------------------------
# CREATE EXTENSION
# ------------------------------------------------------------------------------
IGNORE=-ignorenotice
#if [ "$POSTGRESQL_VERSION" == "8.4" ] || [ "$POSTGRESQL_VERSION" == "9.0" ]
#then
#    run_psql -f `find $POSTGRESQL_DIRECTORY/contrib -name "postgis.sql"`
#    run_psql -f `find $POSTGRESQL_DIRECTORY/contrib -name "spatial_ref_sys.sql"`
#    run_psql -f `find $POSTGRESQL_DIRECTORY/contrib -name "pgrouting.sql"`
#    IGNORE=-ignorenotice
#fi

#if [ "$POSTGRESQL_VERSION" == "9.1" ]
#then
#    if [ "$POSTGIS_VERSION" == "1.5" ]
#    then 
#        run_psql -f `find $POSTGRESQL_DIRECTORY/contrib -name "postgis.sql"`
#        run_psql -f `find $POSTGRESQL_DIRECTORY/contrib -name "spatial_ref_sys.sql"`
#        run_psql -f `find $POSTGRESQL_DIRECTORY/contrib -name "pgrouting.sql"`
#    else
#        run_psql -c "CREATE EXTENSION postgis;"
#        run_psql -c "CREATE EXTENSION pgrouting;"
#    fi
#    IGNORE=-ignorenotice
#fi

#if [ "$POSTGRESQL_VERSION" == "9.2" ] || [ "$POSTGRESQL_VERSION" == "9.3" ] || [ "$POSTGRESQL_VERSION" == "9.4" ]
#then
#we are allways creating extension
    run_psql -c "CREATE EXTENSION postgis;"
    run_psql -c "CREATE EXTENSION pgrouting;"
#fi

# ------------------------------------------------------------------------------
# Get version information
# ------------------------------------------------------------------------------
run_psql -c "SELECT postgis_full_version();"    
run_psql -c "SELECT pgr_version();"

PGROUTING_VERSION=`run_psql -A -t -c "SELECT version FROM pgr_version();"`

# ------------------------------------------------------------------------------
# Test runner
# ------------------------------------------------------------------------------
# use -v -v for more verbose debuging output
# ./tools/test-runner.pl -v -v -pgver $POSTGRESQL_VERSION
 ./tools/test-runner.pl -pgver $POSTGRESQL_VERSION $IGNORE -clean 
#./tools/test-runner.pl -pgver $POSTGRESQL_VERSION $IGNORE -v -alg ksp
if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
