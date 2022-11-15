#!/bin/bash
## /*PGR-GNU*****************************************************************
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
export PGROUTING_VER=$BRANCH
export OS_BUILD=64
export PG_VER=11
export PGSQL_VERSION=110
export PGPORT=5411
export GEOS_VER=3.8
export GDAL_VER=2.3
export PROJECTS=${JENKINS_HOME}/workspace
export PGPATH=${PROJECTS}/pg/rel/pg${PG_VER}w${OS_BUILD}
export PGDATA=${PGPATH}/data_${PGPORT}
export PGDATABASE=postgres
export PGUSER=postgres
export LD_LIBRARY_PATH="${PROJECTS}/gdal/rel-${GDAL_VER}w${OS_BUILD}/lib:${PROJECTS}/geos/rel-${GEOS_VER}w${OS_BUILD}/lib:${PGPATH}/lib"
export PATH="${PATH}:${PGPATH}/bin:${PGPATH}/lib:${PGPATH}/include"
export POSTGIS_VER=3.1.0dev

#---
# start the pg sever
# --
export PGLOCALEDIR=${PGPATH}/share/locale

DAEMON=${PGPATH}/bin/postmaster
PGSTARTLOG=${PGDATA}/start_log.log
echo "${DAEMON}"


#initialize the database
"${PGPATH}"/bin/initdb -U postgres -D "${PGDATA}" -A trust

# check to see if pg is already running
state=$("${PGPATH}"/bin/pg_ctl status -D "${PGDATA}" -l "${PGDATA}"/logfile | grep "server is running")

# start pg if not running
if [ "0" == "0$state" ]; then
  "${PGPATH}/bin/pg_ctl" start -D "${PGDATA}" -l "${PGDATA}/logfile" >> "${PGSTARTLOG}" 2>&1
fi
echo "ok"

#--
# end start pg server
#

#-------------------------
# File used in Jenkins setup
#-------------------------

echo "$PATH"
export PGUSER=postgres
export PGROUTING_VER="$BRANCH"

rm -rf "${WORKSPACE}/build${BRANCH}"
mkdir "${WORKSPACE}/build${BRANCH}"

cmake --version

cd ../build"${BRANCH}" || exit 1
cmake "../${BRANCH}"

make
make install
cd ../"${BRANCH}" || exit 1
PERL5LIB=$(pwd)
export PERL5LIB
perl tools/testers/doc_queries_generator.pl -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"

#pgTap tests disable for now until we have installed
if false; then
psql -c "CREATE DATABASE ___pgr___test___"
tools/testers/pg_prove_tests.sh "${PGUSER}"
psql -c "DROP DATABASE ___pgr___test___"
fi

#stop the postgres server
state=$("${PGPATH}"/bin/pg_ctl status -D "${PGDATA}" -l "${PGPATH}"/data/logfile | grep "server is running")
echo "${state}"

if [ "0" != "0$state" ]; then
  "${PGPATH}"/bin/pg_ctl stop -D "${PGDATA}" -l logfile -m fast
  rm -rf "${PGDATA}"
fi
echo "done"


