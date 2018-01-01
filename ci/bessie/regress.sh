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

cmake --version

cd ../build${PGROUTING_VER}
cmake ../${PGROUTING_VER}

make
sudo make install
cd ../${PGROUTING_VER}
perl tools/testers/algorithm-tester.pl

psql -c "CREATE DATABASE ___pgr___test___"
sh tools/testers/pg_prove_tests.sh ${PGUSER}
psql -c "DROP DATABASE ___pgr___test___"
