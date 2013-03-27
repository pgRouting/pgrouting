#!/bin/sh
PROJECTS=/c/ming64/projects

export PATH='/c/ming64/projects/gettext/rel-gettext-0.18.1/bin:/c/ming64/projects/xsltproc:/c/ming64/projects/gtk/bin:/c/ming64/projects/rel-libiconv-1.13.1w64/include:.:/bin:/include:/mingw/bin:/mingw/include:/c/Windows/system32:/c/Windows:/usr/local/bin:/c/ming64/Silksvn/bin::/c/ming64/projects/pgx64/pg92/bin:/c/ming64/projects/pgx64/pg92/lib'

cd ${PROJECTS}
wget http://sourceforge.net/projects/boost/files/boost/1.46.1/boost_1_46_1.zip
unzip boost_1_46_1.zip
cd boost_1_46_1/tools/build/v2
cmd //c bootstrap.bat gcc
cp bjam.exe ../../..
cd ../../..
bjam toolset=gcc address-model=64 define=BOOST_USE_WINDOWS_H link=static threading=multi --with-thread --prefix=/usr/local install


# Download CGAL-3.9.zip from http://www.cgal.org/download.html
# Put CGAL-3.9.zip in the PROJECTS directory

cd ${PROJECTS}
wget --no-check-certificate https://gforge.inria.fr/frs/download.php/2912
unzip CGAL-3.9.zip
cd CGAL-3.9
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

make &&  make install


cd ${PROJECTS}
wget http://prdownloads.sourceforge.net/gaul/gaul-devel-0.1849-0.tar.bz2?download
tar xj gaul-devel-0.1849-0.tar.bz2
cd gaul-devel-0.1849-0
# Reginia's cmake
#./configure --host=x86_64-w64-mingw32 --target=x86_64-w64-mingw32 --enable-slang=no --enable-pthread=no --enable-shared=yes
./configure --enable-slang=no --enable-pthread=no --enable-shared=yes
cat ../gaul.patch | patch -p1
touch /mingw/x86_64-w64-mingw32/include/values.h
make && make install


#cd ${PROJECTS}
#wget http://ftp.postgresql.org/pub/source/v9.2.2/postgresql-9.2.2.tar.bz2
#tar xjf postgresql-9.2.2.tar.bz2
#cd postgresql-9.2.2
#./configure --prefix=/c/ming64/projects/pgx64/pg92 --build=x86_64-w64-mingw32 --host=x86_64-w64-mingw32 --target=x86_64-w64-mingw32 --with-pgport=5441 --enable-cassert --enable-debug --enable-integer-datetimes --disable-float8-byval --without-zlib build_alias=x86_64-w64-mingw32 host_alias=x86_64-w64-mingw32 target_alias=x86_64-w64-mingw32
#make
#make install


cd ${PROJECTS}
wget http://ftp.postgresql.org/pub/source/v9.2.3/postgresql-9.2.3.tar.bz2
tar xjf postgresql-9.2.3.tar.bz2
cd postgresql-9.2.3
./configure --prefix=/c/ming64/projects/pgx64/pg923 --build=x86_64-w64-mingw32 --host=x86_64-w64-mingw32 --target=x86_64-w64-mingw32 --with-pgport=5441 --enable-cassert --enable-debug --enable-integer-datetimes --disable-float8-byval --without-zlib build_alias=x86_64-w64-mingw32 host_alias=x86_64-w64-mingw32 target_alias=x86_64-w64-mingw32
make && make install


