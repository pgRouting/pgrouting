# pgRouting - Routing on PostgreSQL

## STATUS

[![Build Status](https://travis-ci.org/pgRouting/pgrouting.png?branch=develop)](https://travis-ci.org/pgRouting/pgrouting)

## INTRODUCTION

pgRouting extends the PostGIS/PostgreSQL geospatial database to provide geospatial routing and other network analysis functionality.

This library contains following features:

* All Pairs Shortest Path, Johnson’s Algorithm
* All Pairs Shortest Path, Floyd-Warshall Algorithm
* Shortest Path A*
* Bi-directional Dijkstra Shortest Path
* Bi-directional A* Shortest Path
* Shortest Path Dijkstra
* Driving Distance
* K-Shortest Path
* Traveling Sales Person
* Turn Restriction Shortest Path (TRSP)


## REQUIREMENT

* C and C++ compilers
* Postgresql version >= [TBD]
* PostGIS version >= [TBD]
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
	psql template_postgis -f /usr/share/postgresql/8.3/contrib/postgis-1.5/postgis.sql
	psql template_postgis -f /usr/share/postgresql/8.3/contrib/postgis-1.5/spatial_ref_sys.sql
	psql template_postgis -f /usr/share/postgresql/8.3/contrib/postgis_comments.sql

	createdb -T template_postgis template_pgrouting
	psql template_pgrouting -f /usr/share/pgrouting/pgrouting--1.0.7.sql

	createdb -T template_pgrouting mydatabase


## USAGE

See online documentation: http://docs.pgrouting.org/dev/doc/index.html


## LICENSE

Most features are available under GPLv2.
Some Boost extensions are available under Boost license (see LICENSE_1_0.txt)
Some code contributed by iMaptools.com is available under MIT-X license.
