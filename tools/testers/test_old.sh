#!/bin/bash

set -e

v="$1"
PGPORT=$2
PGUSER=$3
DIR=$(git rev-parse --show-toplevel)
pushd "${DIR}/tools/testers" > /dev/null || exit 1


PGDATABASE="___pgr___test___"
echo "--------------------------"
echo " Running with version ${v}"
echo "--------------------------"

dropdb --if-exists "${PGDATABASE}"
createdb "${PGDATABASE}"

bash setup_db.sh "${PGPORT}" "${PGDATABASE}" "${PGUSER}" "${v}"

pg_prove -q --normalize --directive --recurse --ext .sql -d "${PGDATABASE}" "../../pgtap"
