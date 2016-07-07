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

echo "Postgres $POSTGRESQL_VERSION"
echo "User $PGUSER"

if test "$POSTGRESQL_VERSION" = "9.6" ; then

    echo "Installing postgresql 9.6 & postgis for 9.6 pgtap & pg_prove"
    sudo apt-get install -y postgresql-9.6 postgresql-9.6-postgis
    #sudo apt-get install -y pgtap libtap-parser-sourcehandler-pgtap-perl
    sudo cp /usr/lib/postgresql/$POSTGRESQL_VERSION/bin/pg_config /usr/bin/pg_config
    sudo /etc/init.d/postgresql stop
    sudo /etc/init.d/postgresql stop
    ps -fea | grep postgres
    echo "making grep before change"
    grep port /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
    echo "finished grep"
    sudo sed -i -e 's/port = 5433/port = 5432/g' /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
    echo "making grep after change"
    grep port /etc/postgresql/$POSTGRESQL_VERSION/main/postgresql.conf
    echo "finished grep"
    sudo cp $TRAVIS_BUILD_DIR/tools/travis/pg_hba.conf /etc/postgresql/$POSTGRESQL_VERSION/main/pg_hba.conf
    sudo /etc/init.d/postgresql start $POSTGRESQL_VERSION
    #sudo service postgres-$POSTGRESQL_VERSION start
    ps -fea | grep postgres
    #sudo -u $DBUSER psql -c "ALTER ROLE postgres WITH PASSWORD '';"

#else

#echo "starting server"
#sudo /etc/init.d/postgresql start $POSTGRESQL_VERSION


#sudo cp /usr/lib/postgresql/$POSTGRESQL_VERSION/bin/pg_config /usr/bin/pg_config

#echo "Installing pgtap ... this may take some time."
#wget https://github.com/theory/pgtap/archive/master.zip
#unzip master.zip
#cd pgtap-master
#make
#make installcheck
#sudo make install
#cd ..

#sudo apt-get install -y libtap-parser-sourcehandler-pgtap-perl

fi
