#!/bin/bash

set -e

if [[ -z  $1 ]]; then
    echo "Directory missing";
    echo "Usage"
    echo "tools/developer/taptest.sh <pgtap directory> [<postgresql options>]";
    echo "Examples:"
    echo "tools/developer/taptest.sh withPoints/undirected_equalityDD.sql -p 5433"
    exit 1;
fi

DIR=$1
shift
PGFLAGS=$*

PGDATABASE="___pgr___test___"

dropdb $PGFLAGS --if-exists ___pgr___test___
createdb  $PGFLAGS  ___pgr___test___


cd tools/testers/
psql $PGFLAGS  -f setup_db.sql -d ___pgr___test___
pg_prove $PGFLAGS  -d ___pgr___test___ ../../pgtap/$DIR
dropdb $PGFLAGS  ___pgr___test___
