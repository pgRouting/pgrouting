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

DIR="$1"
shift
PGFLAGS=$*

PGDATABASE="___pgr___test___"

dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
createdb "${PGFLAGS}" "${PGDATABASE}"

echo "../../pgtap/${DIR}"

cd tools/testers/
psql "$PGFLAGS"  -f setup_db.sql -d "${PGDATABASE}"
pg_prove --recurse --ext .sql "${PGFLAGS}"  -d "${PGDATABASE}" "../../pgtap/${DIR}"
dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
