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
* C and C++ compilers
* Postgresql version >= 9.1
* PostGIS version >= 2.0
* The Boost Graph Library (BGL).
* Version >= 1.55 for linux
* Version >= 1.58 for MAC
* CMake >= 2.8.8
* CGAL >= 4.4
* Sphinx >= 1.2

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



