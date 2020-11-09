#!/bin/bash

set -e

if [  "$1" = "--help" ] ; then
    echo "Usage: getSignatures.sh [DB_ARGS]"
    echo "  DB_ARGS optional"
    echo "example"
    echo "get_signatures.sql -U postgres -h localhost -p 5432 "
    echo "Exeute from the root of the repository"
    exit 0
fi

VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
MINOR=${VERSION%.*}

DB_NAME="____sigs_routing____"
DIR="sql/sigs"

# DB_ARGS are the remaining of the arguments
read -ra DB_ARGS <<< "$*"

FILE="$DIR/pgrouting--$MINOR.sig"

dropdb --if-exists "${DB_ARGS[@]}" "$DB_NAME"
createdb "${DB_ARGS[@]}" "$DB_NAME"

psql  "${DB_ARGS[@]}"  "$DB_NAME" <<EOF
SET client_min_messages = WARNING;
CREATE EXTENSION pgrouting WITH VERSION '$VERSION' CASCADE;
EOF

psql "${DB_ARGS[@]}" "$DB_NAME" -c '\dx+ pgrouting' -A | grep '^function' | cut -d ' ' -f2- | sort -d > "$FILE"
