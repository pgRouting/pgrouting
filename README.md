# pgRouting - Routing on PostgreSQL

[![Financial Contributors on Open Collective](https://opencollective.com/pgrouting/all/badge.svg?label=financial+contributors)](https://opencollective.com/pgrouting) [![Join the chat at https://gitter.im/pgRouting/pgrouting](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/pgRouting/pgrouting?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## STATUS

### Branches

* The *master* branch has the latest release
* The *develop* branch has the development of the next release

For the complete list of releases go to:
https://github.com/pgRouting/pgrouting/releases


## LINKS

* http://pgrouting.org/
* http://docs.pgrouting.org/
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

The latest documentation: http://docs.pgrouting.org/latest

## REQUIREMENTS

Building requirements
--------------------
* Perl
* C and C++ compilers
  * GNU >= 4.8
  * MSVC >= 18.0
* Postgresql >= 9.4
* The Boost Graph Library (BGL) >= 1.53
* CMake >= 3.2
* Sphinx >= 1.2


User's requirements
--------------------

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

Postgresql 9.3+

	createdb mydatabase
	psql mydatabase -c "CREATE EXTENSION postgis"
	psql mydatabase -c "CREATE EXTENSION pgrouting"

## USAGE

See online documentation: http://docs.pgrouting.org/dev/en/index.html

## Contributors

### Code Contributors

This project exists thanks to all the people who contribute. [[Contribute](CONTRIBUTING.md)].
<a href="https://github.com/pgRouting/pgrouting/graphs/contributors"><img src="https://opencollective.com/pgrouting/contributors.svg?width=890&button=false" /></a>

### Financial Contributors

Become a financial contributor and help us sustain our community. [[Contribute](https://opencollective.com/pgrouting/contribute)]

#### Individuals

<a href="https://opencollective.com/pgrouting"><img src="https://opencollective.com/pgrouting/individuals.svg?width=890"></a>

#### Organizations

Support this project with your organization. Your logo will show up here with a link to your website. [[Contribute](https://opencollective.com/pgrouting/contribute)]

<a href="https://opencollective.com/pgrouting/organization/0/website"><img src="https://opencollective.com/pgrouting/organization/0/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/1/website"><img src="https://opencollective.com/pgrouting/organization/1/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/2/website"><img src="https://opencollective.com/pgrouting/organization/2/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/3/website"><img src="https://opencollective.com/pgrouting/organization/3/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/4/website"><img src="https://opencollective.com/pgrouting/organization/4/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/5/website"><img src="https://opencollective.com/pgrouting/organization/5/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/6/website"><img src="https://opencollective.com/pgrouting/organization/6/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/7/website"><img src="https://opencollective.com/pgrouting/organization/7/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/8/website"><img src="https://opencollective.com/pgrouting/organization/8/avatar.svg"></a>
<a href="https://opencollective.com/pgrouting/organization/9/website"><img src="https://opencollective.com/pgrouting/organization/9/avatar.svg"></a>

## LICENSE

* Most features are available under GPLv2.
* Some Boost extensions are available under Boost license (see LICENSE_1_0.txt)
* Some code contributed by iMaptools.com is available under MIT-X license.


