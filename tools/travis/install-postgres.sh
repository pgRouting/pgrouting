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
POSTGIS_VERSION="2.3"

if [[ -z "$POSTGRESQL_VERSION" ]] ; then
    exit 1
fi


if [[ "$POSTGRESQL_VERSION" = "10" ]] ; then
    POSTGIS_VERSION="2.4"
fi

echo "Postgres $POSTGRESQL_VERSION"
echo "User $PGUSER"

sudo /etc/init.d/postgresql stop

echo "Installing postgresql-server-dev-$POSTGRESQL_VERSION postgresql-$POSTGRESQL_VERSION-postgis-$POSTGIS_VERSION"
sudo apt-get install -y \
    postgresql-server-dev-$POSTGRESQL_VERSION \
    postgresql-$POSTGRESQL_VERSION-postgis-$POSTGIS_VERSION




sudo cp /usr/lib/postgresql/$POSTGRESQL_VERSION/bin/pg_config /usr/bin/pg_config

sudo /etc/init.d/postgresql stop


sudo cat /etc/postgresql/$POSTGRESQL_VERSION/main/pg_hba.conf

sudo sed -i -e 's/port = 5433/port = 5432/g' /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
sudo cp $TRAVIS_BUILD_DIR/tools/travis/pg_hba.conf /etc/postgresql/$POSTGRESQL_VERSION/main/pg_hba.conf

ps -fea | grep postgres

sudo /etc/init.d/postgresql start $POSTGRESQL_VERSION

# intall pgtap after postgres instance has started
sudo apt-get install -y  postgresql-$POSTGRESQL_VERSION-pgtap
sudo apt-get install -y libtap-parser-sourcehandler-pgtap-perl
