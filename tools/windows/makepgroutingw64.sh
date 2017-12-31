#!/bin/sh.exe

# CHANGE THESE TO SUIT YOUR PROJECT

export PROJECTS=/c/ming${OS_BUILD}/projects
export PG_VER=92


export PATHOLD=$PATH
echo $PATHOLD >c:/path.txt

export PATHOLD=".:/bin:/include:/mingw/bin:/mingw/include:/c/Windows/system32:/c/Windows:/usr/local/bin:/c/ming${OS_BUILD}/Silksvn/bin:"
export PGWINVER=${PG_VER}edb
export PATH="${PATHOLD}:${PROJECTS}/pgx${OS_BUILD}/pg${PG_VER}/bin:${PROJECTS}/pgx${OS_BUILD}/pg${PG_VER}/lib"
export PATH="${PROJECTS}/gettext/rel-gettext-0.18.1/bin:${PROJECTS}/xsltproc:${PROJECTS}/gtk/bin:${PROJECTS}/rel-libiconv-1.13.1w${OS_BUILD}/include:${PATH}"

echo $PATH
echo "" >> c:/path.txt
echo $PATH >> c:/path.txt

cd ${PROJECTS}
#git clone git@github.com:pgRouting/pgrouting.git

export PostgreSQL_ROOT=/c/ming${OS_BUILD}/projects/pgx${OS_BUILD}/pg${PG_VER}
cd pgrouting

#git checkout sew-devel-2_0
#git pull
rm -rf build
mkdir build
cd build
cmake -G "MSYS Makefiles" ..
make && make install

