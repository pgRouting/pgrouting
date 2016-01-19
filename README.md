<<<<<<< HEAD
# pgRouting - Routing on PostgreSQL

[![Join the chat at https://gitter.im/pgRouting/pgrouting](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/pgRouting/pgrouting?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## STATUS

### Current Development is in the *develop* branch

The *master* branch reflects our current 2.1.0 release.
Post 2.1.0 development will happen in the *develop* branch.


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

=======
# OSMContraction

OSMContraction is a cpp library which compresses OSM graph data and makes routing faster.

### Version
0.0.0

### Technologies Used

OSMContraction uses a number of open source projects to work properly:

* Boost Graph libraries - CPP libraries for graphs.
* PostgreSQL - A relational database query language
* Postgis - A plugin for Postgresql.
* Osm2pgrouting - A tool which converts osm data into a postgres schema.

### Requirements
>>>>>>> aea359f36032a4fb2841d4d6abdc5d0886759c39
* C and C++ compilers
* Postgresql version >= 9.1
* PostGIS version >= 2.0
* The Boost Graph Library (BGL).
<<<<<<< HEAD
  * Version >= 1.55 for linux
  * Version >= 1.58 for MAC
=======
* Version >= 1.55 for linux
* Version >= 1.58 for MAC
>>>>>>> aea359f36032a4fb2841d4d6abdc5d0886759c39
* CMake >= 2.8.8
* CGAL >= 4.4
* Sphinx >= 1.2

<<<<<<< HEAD
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
=======
### Installation

Install the above requirements



```sh
$ git clone [git-repo-url] directory_path
$ cd directory
$ cd build
$ cmake ..
$ make
$ sudo make install
```
### Usage

This query gives the shortest path between two vertices in the OSM data.

```sh
$ psql > select my_dijkstra('SELECT * from ways',start_vid,end_vid);
```

### Todos

 - Write Graph Contraction class in cpp
 - Writing a postgres query which contracts the graph
 - Testing the query for different cases


### Testing

 - The functionality of Graph contraction class is defined in graphMinimizer.hpp
 - Its functionality can be tested using the test.cpp file. 



>>>>>>> aea359f36032a4fb2841d4d6abdc5d0886759c39
