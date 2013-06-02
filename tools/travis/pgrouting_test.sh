#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

DBUSER="postgres"
DBNAME="pgrouting"

# Create database 
psql -U $DBUSER -c 'CREATE DATABASE $DBNAME;'

# Add PostGIS extension to database
if [[ "$POSTGIS_VERSION" == "1.5" ]]; then 
	psql -U $DBUSER -d $DBNAME -f /usr/share/postgresql/9.1/contrib/postgis-1.5/postgis.sql
	psql -U $DBUSER -d $DBNAME -f /usr/share/postgresql/9.1/contrib/postgis-1.5/spatial_ref_sys.sql
fi

if [[ "$POSTGIS_VERSION" != "1.5" ]]; then 
	psql -U $DBUSER -d $DBNAME -c "CREATE EXTENSION postgis;" 
fi

# Add pgRouting extension to database
psql -U $DBUSER -d $DBNAME -c "CREATE EXTENSION pgrouting;"

# Test runner
../tools/test-runner.pl
