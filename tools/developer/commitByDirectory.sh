#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2021-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

DIR=$1
TASK=$2

if [ -z "${DIR}" ] ; then echo "USE: tools/developer/commitByDirectory.sh directory message"; exit 1; fi;
if [ -z "${TASK}" ] ; then echo "USE: tools/developer/commitByDirectory.sh directory message"; exit 1; fi;

for d in "${DIR}"/* ; do
    echo "--- PROCESSING $d ---"
    COMMIT_MSG="[${d}] ${TASK}"
    git add "${d}"
    git diff --name-only --cached
    sleep 3
    git diff --cached --quiet || echo "${COMMIT_MSG}"
    git diff --cached --quiet || git commit --no-status -m "${COMMIT_MSG}"
done

