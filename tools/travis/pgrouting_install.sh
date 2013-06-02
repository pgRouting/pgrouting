#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Install pgRouting prerequesits
# ------------------------------------------------------------------------------

POSTGIS_VERSION="$1"

# Add PPA for pgRouting Travis
sudo apt-add-repository -y ppa:georepublic/pgrouting-travis

if [[ "$POSTGIS_VERSION" == "1.5" ]]; then 

	sudo apt-add-repository -y ppa:ubuntugis/ubuntugis-unstable

fi
# Add PPA for PostGIS 2.x
if [[ "$POSTGIS_VERSION" != "1.5" ]]; then ; fi

sudo apt-get -q update

# Install dependecies
sudo apt-get install -q cmake libcgal-dev libboost-graph-dev libboost-thread-dev postgresql-server-dev-all

# Install PostGIS
sudo apt-get install -q postgresql-9.1-postgis

