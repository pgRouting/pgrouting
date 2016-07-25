#!/bin/sh.exe

# CHANGE THESE TO SUIT YOUR PROJECT
#all these get passed in by jenkins
#export OS_BUILD=64 
#export PG_VER=9.2
#export PGHOST=localhost
#export PGPORT=8442
export PGUSER=postgres
#export PGROUTING_VER=2.1
#POSTGIS_VER=2.1.0SVN
#GCC_TYPE=
#export POSTIGS_VER=2.1.0beta3
#export GIT_COMMIT=whatever
export PROJECTS=/projects
export PGPATHEDB=${PROJECTS}/postgresql/rel/pg${PG_VER}w${OS_BUILD}${GCC_TYPE}edb  #this is so winnie know's where to copy the dlls for vc++ edb compiled postgresql testing
export PATHOLD=$PATH
export PGPATH=${PROJECTS}/postgresql/rel/pg${PG_VER}w${OS_BUILD}${GCC_TYPE}

#export PROJECTS=/c/ming${OS_BUILD}/projects

export PostgreSQL_ROOT=${PGPATH}

export PATHOLD="/mingw/bin:/mingw/include:/c/Windows/system32:/c/Windows"
export PGWINVER=${PG_VER}w${OS_BUILD}${GCC_TYPE}edb
export PATH="${PATHOLD}:${PGPATH}/bin:${PGPATH}/lib:${PGPATH}/include"
export PATH="${PROJECTS}/rel-libiconv-1.13.1w${OS_BUILD}${GCC_TYPE}/include:${PATH}"
GMP_VER=5.1.2
MPFR_VER=3.1.2
CGAL_VER=4.2
BOOST_VER=1.59.0
BOOST_VER_WU=1_59_0
BOOST_VER_WUM=1_59
ZLIB_VER=1.2.8
#export PATH="/mingw/bin:/mingw/include:/c/Windows/system32"
#zlib
PATH="${PATH}:${PROJECTS}/zlib/rel-${ZLIB_VER}w${OS_BUILD}${GCC_TYPE}/include:${PROJECTS}/zlib/rel-${ZLIB_VER}w${OS_BUILD}${GCC_TYPE}/lib:${PROJECTS}/zlib/rel-${ZLIB_VER}w${OS_BUILD}${GCC_TYPE}/bin"

#gmp
PATH="${PATH}:${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE}/include:${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE}/lib"

#mpfr
PATH="${PATH}:${PROJECTS}/CGAL/rel-mpfr-${MPFR_VER}w${OS_BUILD}${GCC_TYPE}/include:${PROJECTS}/CGAL/rel-mpfr-${MPFR_VER}w${OS_BUILD}${GCC_TYPE}/lib:${PROJECTS}/CGAL/rel-mpfr-${MPFR_VER}w${OS_BUILD}${GCC_TYPE}/bin"

#boost
PATH="${PATH}:${PROJECTS}/boost/rel-${BOOST_VER_WU}w${OS_BUILD}${GCC_TYPE}/lib"
#BOOSTROOT_PATH
BOOSTROOT_PATH="${PROJECTS}/boost/rel-${BOOST_VER_WU}w${OS_BUILD}${GCC_TYPE}"

#cgal
PATH="${PATH}:${PROJECTS}/CGAL/rel-cgal-${CGAL_VER}w${OS_BUILD}${GCC_TYPE}/include:${PROJECTS}/CGAL/rel-cgal-${CGAL_VER}w${OS_BUILD}${GCC_TYPE}/lib"

#cmake
export PATH="${PATH}:/cmake/bin"
export PATH="${PATH}:.:/bin:/include"

cd ${PROJECTS}/pgrouting
rm -rf build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
mkdir build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
cd build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
cmake -G "MSYS Makefiles"  -DCMAKE_VERBOSE_MAKEFILE=ON -DBOOST_ROOT:PATH=${BOOSTROOT_PATH} -DCGAL_ROOT:PATH=${PROJECTS}/CGAL/rel-cgal-${CGAL_VER}w${OS_BUILD}${GCC_TYPE} -DGMP_ROOT:PATH=${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE} -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_MULTITHREADED=ON -DCMAKE_CXX_FLAGS="-I${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE}/include -I${PROJECTS}/CGAL/rel-mpfr-${MPFR_VER}w${OS_BUILD}${GCC_TYPE}/include"  ../branches/${PGROUTING_VER}

#first delete old pgrouting files from installed folder before we reinstall


echo "The git commit is ${GIT_COMMIT}"
rm ${PGPATH}/lib/librouting*
rm ${PGPATH}/share/extension/pgrouting*
make && make install

#we need uninstall and reinstall copy to VC++ EDB instance if we want to test on standard Windows installed versions
rm ${PGPATHEDB}/lib/librouting*
cp lib/*.dll ${PGPATHEDB}/lib/
rm ${PGPATHEDB}/share/extension/pgrouting*
cp lib/*.sql ${PGPATHEDB}/share/extension/
cp lib/*.control ${PGPATHEDB}/share/extension/

cd ${PROJECTS}/pgrouting/branches/${PGROUTING_VER}

#perl tools/test-runner.pl   -pgver ${PG_VER} -pgport "${PGPORT}"  -clean
echo "PGVER ${PG_VER}"
echo "POSTGIS_VER ${POSTGIS_VER}"
echo "PGPORT ${PGPORT}"
perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"  -alg common
perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"  -alg dijkstra
perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"  -alg contraction
perl tools/testers/algorithm-tester.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"


cd ${PROJECTS}/pgrouting/build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}/lib
strip *.dll
