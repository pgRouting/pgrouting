#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: commitByDirectory.sh
# Copyright (c) 2021 pgRouting developers
# Mail: project@pgrouting.org
# ------
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ********************************************************************PGR-GNU*/
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

