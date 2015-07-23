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

# WE ARE NOT USING TEMPLATE DATABASES SO DON'T GO BELOW HERE

# ------------------------------------------------------------------------------
# Create database templates
# ------------------------------------------------------------------------------

# PostGIS
# ------------------------------------------------------------------------------
#run_psql -U $DBUSER -c "CREATE DATABASE template_postgis ENCODING 'UTF8' TEMPLATE template0;"

#if [ "$POSTGRESQL_VERSION" == "8.4" ] 
#then
#    run_psql -U $DBUSER -d template_postgis -c "CREATE LANGUAGE plpgsql;"
#fi

#run_psql -U $DBUSER -d template_postgis -f `find $POSTGRESQL_DIRECTORY/contrib -name "postgis.sql"`
#run_psql -U $DBUSER -d template_postgis -f `find $POSTGRESQL_DIRECTORY/contrib -name "spatial_ref_sys.sql"`
#
#run_psql -U $DBUSER -d template_postgis -c "GRANT ALL ON geometry_columns TO PUBLIC;"
#run_psql -U $DBUSER -d template_postgis -c "GRANT ALL ON geography_columns TO PUBLIC;"
#run_psql -U $DBUSER -d template_postgis -c "GRANT ALL ON spatial_ref_sys TO PUBLIC;"

#run_psql -U $DBUSER -d template_postgis -c "VACUUM FULL;"
#run_psql -U $DBUSER -d template_postgis -c "VACUUM FREEZE;"

#run_psql -U $DBUSER -c "UPDATE pg_database SET datistemplate='true' WHERE datname='template_postgis';"
#run_psql -U $DBUSER -c "UPDATE pg_database SET datallowconn='false' WHERE datname='template_postgis';"

# pgRouting
# ------------------------------------------------------------------------------
#run_psql -U $DBUSER -c "CREATE DATABASE template_pgrouting ENCODING 'UTF8' TEMPLATE template_postgis;"
#run_psql -U $DBUSER -d template_pgrouting -f `find $POSTGRESQL_DIRECTORY/contrib -name "pgrouting.sql"`

#run_psql -U $DBUSER -d template_pgrouting -c "VACUUM FULL;"
#run_psql -U $DBUSER -d template_pgrouting -c "VACUUM FREEZE;"

#run_psql -U $DBUSER -c "UPDATE pg_database SET datistemplate='true' WHERE datname='template_pgrouting';"
#run_psql -U $DBUSER -c "UPDATE pg_database SET datallowconn='false' WHERE datname='template_pgrouting';"

# Return success or failure
# ------------------------------------------------------------------------------
#exit $ERROR
