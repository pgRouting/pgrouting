#!/bin/sh
# /*PGR-GNU*****************************************************************
# File: regress.sh
# Copyright (c) 2018 pgRouting developers
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
#--
# Jenkins variables passed in
#--
#
# export BRANCH=develop

#-------------------------
# File used in Jenkins setup
#-------------------------
export PATH="${PATH}":/usr/local::/usr/local/lib::/usr/local/bin
echo "$PATH"
export PGUSER=postgres
export PGROUTING_VER="${BRANCH}"

rm -rf "${WORKSPACE}/build${BRANCH}"
mkdir "${WORKSPACE}/build${BRANCH}"

cmake --version

cd "../build${BRANCH}" || exit 1
cmake "../${BRANCH}"

make
sudo make install
cd "../${BRANCH}" || exit 1
perl tools/testers/doc_queries_generator.pl

psql -c "CREATE DATABASE ___pgr___test___"
bash tools/testers/pg_prove_tests.sh "${PGUSER}"
psql -c "DROP DATABASE ___pgr___test___"
