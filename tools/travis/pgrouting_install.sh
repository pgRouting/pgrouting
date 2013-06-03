#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Install pgRouting prerequesits
# ------------------------------------------------------------------------------

POSTGRESQL_VERSION="$1"
POSTGIS_VERSION="$2"

# Add PPA's'
# ------------------------------------------------------------------------------
sudo apt-add-repository -y ppa:georepublic/pgrouting-travis

if [[ "$POSTGIS_VERSION" == "2.0" ]]; then 
	sudo apt-add-repository -y ppa:ubuntugis/ubuntugis-unstable
fi

sudo apt-get update -qq

# Install PostgreSQL
# ------------------------------------------------------------------------------

if [[ "$POSTGRESQL_VERSION" != "9.1" ]]; then 
	sudo service postgresql stop
	sudo cp /etc/postgresql/9.1/main/pg_hba.conf ./
	sudo apt-get remove postgresql postgresql-9.1 -qq --purge
	echo "deb http://apt.postgresql.org/pub/repos/apt/ $(lsb_release -cs)-pgdg main" > pgdg.list
	sudo mv pgdg.list /etc/apt/sources.list.d/
	wget --quiet -O - http://apt.postgresql.org/pub/repos/apt/ACCC4CF8.asc | sudo apt-key add -
	sudo apt-get update -qq
	sudo apt-get -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confnew" install -qq postgresql-$POSTGRESQL_VERSION postgresql-contrib-$POSTGRESQL_VERSION
	sudo service postgresql stop
	sudo cp ./pg_hba.conf /etc/postgresql/$POSTGRESQL_VERSION/main				
	sudo service postgresql start	
fi

# Install dependecies
# ------------------------------------------------------------------------------
sudo apt-get install -y -qq cmake libcgal-dev libboost-graph-dev libboost-thread-dev postgresql-server-dev-$POSTGRESQL_VERSION


# Install PostGIS
# ------------------------------------------------------------------------------

if [[ "$POSTGRESQL_VERSION" == "9.1" ]]; 
then 
	sudo apt-get install -y -qq postgresql-9.1-postgis
else
	sudo apt-get install -y -qq build-essential libxml2-dev libproj-dev libjson0-dev xsltproc docbook-xsl docbook-mathml libgeos-dev libgdal1-dev 

	if [[ "$POSTGIS_VERSION" == "1.5" ]]; then 
		wget --quiet http://download.osgeo.org/postgis/source/postgis-1.5.8.tar.gz

	fi

	if [[ "$POSTGIS_VERSION" == "2.0" ]]; then 
		wget --quiet http://download.osgeo.org/postgis/source/postgis-2.0.3.tar.gz
	fi

	if [[ "$POSTGIS_VERSION" == "2.1" ]]; then 
		sudo apt-get install -y -qq libpoppler-dev libarmadillo-dev libepsilon-dev liblzma-dev
		wget --quiet http://postgis.net/stuff/postgis-2.1.0beta3dev.tar.gz
	fi

	# Build and compile
	POSTGIS_TARBALL=$(find postgis-${POSTGIS_VERSION}.*.tar.gz)
	tar -xzf $POSTGIS_TARBALL 
	cd `basename $POSTGIS_TARBALL .tar.gz`
	./configure 
	make
	sudo make install
	sudo ldconfig

	# Build extension for PostGIS > 2.0
	if [[ "$POSTGIS_VERSION" != "1.5" ]]; then 
		cd extensions && make && sudo make install
	fi

	sudo service postgresql restart
fi

