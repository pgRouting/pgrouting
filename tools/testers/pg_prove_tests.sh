#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

set -e

PGUSER=$1
PGPORT=$2
if [ "b$PGPORT" = "b" ]
then
    PGPORT="-p 5432"
else
    PGPORT="-p $PGPORT"
fi
PGDATABASE="___pgr___test___"
echo "$PGPORT"


echo "cd ./tools/testers/"
cd ./tools/testers/
echo "psql -f setup_db.sql"
psql "$PGPORT" -U "$PGUSER"  -d "$PGDATABASE" -X -q -v ON_ERROR_STOP=1 --pset pager=off -f setup_db.sql

pg_prove --failures --quiet --recurse --ext .sql "$PGPORT" -d "$PGDATABASE"  -U "$PGUSER"  ../../pgtap/
