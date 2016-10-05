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

## removing unused postgresql
sudo apt-get -y remove --purge postgresql-9.1
sudo apt-get -y remove --purge postgresql-9.2
sudo apt-get -y remove --purge postgresql-9.3
sudo apt-get -y remove --purge postgresql-9.4
sudo apt-get -y remove --purge postgresql-9.5
sudo apt-get -y remove --purge postgresql-9.6

echo "Installing postgresql $POSTGRESQL_VERSION  & postgis "
# sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt/ $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list'
# sudo apt-get install wget ca-certificates
# wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
# sudo apt-get update

sudo apt-get install -y \
    postgresql-$POSTGRESQL_VERSION \
    postgresql-server-dev-$POSTGRESQL_VERSION  \
    postgresql-$POSTGRESQL_VERSION-postgis-$POSTGIS_VERSION

#sudo apt-get install -y pgtap libtap-parser-sourcehandler-pgtap-perl
sudo cp /usr/lib/postgresql/$POSTGRESQL_VERSION/bin/pg_config /usr/bin/pg_config
sudo /etc/init.d/postgresql stop
sudo /etc/init.d/postgresql stop
ps -fea | grep postgres

#sudo sh -c 'echo "local all postgres trust" > /etc/postgresql/9.5/main/pg_hba.conf'
#sudo sh -c 'echo -n "host all all 127.0.0.1/32 trust" >> /etc/postgresql/9.5/main/pg_hba.conf'

sudo cat /etc/postgresql/9.5/main/pg_hba.conf

#echo "making grep before change"
#grep port /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
#echo "finished grep"
sudo sed -i -e 's/port = 5433/port = 5432/g' /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
#echo "making grep after change"
#grep port /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
#echo "finished grep"
sudo cp $TRAVIS_BUILD_DIR/tools/travis/pg_hba.conf /etc/postgresql/$POSTGRESQL_VERSION/main/pg_hba.conf
sudo /etc/init.d/postgresql start $POSTGRESQL_VERSION
ps -fea | grep postgres

sudo apt-get install -y  postgresql-$POSTGRESQL_VERSION-pgtap
