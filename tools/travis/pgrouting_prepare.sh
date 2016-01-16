#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Prepare pgRouting
# ------------------------------------------------------------------------------

DBUSER="postgres"
DBNAME="pgrouting"

POSTGRESQL_VERSION="$1"
POSTGIS_VERSION="$2"

POSTGRESQL_DIRECTORY="/usr/share/postgresql/$POSTGRESQL_VERSION"

echo "POSTGRESQL_VERSION=$POSTGRESQL_VERSION"
echo "POSTGIS_VERSION=$POSTGIS_VERSION"
echo "POSTGRESQL_DIRECTORY=/usr/share/postgresql/$POSTGRESQL_VERSION"
ls $POSTGRESQL_DIRECTORY/extension/postgis--*
ls /usr/lib/postgresql/$POSTGRESQL_VERSION/lib/postgis-*.so

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
# Set PostgreSQL users and permissions
# ------------------------------------------------------------------------------
sudo cp $TRAVIS_BUILD_DIR/tools/travis/pg_hba.conf `find /etc/postgresql/*/*/pg_hba.conf`
sudo /etc/init.d/postgresql restart	

# Disable password (better: create new user)
sudo -u $DBUSER psql -c "ALTER ROLE postgres WITH PASSWORD '';"

exit $ERROR

