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
    PGPORT="5432"
fi
PGDATABASE="___pgr___test___"
echo "$PGPORT"


pushd ./tools/testers/ || exit 1

bash setup_db.sh "${PGPORT}" "${PGDATABASE}" "${PGUSER}" "3.3.3"

pg_prove --failures --quiet --recurse --ext .sql \
    --S client_min_messages=WARNING \
    --S on_error_rollback=off \
    --S on_error_stop=true \
    --P format=unaligned \
    --P tuples_only=true \
    --P pager=off \
    -p "$PGPORT" -d "$PGDATABASE"  -U "$PGUSER"  ../../pgtap/

popd
