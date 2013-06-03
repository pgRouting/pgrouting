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

# Define alias for psql command
alias run_psql='PGOPTIONS='--client-min-messages=warning' psql -X -q -a -1 -v ON_ERROR_STOP=1 --pset pager=off'

# ------------------------------------------------------------------------------
# CREATE DATABASE
# ------------------------------------------------------------------------------
run_psql -U postgres -c "CREATE DATABASE $DBNAME;"

# ------------------------------------------------------------------------------
# CREATE EXTENSION
# ------------------------------------------------------------------------------
if [[ "$POSTGRESQL_VERSION" == "8.4" ] || [ "$POSTGRESQL_VERSION" == "9.0" ]]
then
    run_psql -U postgres -d $DBNAME -f /usr/share/postgresql/$POSTGRESQL_VERSION/contrib/postgis-$POSTGIS_VERSION/postgis.sql
    run_psql -U postgres -d $DBNAME -f /usr/share/postgresql/$POSTGRESQL_VERSION/contrib/postgis-$POSTGIS_VERSION/spatial_ref_sys.sql
    run_psql -U postgres -d $DBNAME -f `find /usr/share -name "pgrouting--*.sql"`
fi

if [[ "$POSTGRESQL_VERSION" == "9.1" ]]
then
    if [[ "$POSTGIS_VERSION" == "1.5" ]]
    then 
        run_psql -U postgres -d $DBNAME -f /usr/share/postgresql/$POSTGRESQL_VERSION/contrib/postgis-$POSTGIS_VERSION/postgis.sql
        run_psql -U postgres -d $DBNAME -f /usr/share/postgresql/$POSTGRESQL_VERSION/contrib/postgis-$POSTGIS_VERSION/spatial_ref_sys.sql
        run_psql -U postgres -d $DBNAME -f `find /usr/share -name "pgrouting--*.sql"`
    else
        run_psql -U postgres -d $DBNAME -c "CREATE EXTENSION postgis;"
        run_psql -U postgres -d $DBNAME -c "CREATE EXTENSION pgrouting;"
    fi
fi

if [[ "$POSTGRESQL_VERSION" == "9.2" ] || [ "$POSTGRESQL_VERSION" == "9.3" ]]
then
    run_psql -U postgres -d $DBNAME -c "CREATE EXTENSION postgis;"
    run_psql -U postgres -d $DBNAME -c "CREATE EXTENSION pgrouting;"
fi

# Test runner
#./tools/test-runner.pl
