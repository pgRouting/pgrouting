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