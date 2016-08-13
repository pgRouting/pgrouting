# pgRouting - Routing on PostgreSQL

[![Join the chat at https://gitter.im/pgRouting/pgrouting](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/pgRouting/pgrouting?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## STATUS

### Branches

The *master* branch reflects the current 2.2.2 release.
The *develop* branch reflects the 2.3.0-dev

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

Postgis 2.2 on Travis and AppVeyor
Postgis 2.3.0dev on Jenkins

Postgres matrix of tests:

 postgreSQL   | PostGis | Travis | AppVeyor | Jenkins
--- |---- | --- | ---
    | Precise | vs2013 | winnie 32 & 64 bits
 9.1 | yes | no | no 
 9.2 | yes | no | no
 9.3 | yes | no | no
 9.4 | yes | yes | yes
 9.5 | yes | no | yes
 9.6 | no  | no | yes





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

* C and C++ compilers
* Postgresql version >= 9.1
* PostGIS version >= 2.0
* The Boost Graph Library (BGL).
  * Version >= 1.46
* Version >= 1.55 for linux
* Version >= 1.58 for MAC
* CMake >= 2.8.8
* CGAL >= 4.4
* Sphinx >= 1.2

## INSTALLATION

See online documentation: http://docs.pgrouting.org/

## COMPILATION

For MinGW on Windows

	mkdir build
	cd build
	cmake -G"MSYS Makefiles" ..
	make
	make install

Also pre-built Windows binaries can be downloaded from http://.postgis.net/windows_downloads

For Linux
	
	mkdir build
	cd build
	cmake  ..
	make
	sudo make install

Build with documentation (requires [Sphinx](http://sphinx-doc.org/))

	cmake -DWITH_DOC=ON ..

Postgresql 9.1+

	createdb mydatabase
	psql mydatabase -c "CREATE EXTENSION postgis"
	psql mydatabase -c "CREATE EXTENSION pgrouting"

## USAGE

See online documentation: http://docs.pgrouting.org/2.2/en/doc/index.html

## LICENSE

* Most features are available under GPLv2.
* Some Boost extensions are available under Boost license (see LICENSE_1_0.txt)
* Some code contributed by iMaptools.com is available under MIT-X license.



