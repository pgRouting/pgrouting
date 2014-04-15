#!/bin/bash
# ------------------------------------------------------------------------------
# Vagrant - Virtualized Development 
# Copyright(c) pgRouting Contributors
#
# Virtual environment bootstrap script
# ------------------------------------------------------------------------------

# set -e # Exit script immediately on first error.
# set -x # Print commands and their arguments as they are executed.

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
	apt-add-repository -y ppa:ubuntugis/ppa
fi

# Add PostgreSQL Apt repository
# ------------------------------------------------------------------------------
echo "deb http://apt.postgresql.org/pub/repos/apt/ $(lsb_release -cs)-pgdg main" > pgdg.list
sudo mv pgdg.list /etc/apt/sources.list.d/
wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
sudo apt-get update -qq

# Install packages
# ------------------------------------------------------------------------------
echo "Installing packages ... this may take some time."
apt-get install -y -qq packaging-dev checkinstall libcgal-dev libboost-graph-dev libboost-thread-dev postgresql-$POSTGRESQL_VERSION postgresql-contrib-$POSTGRESQL_VERSION postgresql-server-dev-$POSTGRESQL_VERSION libxml2-dev libproj-dev libjson0-dev xsltproc docbook-xsl docbook-mathml libgeos-dev libgdal1-dev texlive texlive-latex-extra

# ------------------------------------------------------------------------------
# Manage localization with Transifex
# https://www.transifex.com/projects/p/pgrouting/ 
# http://sphinx.readthedocs.org/en/latest/intl.html
# ------------------------------------------------------------------------------
apt-get install python-pip python-dev
pip install sphinx transifex-client sphinx-intl

# ------------------------------------------------------------------------------
# Install PostGIS (always build from source)
# ------------------------------------------------------------------------------

if [ "$POSTGIS_VERSION" == "1.5" ]; then 
	RELEASE="1.5.8"
	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-${RELEASE}.tar.gz | tar xzf -
fi

if [ "$POSTGIS_VERSION" == "2.0" ]; then 
	RELEASE="2.0.4"
	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-${RELEASE}.tar.gz | tar xzf -
fi

if [ "$POSTGIS_VERSION" == "2.1" ]; then 
	sudo apt-get install -y -qq libpoppler-dev libarmadillo-dev libepsilon-dev liblzma-dev libxml2-dev
	RELEASE="2.1.1"
	wget --quiet -O - http://download.osgeo.org/postgis/source/postgis-${RELEASE}.tar.gz | tar xzf -
fi

# Build and compile
cd postgis-*
./autogen.sh
./configure && make && make install
ldconfig

# Build extension for PostGIS > 2.0
if [ "$POSTGIS_VERSION" != "1.5" ]; then 
	cd extensions
	make
	make install
	ldconfig
fi

# ------------------------------------------------------------------------------
# Restart once
# ------------------------------------------------------------------------------
/etc/init.d/postgresql restart
