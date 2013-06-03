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
sudo cp $TRAVIS_BUILD_DIR/tools/travis/pg_hba.conf `find /etc/postgresql/*/main/pg_hba.conf`
sudo /etc/init.d/postgresql restart	

sudo cat `find /etc/postgresql/*/main/pg_hba.conf`

# Disable password (better: create new user)
sudo -u postgres psql -c "ALTER ROLE postgres WITH PASSWORD '';"
