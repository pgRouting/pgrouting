#!/bin/sh

if [  "$#" -eq 0 ] ; then
    echo "Usage: getSignatures.sh VERSION DB_ARGS"
    echo "  VERSION like '2.4.0'"
    echo "  DB_ARGS like -U postgres -h localhost -p 5432 dbname"
    exit 0
fi

VERSION=$1
FILE=tools/curr-sig/pgrouting--$VERSION.sig
#FILE=test.sig
shift

psql  $*  <<EOF
drop extension if exists pgrouting;
drop extension if exists postgis;
create extension postgis;
create extension pgrouting with version '$VERSION';
EOF

echo "#VERSION pgrouting $VERSION" > $FILE
echo "#TYPES" >> $FILE
psql $* -c '\dx+ pgrouting' -A | grep '^type' | cut -d ' ' -f2- | sort >> $FILE
echo "#FUNCTIONS" >> $FILE
psql $* -c '\dx+ pgrouting' -A | grep '^function' | cut -d ' ' -f2- | sort >> $FILE
