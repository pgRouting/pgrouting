#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Install pgRouting prerequesits
# ------------------------------------------------------------------------------

POSTGRESQL_VERSION="$1"
POSTGIS_VERSION="$2"

# exit script on error
set -e 
ERROR=0

# ------------------------------------------------------------------------------
# Remove PostgreSQL and all its files
# ------------------------------------------------------------------------------
#sudo service postgresql stop
#sudo apt-get remove postgresql libpq-dev libpq5 postgresql-client-common postgresql-common -qq --purge -y 
#sudo rm -rf /var/lib/postgresql

# Add PPA's'
# ------------------------------------------------------------------------------
#sudo apt-add-repository -y ppa:georepublic/pgrouting-travis

#if [ "$POSTGIS_VERSION" == "2.0" ] || [ "$POSTGIS_VERSION" == "2.1" ]; then 
#	sudo apt-add-repository -y ppa:ubuntugis/ppa
#fi

# Add PostgreSQL Apt repository
# UPDATE: seems to be already available in Travis
# ------------------------------------------------------------------------------
# echo "deb http://apt.postgresql.org/pub/repos/apt/ $(lsb_release -cs)-pgdg main" > pgdg.list
# sudo mv pgdg.list /etc/apt/sources.list.d/
# wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
#sudo apt-get update -qq

# ------------------------------------------------------------------------------
# Install PostgreSQL (always install from PostgreSQL Apt repository)
# ------------------------------------------------------------------------------
#sudo apt-get -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confnew" install -y -qq postgresql-$POSTGRESQL_VERSION postgresql-contrib-$POSTGRESQL_VERSION postgresql-server-dev-$POSTGRESQL_VERSION

# Install packages
# ------------------------------------------------------------------------------
echo "Installing packages ... this may take some time."
#sudo apt-get install -y -qq packaging-dev cmake checkinstall libcgal-dev libboost-graph-dev libboost-thread-dev libxml2-dev libproj-dev libjson0-dev xsltproc docbook-xsl docbook-mathml libgeos-dev libgdal1-dev 

#if [ "$POSTGIS_VERSION" == "1.5" ]; then 
#	RELEASE="1.5.8"
#	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-${RELEASE}.tar.gz | tar xzf -
#fi

#if [ "$POSTGIS_VERSION" == "2.0" ]; then 
#	RELEASE="2.0.7"
#	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-${RELEASE}.tar.gz | tar xzf -
#fi

#if [ "$POSTGIS_VERSION" == "2.1" ]; then 
#	sudo apt-get install -y -qq libpoppler-dev libarmadillo-dev libepsilon-dev liblzma-dev libxml2-dev
#	RELEASE="2.1.7"
#	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-${RELEASE}.tar.gz | tar xzf -
#fi

# Build and compile
#cd postgis-*
#./autogen.sh
#./configure 
#make
#sudo make install
#sudo ldconfig

# Build extension for PostGIS > 2.0
#if [ "$POSTGIS_VERSION" != "1.5" ]; then 
#	cd extensions && make && sudo make install
#fi

# ------------------------------------------------------------------------------
# Restart once
# ------------------------------------------------------------------------------
#sudo /etc/init.d/postgresql restart

# Return success or failure
# ------------------------------------------------------------------------------
exit $ERROR
