# pgRouting - Routing on PostgreSQL

[![Join the chat at
https://gitter.im/pgRouting/pgrouting](https://badges.gitter.im/Join%20Chat.svg)](https://app.gitter.im/#/room/#pgrouting:osgeo.org)
[Join discourse](https://discourse.osgeo.org/c/pgrouting/15)


## Branches

* The *main* branch has the development of the next micro release
* The *develop* branch has the development of the next minor/major release

For the complete list of releases go to:
https://github.com/pgRouting/pgrouting/releases

For the release notes go to: https://docs.pgrouting.org/latest/en/release_notes.html


## LINKS

* https://pgrouting.org/
* https://docs.pgrouting.org/
* https://github.com/pgRouting/pgrouting

## STATUS

Status of the project can be found [here](https://github.com/pgRouting/pgrouting/wiki#status)

## INTRODUCTION

pgRouting extends the PostGIS/PostgreSQL geospatial database to provide geospatial routing and other network analysis functionality.

This library contains the following features:

* All Pairs Shortest Path Algorithms
* A-star algorithm
* Bi-directional algorithms
* A variety of applications of Dijkstra algorithms
  * Cost functions
  * With points
* Driving Distance
  * With points
* Yen's algorithm
* Traveling Sales Person (TSP)

and many more.

The latest documentation: https://docs.pgrouting.org/latest

## REQUIREMENTS

### Building requirements

* perl
* C and C++ compilers
  * Compiling with Boost 1.56 up to Boost 1.74 requires C++ Compiler with C++03 or C++11 standard support
  * Compiling with Boost 1.75 requires C++ Compiler with C++14 standard support
* Postgresql = Supported [version](https://www.postgresql.org/support/versioning/) by PostgreSQL
  * Not supporting v12 & under
* The Boost Graph Library (BGL) >= 1.56
* CMake >= 3.12
* 7.0 > Sphinx >= 4.0.0

### User's requirements

* PostGIS

## COMPILATION

For MinGW on Windows

	mkdir build
	cd build
	cmake -G"MSYS Makefiles" ..
	make
	make install

Also pre-built Windows binaries can be downloaded from https://postgis.net/windows_downloads

For Linux

	mkdir build
	cd build
	cmake  ..
	make
	sudo make install

Build with documentation (requires [Sphinx](http://sphinx-doc.org/))

	cmake -DWITH_DOC=ON ..

Postgresql

	createdb mydatabase
	psql mydatabase -c "CREATE EXTENSION pgrouting CASCADE"

## USAGE

See online documentation: http://docs.pgrouting.org/latest/en/index.html

## LICENSE

* Most features are available under [GPL-2.0-or-later](https://spdx.org/licenses/GPL-2.0-or-later.html)
* Some Boost extensions are available under Boost license (see LICENSE_1_0.txt)
* Some code contributed by iMaptools.com is available under MIT-X license.
