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

PGDATABASE="___cti___test___"

dropdb "${PGFLAGS}" --if-exists "${PGDATABASE}"
createdb "${PGFLAGS}" "${PGDATABASE}"


cd tools/testers/
psql "$PGFLAGS"  -f setup_db.sql -d "${PGDATABASE}"
pg_prove --recurse --ext .sql "${PGFLAGS}"  -d "${PGDATABASE}" "../../pgtap/${DIR}"
dropdb "${PGFLAGS}"  "${PGDATABASE}"
