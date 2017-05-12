# pgRouting - Routing on PostgreSQL

[![Join the chat at https://gitter.im/pgRouting/pgrouting](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/pgRouting/pgrouting?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## STATUS

### Branches

* The *master* branch has the latest minor release.
* The *develop* branch has the next minor release.

For the complete list of releases go to:
https://github.com/pgRouting/pgrouting/releases


## LINKS

* http://pgrouting.org/ 
* http://docs.pgrouting.org/
* https://github.com/pgRouting/pgrouting

<table>
	<tr>
		<td>Branch</td>
		<td>Travis</td>
		<td>Appveyor</td>
		<td>Jenkins</td>
		<td>Comments</td>
	</tr>
	<tr>
		<td>master</td>
		<td><img src="https://travis-ci.org/pgRouting/pgrouting.png?branch=master" alt="Build Status"/></td>
		<td><img src="https://ci.appveyor.com/api/projects/status/github/pgRouting/pgrouting?branch=master&svg=true" alt="Appveyor Status"/></td>
        <td><img src="http://winnie.postgis.net:1500/buildStatus/icon?job=pgRouting_matrix_master" alt="Jenkins Status"/></td>
		<td></td>
	</tr>
	<tr>
		<td>develop</td>
		<td><img src="https://travis-ci.org/pgRouting/pgrouting.png?branch=develop" alt="Build Status"/></td>
		<td><img src="https://ci.appveyor.com/api/projects/status/github/pgRouting/pgrouting?branch=develop&svg=true" alt="Appveyor Status"/></td>
        <td><img src="http://winnie.postgis.net:1500/buildStatus/icon?job=pgRouting_matrix_branch_develop" alt="Jenkins Status"/></td>
		<td></td>
	</tr>
	<tr>
		<td>gh-pages</td>
		<td><img src="https://travis-ci.org/pgRouting/pgrouting.png?branch=gh-pages" alt="Build Status"/></td>
		<td></td>
		<td></td>
		<td>not enabled</td>
	</tr>
</table>

* travis: Postgis 2.2 for  Postgres 9.1, 9.2, 9.3, 9.4, 9.5
  * precise
* jenkins: g++ 4.8 on 32 & 64 bits for Postgis 2.3.0dev and Postgres 9.4, 9.5, 9.6
* appveyor: vs1013 Postgis 2.2 Postgres 9.4




## INTRODUCTION

pgRouting extends the PostGIS/PostgreSQL geospatial database to provide geospatial routing and other network analysis functionality.

This library contains following features:

* All Pairs Shortest Path, Johnson’s Algorithm
* All Pairs Shortest Path, Floyd-Warshall Algorithm
* Shortest Path A*
* Bi-directional Dijkstra Shortest Path
* Bi-directional A* Shortest Path
* Shortest Path Dijkstra Family of functions
* Shortest Path Dijkstra Cost Family of functions
* Driving Distance
* K-Shortest Path, Multiple Alternative Paths (Yen's algorithm)
* Traveling Sales Person
* Turn Restriction Shortest Path (TRSP)

Additionally, ready for testing and to be part of 2.3 official version:

* Shortest Path Dijkstra With Points Family of functions
* Shortest Path Dijkstra Cost With Points Family of functions
* Driving Distance With Points
* K-Shortest Path With Points, Multiple Alternative Paths (Yen's algorithm)
* Dijkstra Via vertices

Family of functions include:

* one to one
* one to many
* many to one
* many to many

## REQUIREMENTS

Building reqirements
--------------------
* Perl 
* C and C++ compilers
  * GNU >= 4.6
  * MSVC >= 18.0
* Postgresql >= 9.2
* The Boost Graph Library (BGL) >= 1.46
* CMake >= 2.8.8
* CGAL >= 4.4
* Sphinx >= 1.2


User's reqirements
--------------------

* PostGIS  >= 2.0

## INSTALLATION

See online documentation: http://docs.pgrouting.org/

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

Postgresql 9.2+

	createdb mydatabase
	psql mydatabase -c "CREATE EXTENSION postgis"
	psql mydatabase -c "CREATE EXTENSION pgrouting"

## USAGE

See online documentation: http://docs.pgrouting.org/2.3/en/doc/index.html

## LICENSE

* Most features are available under GPLv2.
* Some Boost extensions are available under Boost license (see LICENSE_1_0.txt)
* Some code contributed by iMaptools.com is available under MIT-X license.



