#!/bin/sh
#--
# Jenkins variables passed in
#--
#
# export BRANCH=develop
export PGROUTING_VER=$BRANCH

#-------------------------
# File used in Jenkins setup
#-------------------------
export PATH=${PATH}:/usr/local::/usr/local/lib::/usr/local/bin
echo $PATH
export PGUSER=postgres
export PGROUTING_VER=$BRANCH

rm -rf ${WORKSPACE}/build${BRANCH}
mkdir ${WORKSPACE}/build${BRANCH}

cmake --version

cd ../build${BRANCH}
cmake ../${BRANCH}

make
sudo make install
cd ../${BRANCH}
perl tools/testers/algorithm-tester.pl

psql -c "CREATE DATABASE ___pgr___test___"
sh tools/testers/pg_prove_tests.sh ${PGUSER}
psql -c "DROP DATABASE ___pgr___test___"
