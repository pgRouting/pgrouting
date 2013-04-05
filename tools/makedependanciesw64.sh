#!/bin/sh

if [[ "${OS_BUILD}" == "" && "$1" == "" ]] ; then
    echo "Usage: makedependencies OS_BUILD"
    echo "       OS_BUILD = 32|64"
    echo "       or export OS_BUILD=xx"
    exit 1
fi

if [[ "${OS_BUILD}" == "64" ]] ; then
	export MINGHOST=x86_64-w64-mingw32
    export PG_BITS=
else
	export MINGHOST=i686-w64-mingw32
    export PG_BITS=w32
#    alias cmake="/c/ming32/projects/cmake-2.8.10.2-win32-x86/bin/cmake"
fi

PROJECTS=/c/ming${OS_BUILD}/projects
export PATH="/c/ming${OS_BUILD}/projects/gettext/rel-gettext-0.18.1/bin:/c/ming${OS_BUILD}/projects/xsltproc:/c/ming${OS_BUILD}/projects/gtk/bin:/c/ming${OS_BUILD}/projects/rel-libiconv-1.13.1w64/include:.:/bin:/include:/mingw/bin:/mingw/include:/c/Windows/system32:/c/Windows:/usr/local/bin:/c/ming64/Silksvn/bin::/c/ming${OS_BUILD}/projects/pgx${OS_BUILD}/pg${PG_VER}${PG_BITS}/bin:/c/ming${OS_BUILD}/projects/pgx${OS_BUILD}/pg${PG_VER}${PG_BITS}/lib"

if false ; then
cd ${PROJECTS}
#wget -N http://sourceforge.net/projects/boost/files/boost/1.46.1/boost_1_46_1.zip
#unzip -oq boost_1_46_1.zip
if [[ "${OS_BUILD}" == "32" ]] ; then
#cat boost-w32.patch | patch -p1 boost_1_46_1/libs/thread/src/win32/tss_pe.cpp
echo
fi
cd boost_1_46_1/tools/build/v2
cmd //c bootstrap.bat gcc
cp bjam.exe ../../..
cd ../../..
bjam toolset=gcc address-model=${OS_BUILD} define=BOOST_USE_WINDOWS_H link=static threading=multi --with-thread --prefix=/usr/local install
fi


# Download CGAL-3.9.zip from http://www.cgal.org/download.html
# Put CGAL-3.9.zip in the PROJECTS directory

if true ; then
cd ${PROJECTS}
#  this site requires a login so you have to fetch the file interactively
#wget -N --no-check-certificate https://gforge.inria.fr/frs/download.php/2912
#unzip -oq CGAL-3.9.zip
cd CGAL-3.9
rm -rf build/mingw
mkdir -p build/mingw
cd build/mingw
cmake -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DCMAKE_CXX_FLAGS:STRING="-DBOOST_THREAD_USE_LIB -DBoost_USE_STATIC_LIBS -DBOOST_USE_WINDOWS_H" -DBoost_INCLUDE_DIR:PATH=/usr/local/include/boost-1_46_1 -DBOOST_LIBRARYDIR:PATH=/usr/local/lib ../../

## Fixup generated files
mv src/CGAL/all_files.cpp  aatmp
echo "#include <windows.h>" > src/CGAL/all_files.cpp
cat aatmp >> src/CGAL/all_files.cpp
rm aatmp
mv src/ImageIO/all_files.cpp  aatmp
echo "#include <windows.h>" > src/ImageIO/all_files.cpp
cat aatmp >> src/ImageIO/all_files.cpp
rm aatmp

make && make install
fi

if false ; then
cd ${PROJECTS}

#wget -N http://prdownloads.sourceforge.net/gaul/gaul-devel-0.1849-0.tar.bz2?download
#tar xjf gaul-devel-0.1849-0.tar.bz2
cd gaul-devel-0.1849-0
./configure --enable-slang=no --enable-pthread=no --enable-shared=yes
cat ../gaul.patch | patch -p1
touch /mingw/${MINGHOST}/include/values.h
make  && make install
fi


if false ; then
cd ${PROJECTS}
wget -N http://ftp.postgresql.org/pub/source/v9.2.3/postgresql-9.2.3.tar.bz2
tar xjf postgresql-9.2.3.tar.bz2
cd postgresql-9.2.2
./configure --prefix=/c/ming${OS_BUILD}/projects/pgx${OS_BUILD}/pg92 --build=${MINGHOST} --host=${MINGHOST} --target=x86_64-w64-mingw32 --with-pgport=5441 --enable-cassert --enable-debug --enable-integer-datetimes --disable-float8-byval --without-zlib build_alias=${MINGHOST} host_alias=${MINGHOST} target_alias=${MINGHOST}
make && make install
fi



