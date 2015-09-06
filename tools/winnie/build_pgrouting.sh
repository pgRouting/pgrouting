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
if [[ "${GCC_TYPE}" == *gcc48* ]] ; then
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
	#cgal
	PATH="${PATH}:${PROJECTS}/CGAL/rel-cgal-${CGAL_VER}w${OS_BUILD}${GCC_TYPE}/include:${PROJECTS}/CGAL/rel-cgal-${CGAL_VER}w${OS_BUILD}${GCC_TYPE}/lib"
	#cmake
	export PATH="${PATH}:/cmake/bin"
	export PATH="${PATH}:.:/bin:/include"
	cd ${PROJECTS}/pgrouting
	rm -rf build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
	mkdir build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
	cd build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
	cmake -G "MSYS Makefiles"  -DCMAKE_VERBOSE_MAKEFILE=ON -DBOOST_ROOT:PATH=${PROJECTS}/boost/rel-${BOOST_VER_WU}w${OS_BUILD}${GCC_TYPE} -DCGAL_ROOT:PATH=${PROJECTS}/CGAL/rel-cgal-${CGAL_VER}w${OS_BUILD}${GCC_TYPE} -DGMP_ROOT:PATH=${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE} -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_MULTITHREADED=ON -DCMAKE_CXX_FLAGS="-I${PROJECTS}/CGAL/rel-gmp-${GMP_VER}w${OS_BUILD}${GCC_TYPE}/include -I${PROJECTS}/CGAL/rel-mpfr-${MPFR_VER}w${OS_BUILD}${GCC_TYPE}/include"  ../branches/${PGROUTING_VER}
else 
	#alias cmake="/c/ming${OS_BUILD}/cmake-2.8.10.2-win32-x86/bin/cmake"
	export PostgreSQL_ROOT=${PGPATH}
	export PATHOLD=$PATH
	
	#export PATHOLD=".:/bin:/include:/mingw/bin:/mingw/include:/c/Windows/system32:/c/Windows:/usr/local/bin:/c/ming64/Silksvn/bin:/c/Program Files (x86)/Git/bin"
	
	export PATHOLD=".:/bin:/include:/mingw/bin:/mingw/include:/c/Windows/system32:/c/Windows:/usr/local/bin"
	export PGWINVER=${PG_VER}edb

	export PATH="${PATHOLD}:${PostgreSQL_ROOT}/bin:${PostgreSQL_ROOT}/lib"
	#export PATH="${PROJECTS}/gettext/rel-gettext-0.18.1/bin:${PROJECTS}/xsltproc:${PROJECTS}/gtk/bin:${PROJECTS}/rel-libiconv-1.13.1w64/include:${PATH}"
	export PATH="${PATH}:/cmake/bin"
	echo $PATH
	cd ${PROJECTS}/pgrouting
	rm -rf build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
	mkdir build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
	cd build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}
	cmake -G "MSYS Makefiles"  ../branches/${PGROUTING_VER}
fi
#cmake -G "MSYS Makefiles" -DWITH_DD=ON ..
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
perl tools/test-runner.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}" -ignorenotice -clean
#perl tools/test-runner.pl  -pgver ${PG_VER} -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}" 
#perl tools/test-runner.pl  -pgver "${PG_VER}" -pgisver "${POSTGIS_VER}" -pgport "${PGPORT}"  -clean -v -alg ksp


cd ${PROJECTS}/pgrouting/build${PGROUTING_VER}w${OS_BUILD}${GCC_TYPE}/lib
strip *.dll
