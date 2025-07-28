#!/bin/sh.exe
# /*PGR-GNU*****************************************************************
# File: build_pgrouting.sh
# Copyright (c) 2016 pgRouting developers
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
#-------------------------
# File used in Jenkis setup
#-------------------------

# Setting defaults
if  [[ "${OS_BUILD}" == '' ]] ; then
    OS_BUILD=64
fi;

if  [[ "${PG_VER}" == '' ]] ; then
    PG_VER=14
fi;

if  [[ "${PGPORT}" == '' ]] ; then
    PGPORT=54613
fi;

if  [[ "${PGROUTING_VER}" == '' ]] ; then
    PGROUTING_VER=cvvergara
fi;

if  [[ "${POSTGIS_VER}" == '' ]] ; then
    POSTGIS_VER=3.3.2
fi;

if  [[ "${GCC_TYPE}" == '' ]] ; then
    GCC_TYPE=gcc81
fi;

if  [[ "${BOOST_VER}" == '' ]] ; then
    BOOST_VER=1.78.0
fi;
BOOST_VER_WU=$(echo "${BOOST_VER//./_}")

echo "DEBUG ${DEBUG}"

# debugging options
if [[ "$DEBUG" == 'true' ]]
then
    JENKINS_DEBUG=1
    VERBOSE=ON
    BUILD_TYPE=Debug
else
    JENKINS_DEBUG=0
    VERBOSE=OFF
    BUILD_TYPE=Release
fi

TAPTEST=${TAPTEST%%*([[:blank:]])}
TAPTEST=${TAPTEST##*([[:blank:]])}

echo "GIT_COMMIT ${GIT_COMMIT}"

if [ $JENKINS_DEBUG -eq 1 ]
then
    echo
    echo "***************************"
    echo Received variables used in this script
    echo "**************************"

    echo "OS_BUILD ${OS_BUILD}"
    echo "PG_VER ${PG_VER}"
    echo "PGPORT ${PGPORT}"
    echo "PGROUTING_VER ${PGROUTING_VER}"
    echo "POSTGIS_VER ${POSTGIS_VER}"
    echo "GCC_TYPE ${GCC_TYPE}"
    echo "BOOST_VER ${BOOST_VER}"
    echo "calculated BOOST_VER_WU ${BOOST_VER_WU}"
    echo "TAPTEST ${TAPTEST}"
    echo "DEBUG ${DEBUG}"
fi

export PGUSER=postgres
export PROJECTS=/projects

#this is so winnie know's where to copy the dlls for vc++ edb compiled postgresql testing
export PGPATHEDB=${PROJECTS}/postgresql/rel/pg${PG_VER}w${OS_BUILD}${GCC_TYPE}edb

export PGPATH=${PROJECTS}/postgresql/rel/pg${PG_VER}w${OS_BUILD}${GCC_TYPE}
export PGWINVER=${PG_VER}w${OS_BUILD}${GCC_TYPE}edb
export BOOSTROOT_PATH="${PROJECTS}/boost/rel-${BOOST_VER_WU}w${OS_BUILD}${GCC_TYPE}"
export PATH="${PATH}:/usr/bin:${PGPATH}/bin:${PGPATH}/lib:${PGPATH}/include"
export PATH="${PROJECTS}/rel-libiconv-1.13.1w${OS_BUILD}${GCC_TYPE}/include:${PATH}"
export PATH="${PATH}:${BOOSTROOT_PATH}/lib"
export PATH="${PATH}:/cmake/bin"
export PATH="${PATH}:.:/bin:/include"


if [ $JENKINS_DEBUG -eq 1 ]
then
    echo "***************************"
    echo Paths
    echo "***************************"

    echo "PGUSER ${PGUSER}"
    echo "PROJECTS ${PROJECTS}"
    echo "PGPATHEDB ${PGPATHEDB}"
    echo "PGPATH ${PGPATH}"
    echo "PGWINVER ${PGWINVER}"
    echo "PATH ${PATH}"
    echo "BOOSTROOT_PATH ${BOOSTROOT_PATH}"
fi

#---------------
echo "Cleanup ${PGPATH} & ${PGPATHEDB}"
rm -f ${PGPATH}/lib/libpgrouting*
rm -f ${PGPATH}/share/extension/pgrouting*
rm -f ${PGPATHEDB}/lib/libpgrouting*
rm -f ${PGPATHEDB}/share/extension/pgrouting*

if [ $JENKINS_DEBUG -eq 1 ]
then
    echo "${PGPATH} & ${PGPATHEDB} pgrouting related files"
    ls ${PGPATH}/lib/libpgrouting* 2>/dev/null
    ls ${PGPATH}/share/extension/pgrouting* 2>/dev/null
    ls ${PGPATHEDB}/lib/libpgrouting* 2>/dev/null
    ls ${PGPATHEDB}/share/extension/pgrouting* 2>/dev/null
fi

cmake --version

cd "${PROJECTS}/pgrouting" || exit 1
DATABASE="___pgr___test___"
PGR_VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' "branches/${PGROUTING_VER}/CMakeLists.txt")
if [ $JENKINS_DEBUG -eq 1 ]
then
    echo "pgRouting VERSION ${PGR_VERSION}"
fi

rm -rf "build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}"
mkdir "build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}"
cd "build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}" || exit 1

cmake -G "MSYS Makefiles" -DCMAKE_VERBOSE_MAKEFILE="${VERBOSE}" \
 -DBOOST_ROOT:PATH="${BOOSTROOT_PATH}" \
 -DBoost_USE_STATIC_LIBS=ON \
 -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
 "../branches/${PGROUTING_VER}"

#---------------
echo
echo "***************************"
echo Building and installing
echo "***************************"
#---------------
make install

if [ $JENKINS_DEBUG -eq 1 ]
then
    echo "***************************"
    echo "Installation on PGPATH ${PGPATH}"
    echo "***************************"
    ls ${PGPATH}/lib/libpgrouting*
    ls ${PGPATH}/share/extension/pgrouting*

    echo "***************************"
    echo "Current contents of PGPATHEDB ${PGPATHEDB}"
    echo Should be empty
    echo "***************************"
    ls ${PGPATHEDB}/lib/libpgrouting*
    ls ${PGPATHEDB}/share/extension/pgrouting*
fi


#we need uninstall and reinstall copy to VC++ EDB instance if we want to test on standard Windows installed versions
#cp *.dll ${PGPATHEDB}/lib/  #TODO remove this once we fix so the .dlls are created in lib folder
cp -r ${PGPATH}/lib/libpgrouting*.dll ${PGPATHEDB}/lib/
cp -r ${PGPATH}/share/extension/pgrouting*.sql ${PGPATHEDB}/share/extension/
cp -r ${PGPATH}/share/extension/pgrouting.control ${PGPATHEDB}/share/extension/

if [ $JENKINS_DEBUG -eq 1 ]
then
    echo
    echo "***************************"
    echo "Installation on PGPATHEDB ${PGPATHEDB}"
    echo "***************************"
    ls ${PGPATHEDB}/lib/libpgrouting*
    ls ${PGPATHEDB}/share/extension/pgrouting*
fi

cd "${PROJECTS}/pgrouting/branches/${PGROUTING_VER}" || exit 1

echo "***************************"
# Testing
echo "***************************"

if [ -n "${TAPTEST}" ]
then

    psql -c "CREATE DATABASE ${DATABASE}"
    bash tools/testers/setup_db.sh "${PGPORT}" "${DATABASE}"  "${PGUSER}" "${PGR_VERSION}"
    pg_prove -v --normalize --directives --recurse -p "${PGPORT}" -d "${DATABASE}" "${TAPTEST}"
    psql -c "DROP DATABASE ${DATABASE}"

fi


psql -c "CREATE DATABASE ${DATABASE}"
tools/testers/pg_prove_tests.sh "${PGUSER}" "${PGPORT}"
psql -c "DROP DATABASE ${DATABASE}"


cd "${PROJECTS}/pgrouting/build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}/lib" || exit 1
strip ./*.dll
