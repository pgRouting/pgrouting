# pgRouting - Routing on PostgreSQL

[![Join the chat at https://gitter.im/pgRouting/pgrouting](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/pgRouting/pgrouting?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## STATUS

### Current Development is in the *dev-2.2* branch

The *master* branch reflects our current 2.1.0 release.



## LINKS

* http://pgrouting.org/ 
* http://docs.pgrouting.org/dev/doc/index.html
* https://github.com/pgRouting/pgrouting

<table>
	<tr>
		<td>Branch</td>
		<td>Status</td>
		<td>Comment</td>
	</tr>
	<tr>
		<td>master</td>
		<td><img src="https://travis-ci.org/pgRouting/pgrouting.png?branch=master" alt="Build Status"/></td>
		<td>not enabled</td>
	</tr>
	<tr>
		<td>develop</td>
		<td><img src="https://travis-ci.org/pgRouting/pgrouting.png?branch=develop" alt="Build Status"/></td>
		<td></td>
	</tr>

	<tr>
		<td>gh-pages</td>
		<td><img src="https://travis-ci.org/pgRouting/pgrouting.png?branch=gh-pages" alt="Build Status"/></td>
		<td>not enabled</td>
	</tr>
</table>

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
* K-Shortest Path, Multiple Alternative Paths
* K-Dijkstra, One to Many Shortest Path
* Traveling Sales Person
* Turn Restriction Shortest Path (TRSP)


## REQUIREMENTS

* C and C++ compilers
* Postgresql version >= 9.1
* PostGIS version >= 2.0
* The Boost Graph Library (BGL).
  * Version >= 1.46
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
	psql mydatabase -c "create extension postgis"
	psql mydatabase -c "create extension pgrouting"

## USAGE

See online documentation: http://docs.pgrouting.org/dev/doc/index.html


## LICENSE

* Most features are available under GPLv2.
* Some Boost extensions are available under Boost license (see LICENSE_1_0.txt)
* Some code contributed by iMaptools.com is available under MIT-X license.
