#!/bin/bash

set -e

if [[ -z  $1 ]]; then
    echo "Usage:"
    echo "taptest.sh <pgtap directory> [<postgresql options>]";
    echo "Examples:"
    echo "taptest.sh withPoints/undirected_equalityDD.sql -p 5432"
    exit 1;
fi


DIR="$1"
shift
PGFLAGS=$*

#QUIET="-v"
QUIET="-q"

PGDATABASE="___pgr___test___"
#PGRVERSION="3.2.1 3.2.0 3.1.3 3.1.2 3.1.1 3.1.0 3.0.5 3.0.4 3.0.3 3.0.2 3.0.1 3.0.0"
PGRVERSION="3.2.1"

cd tools/testers/

for v in ${PGRVERSION}
do
    echo "--------------------------"
    echo " Running with version ${v}"
    echo "--------------------------"

    dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
    createdb "${PGFLAGS}" "${PGDATABASE}"
    psql -c "CREATE EXTENSION pgRouting WITH VERSION '${v}' CASCADE" -d "${PGDATABASE}"

    echo "../../pgtap/${DIR}"

    psql "$PGFLAGS"  -f setup_db.sql -d "${PGDATABASE}"
    pg_prove "$QUIET" --normalize --directive --recurse --ext .sql "${PGFLAGS}"  -d "${PGDATABASE}" "../../pgtap/${DIR}"
    dropdb --if-exists "${PGFLAGS}" "${PGDATABASE}"
done
