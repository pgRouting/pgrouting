=======================================
pgRouting Developer's Documention Guide
=======================================
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Date: $Date: 2013-03-17 10:07:00 -0500 (Sun, 17 Mar 2013) $
:Revision: $Revision: 0000 $
:Description: A guide to documention for developers of pgRouting

+-----------------------------------------------------------------------+
| *NOTE* All documentation should be in reStructuredText format.        |
| See: <http://docutils.sf.net/rst.html> for introductory docs.         |
+-----------------------------------------------------------------------+

Source Tree Layout
==================

build/
    Miscellaneous scripts and tools.

cmake/
    cmake scripts used as part of our build system.

core/
    This is the algorithm source tree. Each alogorithm should be contained
    in its on sub-tree with doc, sql, src, and test sub-directories.
    This might get renamed to "algorithms" at some point.

core/astar/
    This is an implementation of A* Search based on using Boost Graph
    libraries for its implementation. This is a Dijkstra shortest path
    implementation with a Euclidean Heuristic.

core/common/
    At the moment this does not have an core in "src", but does have a lot
    of SQL wrapper code and topology code in the "sql" directory. *Algorithm
    specific wrappers should get move to the algorithm tree and appropriate
    tests should get added to validate the wrappers.*

core/dijkstra/
    This is an implementation of Dikjstra's shortest path solution using
    Boost Graph libraries for the implementation.

core/driving_distance/
    This optional package creates driving distance polygons based on
    solving a Dijkstra shortest path solution, then creating polygons
    based on equal cost distances from the start point.
    This optional package requires CGAL libraries to be installed.

core/shooting_star/
    *DEPRECATED and DOES NOT WORK*
    This is an edge based shortest path algorithm that supports turn
    restrictions. It is based on Boost Graph.
    Do *NOT* use this algorithm as it is broken, instead use *trsp*
    which has the same functionality and is faster and give correct results.

core/trsp/
    This is a turn restricted shortest path algorithm. It has some nice
    features like you can specify the start and end points as a percentage
    along an edge. Restrictions are stored in a separate table from the
    graph edges and this make it easier to manage the data.

core/tsp/
    This optional package provides the ability to compute traveling
    salesman problem solutions and compute the resulting route.
    This optional package requires GAUL libaries to be installed.

lib/
    This is the output directory where compiled libraries and installation
    targets are staged before installation.

Documentation Layout
====================

*As noted above all documentation should be done using reStructuredText
formated files.*

Documentation is distributed into the source tree. This top level "doc"
directory is intended for high level documentation cover subjects like:

    * Compiling and testing
    * Installation
    * Tutorials
    * Users' Guide front materials
    * Reference Manual front materials
    * etc

Since the algorithm specific documentation is contained in the source
tree with the algorithm specific files, the process of building the 
documentation and publishing it will need to assemble the details with
the front material as needed.

Also, to keep the "doc" directory from getting cluttered, each major book
like those listed above, should be contained in a separate directory under
"doc". Any images or other materials related to the book should also be kept
in that directory.


