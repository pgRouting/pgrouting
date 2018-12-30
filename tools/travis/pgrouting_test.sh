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


# ------------------------------------------------------------------------------
# Regenerate the signature file & check if it was updated
# ------------------------------------------------------------------------------
VERSION=`grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt`
FILE="sql/sigs/pgrouting--$VERSION.sig"
echo "#VERSION pgrouting $VERSION" > $FILE
echo "#TYPES" >> $FILE
run_psql -c '\dx+ pgrouting' -A | grep '^type' | cut -d ' ' -f2- | sort >> $FILE
echo "#FUNCTIONS" >> $FILE
run_psql  -c '\dx+ pgrouting' -A | grep '^function' | cut -d ' ' -f2- | sort >> $FILE

DIFF=`git diff sql/sigs/pgrouting--${VERSION}.sig`

if [[ !  -z  $DIFF  ]]
then
    echo Signature file not updated
    echo $DIFF
    ERROR=1
fi

./tools/testers/algorithm-tester.pl -pgver $POSTGRESQL_VERSION -pguser $PGUSER

if [ "$?" -ne 0 ]
then
    ERROR=1
fi

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
