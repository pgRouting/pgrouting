#!/bin/sh

set -e

if [  "$#" -lt 3 ] ; then
    echo "Usage: getSignatures.sh VERSION DB_ARGS"
    echo "  VERSION like '2.4.0'"
    echo "  DB_NAME like 'routing'"
    echo "  DIR: 'curr-sig' OR 'sigs'"
    echo "  (optional) DB_ARGS like  -U postgres -h localhost -p 5432 "
    exit 0
fi

VERSION=$1
DB_NAME=$2
DIR=$3
shift
shift
shift
# DB_ARGS is the remaining of the arguments

FILE=tools/$DIR/pgrouting--$VERSION.sig
#FILE=test.sig

createdb $* $DB_NAME

psql  $*  $DB_NAME <<EOF
SET client_min_messages = WARNING;
drop extension if exists pgrouting;
drop extension if exists postgis;
create extension postgis;
create extension pgrouting with version '$VERSION';
EOF

echo "#VERSION pgrouting $VERSION" > $FILE
echo "#TYPES" >> $FILE
psql $* $DB_NAME -c '\dx+ pgrouting' -A | grep '^type' | cut -d ' ' -f2- | sort >> $FILE
echo "#FUNCTIONS" >> $FILE
psql $* $DB_NAME -c '\dx+ pgrouting' -A | grep '^function' | cut -d ' ' -f2- | sort >> $FILE

dropdb --if-exists $* $DB_NAME
