.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. sectionauthor:: Stephen Woodbridge <woodbri@swoodbridge.com>
.. sectionauthor:: Daniel Kastl <daniel@georepublic.de>

.. _developer:

*******************************************************************************
Developer's Documention Guide
*******************************************************************************

.. NOTE:: 
	All documentation should be in reStructuredText format.
	See: <http://docutils.sf.net/rst.html> for introductory docs.

Source Tree Layout
===============================================================================

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
    *DEPRECATED and DOES NOT WORK and IS BEING REMOVED*
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

tools/
    Miscellaneous scripts and tools.

lib/
    This is the output directory where compiled libraries and installation
    targets are staged before installation.


Documentation Layout
===============================================================================

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


Testing Infrastructure
===============================================================================

There is a very basic testing infrastructure put in place. Here are the
basics of how it works. We need more test cases. Longer term we should
probably get someone to setup travis-ci or jenkins testing frameworks.

Currently all the tests work off a simple graph that was built for
testing trsp functionality.

.. image:: images/trsp-test-image.png

Tests are run via the script at the top level test-runner.pl and it runs
all the test configured tests and at the moment just dumps the results
structure of the test. This can be prettied up later.

    It also assumes that you have installed the libraries as it tests
    using the installed postgresql. This probably needs to be made
    smarter so we can test out of the build tree. I'll need to think
    about that.

Basically each .../test/ directory should include one *test.conf* file that
is a perl script fragment that defines what data files to load and what
tests to run. I have built in some mechanisms to allow test and data to
be pg version and postgis version specific, but I'm not using that yet.
So for example, *core/trsp/test/test-any-00.data* is a sql plain text dump
that will load and needed data for a set of tests. This is also the graph
in the image above. You can specify multiple files to load, but as a
group they need to have unique names. 

core/trsp/test/test-any-00.test is a sql command to be run. It will get
run as:

.. :: 

    psql ... -A -t -q -f file.test dbname > tmpfile
    diff -w file.rest tmpfile 

Then if there is a difference then an test failure is reported.





