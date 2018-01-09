#!/bin/sh.exe

#-------------------------
# File used in Jenkis setup
#-------------------------

JENKINS_DEBUG=1

#-----------------
# variables setup
#  Variables recived by jenkins setup
#-----------------
#export OS_BUILD=
#export PG_VER=
#export PGHOST=
#export PGPORT=
#export PGROUTING_VER=
#export POSTIGS_VER=
#GCC_TYPE=
#export GIT_COMMIT=


if [ $JENKINS_DEBUG -eq 1 ]
then
    #---------------
    echo
    echo "***************************"
    echo Recived variables
    echo "**************************"
    #---------------
    echo "OS_BUILD ${OS_BUILD}"
    echo "PG_VER ${PG_VER}"
    echo "PGHOST ${PGHOST}"
    echo "PGPORT ${PGPORT}"
    echo "PGROUTING_VER ${PGROUTING_VER}"
    echo "POSTGIS_VER ${POSTGIS_VER}"
    echo "GCC_TYPE ${GCC_TYPE}"
    echo "GIT_COMMIT ${GIT_COMMIT}"
fi

#---------------
echo
echo "***************************"
echo Deduced variables
echo "***************************"
#---------------

export PGUSER=postgres
export PROJECTS=/projects
export PGPATHEDB=${PROJECTS}/postgresql/rel/pg${PG_VER}w${OS_BUILD}${GCC_TYPE}edb  #this is so winnie know's where to copy the dlls for vc++ edb compiled postgresql testing
export PGPATH=${PROJECTS}/postgresql/rel/pg${PG_VER}w${OS_BUILD}${GCC_TYPE}
export PATHOLD=$PATH
#export PATHOLD="/mingw/bin:/mingw/include:/c/Windows/system32:/c/Windows"
export PGWINVER=${PG_VER}w${OS_BUILD}${GCC_TYPE}edb
export PATH="${PATHOLD}:/usr/bin:${PGPATH}/bin:${PGPATH}/lib:${PGPATH}/include"
export PATH="${PROJECTS}/rel-libiconv-1.13.1w${OS_BUILD}${GCC_TYPE}/include:${PATH}"

if [ $JENKINS_DEBUG -eq 1 ]
then
    echo "PGUSER ${PGUSER}"
    echo "PROJECTS ${PROJECTS}"
    echo "PGPATHEDB ${PGPATHEDB}"
    echo "PGPATH ${PGPATH}"
    echo "PATHOLD ${PATHOLD}"
    echo "PGWINVER ${PGWINVER}"
    echo "PATH ${PATH}"
fi

GMP_VER=5.1.2
MPFR_VER=3.1.2
CGAL_VER=4.2
BOOST_VER=1.59.0
BOOST_VER_WU=1_59_0
BOOST_VER_WUM=1_59
ZLIB_VER=1.2.8

if [ $JENKINS_DEBUG -eq 1]
then
    echo "GMP_VER ${GMP_VER}"
    echo "MPFR_VER ${MPFR_VER}"
    echo "GAL_VER ${CGAL_VER}"
    echo "BOOST_VER_WU ${BOOST_VER_WU}"
    echo "BOOST_VER_WUM ${BOOST_VER_WUM}"
    echo "ZLIB_VER ${ZLIB_VER}"
fi

#zlib
ZLIB_PATH="${PROJECTS}/zlib/rel-${ZLIB_VER}w${OS_BUILD}${GCC_TYPE}"
PATH="${PATH}:${ZLIB_PATH}/include:${ZLIB_PATH}/lib:${ZLIB_PATH}/bin"

#gmp
GMP_PATH="${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE}"
PATH="${PATH}:${GMP_PATH}/include:${GMP_PATH}/lib"

#mpfr
MPFR_PATH="${PROJECTS}/CGAL/rel-mpfr-${MPFR_VER}w${OS_BUILD}${GCC_TYPE}"
PATH="${PATH}:${MPFR_PATH}/include:${MPFR_PATH}/lib:${MPFR_PATH}/bin"

#boost
BOOSTROOT_PATH="${PROJECTS}/boost/rel-${BOOST_VER_WU}w${OS_BUILD}${GCC_TYPE}"
PATH="${PATH}:${BOOSTROOT_PATH}/lib"

#cgal
CGAL_PATH="${PROJECTS}/CGAL/rel-cgal-${CGAL_VER}w${OS_BUILD}${GCC_TYPE}"
PATH="${PATH}:${CGAL_PATH}/include:${CGAL_PATH}/lib"

#cmake
export PATH="${PATH}:/cmake/bin"
export PATH="${PATH}:.:/bin:/include"

cmake --version

echo "PATH ${PATH}"

cd ${PROJECTS}/pgrouting
rm -rf build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
mkdir build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
cd build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}


#---------------
echo
echo "***************************"
echo "Current contents of PGPATH ${PGPATH}"
echo "***************************"
#---------------
ls ${PGPATH}/lib/libpgrouting*
ls ${PGPATH}/share/extension/pgrouting*

#---------------
echo
echo "***************************"
echo "Current contents of PGPATHEDB ${PGPATHEDB}"
echo "***************************"
#---------------
ls ${PGPATHEDB}/lib/libpgrouting*
ls ${PGPATHEDB}/share/extension/pgrouting*

rm ${PGPATH}/lib/libpgrouting*
rm ${PGPATH}/share/extension/pgrouting*
rm ${PGPATHEDB}/lib/libpgrouting*
rm ${PGPATHEDB}/share/extension/pgrouting*

#---------------
echo
echo "***************************"
echo "After removing in PGPATH ${PGPATH}"
echo "***************************"
#---------------
ls ${PGPATH}/lib/libpgrouting*
ls ${PGPATH}/share/extension/pgrouting*

#---------------
echo
echo "***************************"
echo "After removing in PGPATHEDB ${PGPATHEDB}"
echo "***************************"
#---------------
ls ${PGPATHEDB}/lib/libpgrouting*
ls ${PGPATHEDB}/share/extension/pgrouting*
cmake --version

cmake -G "MSYS Makefiles" -DCMAKE_VERBOSE_MAKEFILE=ON \
 -DBOOST_ROOT:PATH=${BOOSTROOT_PATH} \
 -DCGAL_ROOT:PATH=${CGAL_PATH} \
 -DGMP_ROOT:PATH=${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE} \
 -DBoost_USE_STATIC_LIBS=ON \
 -DBoost_USE_MULTITHREADED=ON \
 -DCMAKE_CXX_FLAGS="-I${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE}/include \
 -I${PROJECTS}/CGAL/rel-mpfr-${MPFR_VER}w${OS_BUILD}${GCC_TYPE}/include" \
 -DCMAKE_BUILD_TYPE=Release \
 ../branches/${PGROUTING_VER}

#---------------
echo
echo "***************************"
echo make
echo "***************************"
#---------------
make

#---------------
echo
echo "***************************"
echo make install
echo "***************************"
#---------------
make install

#---------------
echo
echo "***************************"
echo "Current contents of PGPATH ${PGPATH}"
echo "***************************"
#---------------
ls ${PGPATH}/lib/libpgrouting*
ls ${PGPATH}/share/extension/pgrouting*

#---------------
echo
echo "***************************"
echo "Current contents of PGPATHEDB ${PGPATHEDB}"
echo Should be empty
echo "***************************"
#---------------
ls ${PGPATHEDB}/lib/libpgrouting*
ls ${PGPATHEDB}/share/extension/pgrouting*


#we need uninstall and reinstall copy to VC++ EDB instance if we want to test on standard Windows installed versions
#cp *.dll ${PGPATHEDB}/lib/  #TODO remove this once we fix so the .dlls are created in lib folder
cp ${PGPATH}/lib/*.dll ${PGPATHEDB}/lib/
cp ${PGPATH}/share/extension/pgrouting*.sql ${PGPATHEDB}/share/extension/
cp ${PGPATH}/share/extension/*.control ${PGPATHEDB}/share/extension/

#---------------
echo
echo "***************************"
echo "After copying to PGPATHEDB ${PGPATHEDB}"
echo "***************************"
#---------------
ls ${PGPATHEDB}/lib/libpgrouting*
ls ${PGPATHEDB}/share/extension/pgrouting*

cd ${PROJECTS}/pgrouting/branches/${PGROUTING_VER}

#perl tools/test-runner.pl   -pgver ${PG_VER} -pgport "${PGPORT}"  -clean
#perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"  -alg common -clean -v
#perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"  -alg dijkstra -clean -v
#perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"  -alg contraction

#perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"

if [ $OS_BUILD -eq 32 ]
then

    perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"

else

    psql -c "CREATE DATABASE ___pgr___test___"
    sh tools/testers/pg_prove_tests.sh ${PGUSER}
    psql -c "DROP DATABASE ___pgr___test___"

fi

cd ${PROJECTS}/pgrouting/build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}/lib
strip *.dll
