#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Install pgRouting prerequesits
# ------------------------------------------------------------------------------

set -e

POSTGRESQL_VERSION="$1"
PGUSER="$2"

if [[ -z "$POSTGRESQL_VERSION" ]] ; then
    exit 1
fi


if [[ "$POSTGRESQL_VERSION" = "9.5" ||  "$POSTGRESQL_VERSION" = "9.6"  ]] ; then
    POSTGIS_VERSION="2.3"
fi

echo "Postgres $POSTGRESQL_VERSION"
echo "User $PGUSER"

sudo /etc/init.d/postgresql stop
sudo apt-get install -y \
    postgresql-server-dev-$POSTGRESQL_VERSION

if [[ "$POSTGRESQL_VERSION" = "9.5" ||  "$POSTGRESQL_VERSION" = "9.6"  ]] ; then

    ## removing unused postgresql
    #sudo apt-get -y remove --purge postgresql-9.1
    #sudo apt-get -y remove --purge postgresql-9.2
    #sudo apt-get -y remove --purge postgresql-9.3
    #sudo apt-get -y remove --purge postgresql-9.4
    #sudo apt-get -y remove --purge postgresql-9.5
    #sudo apt-get -y remove --purge postgresql-9.6

    echo "Installing postgresql $POSTGRESQL_VERSION  & postgis "

    sudo apt-get install -y \
        postgresql-$POSTGRESQL_VERSION \
        postgresql-$POSTGRESQL_VERSION-postgis-$POSTGIS_VERSION

    sudo cp /usr/lib/postgresql/$POSTGRESQL_VERSION/bin/pg_config /usr/bin/pg_config

    sudo /etc/init.d/postgresql stop


    sudo cat /etc/postgresql/$POSTGRESQL_VERSION/main/pg_hba.conf

    sudo sed -i -e 's/port = 5433/port = 5432/g' /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
    sudo cp $TRAVIS_BUILD_DIR/tools/travis/pg_hba.conf /etc/postgresql/$POSTGRESQL_VERSION/main/pg_hba.conf

    ps -fea | grep postgres
fi

sudo /etc/init.d/postgresql start $POSTGRESQL_VERSION

# intall pgtap after postgres instance has started
sudo apt-get install -y  postgresql-$POSTGRESQL_VERSION-pgtap
sudo apt-get install -y libtap-parser-sourcehandler-pgtap-perl
