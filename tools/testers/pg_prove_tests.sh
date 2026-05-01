#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2016-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

# Travis CI scripts
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

bash setup_db.sh "${PGPORT}" "${PGDATABASE}" "${PGUSER}" "4.1.0"

PGOPTIONS="-c client_min_messages=WARNING" pg_prove --failures --q --recurse \
    -S on_error_rollback=off \
    -S on_error_stop=true \
    -P format=unaligned \
    -P tuples_only=true \
    -P pager=off \
    -p "$PGPORT" -d "$PGDATABASE"  -U "$PGUSER"  ../../pgtap/

popd
