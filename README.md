# pgRouting - Routing on PostgreSQL

## STATUS

The *master* branch mostly reflects our current 1.05 release. We are working on a major feature release 2.0.0 in the *develop* branch. We are currently in *alpha* stage of development and things are very stable. The 2.0 release is not backwards compatible with the 1.x releases because we have totally restructured the API and the source code to position the product for additional future growth. Once you see all the new functionality that we have added and the simplier API design we thing you will be very happy with the results.

## LINKS

* http://pgrouting.org/ 
* http://docs.pgrouting.org/dev/doc/index.html
* https://github.com/pgRouting/pgrouting

## INTRODUCTION

pgRouting extends the PostGIS/PostgreSQL geospatial database to provide geospatial routing and other network analysis functionality.

This 2.0 library contains following features:

* All Pairs Shortest Path, JohnsonâAlgorithm (new 2.0)
* All Pairs Shortest Path, Floyd-Warshall Algorithm (new 2.0)
* Shortest Path A*
* Bi-directional Dijkstra Shortest Path (new 2.0)
* Bi-directional A* Shortest Path (new 2.0)
* Shortest Path Dijkstra
* Driving Distance
* K-Shortest Path, Multiple Alternative Paths (new 2.0)
* K-Dijkstra, One to Many Shortest Path (new 2.0)
* Traveling Sales Person (enhanced 2.0)
* Turn Restriction Shortest Path (TRSP) (new 2.0)


## REQUIREMENTS for 2.0

* C and C++ compilers
* Postgresql version >= 8.4 (9.1 or higher recommended)
* PostGIS version >= 1.5 (2.0 or higher recommended)
* The Boost Graph Library (BGL). Version >= [TBD]
* CMake >= 2.8.8
* (optional, for Driving Distance) CGAL >= [TBD]
* (optional, for Documentation) Sphinx >= [TBD]

## INSTALLATION

See online documentation: http://docs.pgrouting.org/dev/doc/src/installation/index.html

## COMPILATION

For MinGW on Windows

        mkdir build
        cd build
        cmake -G"MSYS Makefiles" -DWITH_DD=ON ..
        make
        make install

Also pre-built Windows binaries for 2.0 can be downloaded from:

    http://www.postgis.net/windows_downloads

For Linux

        mkdir build
        cd build
        cmake -DWITH_DD=ON ..
        make
        sudo make install

Build with documentation (requires [Sphinx](http://sphinx-doc.org/))

        cmake -DWITH_DOC=ON -DWITH_DD=ON ..

Postgresql 9.1+

        createdb mydatabase
        psql mydatabase -c "create extension postgis"
        psql mydatabase -c "create extension pgrouting"

For older versions of postgresql

        createdb -T template1 template_postgis
        psql template_postgis -c "create language plpgsql"
        psql template_postgis -f /usr/share/postgresql/9.0/contrib/postgis-1.5/postgis.sql
        psql template_postgis -f /usr/share/postgresql/9.0/contrib/postgis-1.5/spatial_ref_sys.sql
        psql template_postgis -f /usr/share/postgresql/9.0/contrib/postgis_comments.sql

        createdb -T template_postgis template_pgrouting
        psql template_pgrouting -f /usr/share/postgresql/9.0/contrib/pgrouting-2.0/pgrouting.sql

        createdb -T template_pgrouting mydatabase


## USAGE

See online documentation: http://docs.pgrouting.org/dev/doc/index.html


## LICENSE

* Most features are available under GPLv2.
* Some Boost extensions are available under Boost license (see LICENSE_1_0.txt)
* Some code contributed by iMaptools.com is available under MIT-X license.

