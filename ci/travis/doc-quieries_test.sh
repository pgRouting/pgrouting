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

echo "POSTGRESQL_VERSION $POSTGRESQL_VERSION"

# exit script on error
set -e
ERROR=0

# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U "${PGUSER}"  -d "${PGDATABASE}" -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then
        echo "Query failed:" "$@"
        exit 1
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
run_psql -c "SELECT pgr_full_version();"


# ------------------------------------------------------------------------------
# Regenerate the signature file & check if it was updated
# ------------------------------------------------------------------------------
VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
DIFF=$(git diff "sql/sigs/pgrouting--${VERSION}.sig")

if [[ !  -z  "${DIFF}"  ]]
then
    echo Signature file not updated
    echo "${DIFF}"
    exit 1
fi

TESTDIRS="version"
for d in ${TESTDIRS}
do
    ./tools/testers/doc_queries_generator.pl -pgver "${POSTGRESQL_VERSION}" -pguser "${PGUSER}" -documentation -alg "${d}"
done

./tools/testers/doc_queries_generator.pl -pgver "${POSTGRESQL_VERSION}" -pguser "${PGUSER}"
