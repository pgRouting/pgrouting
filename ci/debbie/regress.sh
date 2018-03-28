#!/bin/bash
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
export GEOS_VER=3.7.0dev
export GDAL_VER=2.2
export PROJECTS=${JENKINS_HOME}/workspace
export PGPATH=${PROJECTS}/pg/rel/pg${PG_VER}w${OS_BUILD}
export PGDATA=${PGPATH}/data_${PGPORT}
export PGDATABASE=postgres
export PGUSER=postgres
export LD_LIBRARY_PATH="${PROJECTS}/gdal/rel-${GDAL_VER}w${OS_BUILD}/lib:${PROJECTS}/geos/rel-${GEOS_VER}w${OS_BUILD}/lib:${PGPATH}/lib"
export PATH="${PATH}:${PGPATH}/bin:${PGPATH}/lib:${PGPATH}/include"
export POSTGIS_VER=2.5.0dev

#---
# start the pg sever
# --
export PGLOCALEDIR=${PGPATH}/share/locale

DAEMON=${PGPATH}/bin/postmaster
PGSTARTLOG=${PGDATA}/start_log.log


#initialize the database
${PGPATH}/bin/initdb -U postgres -D ${PGDATA} -A trust

# check to see if pg is already running
state=`${PGPATH}/bin/pg_ctl status -D ${PGDATA} -l ${PGDATA}/logfile | grep "server is running" `

# start pg if not running
if [ "0" == "0$state" ]; then
  ${PGPATH}/bin/pg_ctl start -D ${PGDATA} -l ${PGDATA}/logfile >> ${PGSTARTLOG} 2>&1
fi
echo "ok"

#--
# end start pg server
#

#-------------------------
# File used in Jenkins setup
#-------------------------

echo $PATH
export PGUSER=postgres
export PGROUTING_VER=$BRANCH

rm -rf ${WORKSPACE}/build${BRANCH}
mkdir ${WORKSPACE}/build${BRANCH}

cmake --version

cd ../build${BRANCH}
cmake ../${BRANCH}

make
make install
cd ../${BRANCH}
export PERL5LIB=$(echo pwd)
perl tools/testers/algorithm-tester.pl -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"

#pgTap tests disable for now until we have installed
if false; then
psql -c "CREATE DATABASE ___pgr___test___"
sh tools/testers/pg_prove_tests.sh ${PGUSER}
psql -c "DROP DATABASE ___pgr___test___"
fi

#stop the postgres server
state=`${PGPATH}/bin/pg_ctl status -D ${PGDATA} -l ${PGPATH}/data/logfile | grep "server is running"`
echo $state

if [ "0" != "0$state" ]; then
  ${PGPATH}/bin/pg_ctl stop -D ${PGDATA} -l logfile -m fast
  rm -rf ${PGDATA}
fi
echo "done"


