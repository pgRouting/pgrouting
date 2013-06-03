#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Prepare pgRouting
# ------------------------------------------------------------------------------

POSTGRESQL_VERSION="$1"
POSTGIS_VERSION="$2"

# ------------------------------------------------------------------------------
# Set PostgreSQL users and permissions
# ------------------------------------------------------------------------------
find /etc/postgresql/*/main/pg_hba.conf
ls -l  /etc/postgres*

sudo cp $TRAVIS_BUILD_DIR/tools/travis/pg_hba.conf `find /etc/postgresql/*/main/pg_hba.conf`
sudo service postgresql restart	

# Disable password (better: create new user)
psql -U postgres -d postgres -c "ALTER ROLE postgres WITH PASSWORD '';"
