#!/bin/bash
# ------------------------------------------------------------------------------
# Vagrant - Virtualized Development 
# Copyright(c) pgRouting Contributors
#
# Virtual environment bootstrap script
# ------------------------------------------------------------------------------

set -e # Exit script immediately on first error.
#set -x # Print commands and their arguments as they are executed.

# Abort provisioning if pgRouting development environment already setup.
# ------------------------------------------------------------------------------
which cmake >/dev/null &&
{ echo "pgRouting development environment already setup."; exit 0; }

# Run provisioning
# ------------------------------------------------------------------------------
POSTGRESQL_VERSION="$1"
POSTGIS_VERSION="$2"

echo "PostgreSQL version: $POSTGRESQL_VERSION"
echo "PostGIS version: $POSTGIS_VERSION"

# Enable PPA support
# ------------------------------------------------------------------------------
apt-get update -qq
apt-get install -y -qq python-software-properties vim

# Add PPA's'
# ------------------------------------------------------------------------------
apt-add-repository -y ppa:georepublic/pgrouting-travis

if [ "$POSTGIS_VERSION" == "2.0" ] || [ "$POSTGIS_VERSION" == "2.1" ]; then 
	apt-add-repository -y ppa:ubuntugis/ubuntugis-unstable
fi

# Add PostgreSQL Apt repository
# ------------------------------------------------------------------------------
if [ "$POSTGRESQL_VERSION" != "9.1" ]; then
	echo "deb http://apt.postgresql.org/pub/repos/apt/ $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list
	wget --quiet -O - http://apt.postgresql.org/pub/repos/apt/ACCC4CF8.asc | apt-key add -
fi

apt-get update -qq

# Install packages
# ------------------------------------------------------------------------------
echo "Installing packages ... this may take some time."
apt-get install -y -qq packaging-dev checkinstall python-sphinx libcgal-dev libboost-graph-dev libboost-thread-dev postgresql-$POSTGRESQL_VERSION postgresql-contrib-$POSTGRESQL_VERSION postgresql-server-dev-$POSTGRESQL_VERSION libxml2-dev libproj-dev libjson0-dev xsltproc docbook-xsl docbook-mathml libgeos-dev libgdal1-dev

# ------------------------------------------------------------------------------
# Install PostGIS (always build from source)
# ------------------------------------------------------------------------------

if [ "$POSTGIS_VERSION" == "1.5" ]; then 
	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-1.5.8.tar.gz | tar xzf -
fi

if [ "$POSTGIS_VERSION" == "2.0" ]; then 
	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-2.0.3.tar.gz | tar xzf -
fi

if [ "$POSTGIS_VERSION" == "2.1" ]; then 
	apt-get install -y -qq libpoppler-dev libarmadillo-dev libepsilon-dev liblzma-dev libxml2-dev
	wget --quiet -O - https://github.com/postgis/postgis/archive/svn-trunk.tar.gz | tar xzf -
fi

# Build and compile
cd postgis-*
./autogen.sh
./configure && make && make install
ldconfig

# Build extension for PostGIS > 2.0
if [ "$POSTGIS_VERSION" != "1.5" ]; then 
	cd extensions && make && make install
fi

# ------------------------------------------------------------------------------
# Restart once
# ------------------------------------------------------------------------------
/etc/init.d/postgresql restart

