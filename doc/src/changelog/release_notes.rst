..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _change_log:

Release Notes
===============================================================================

To see the full list of changes check the list of `Git commits <https://github.com/pgRouting/pgrouting/commits>`_ on Github.

Table of contents
--------------------------

   - :ref:`changelog_2_4_0`
   - :ref:`changelog_2_3_1`
   - :ref:`changelog_2_3_0`
   - :ref:`changelog_2_2_4`
   - :ref:`changelog_2_2_3`
   - :ref:`changelog_2_2_2`
   - :ref:`changelog_2_2_1`
   - :ref:`changelog_2_2_0`
   - :ref:`changelog_2_1_0`
   - :ref:`changelog_2_0_1`
   - :ref:`changelog_2_0_0`
   - :ref:`changelog_1_x`

.. _changelog_2_4_0:

pgRouting 2.4.0 Release Notes
===============================================================================

.. rubric:: New Signatures

* pgr_bdDijkstra


.. rubric:: New Proposed Signatures
* pgr_maxFlow
* pgr_astar(one to many)
* pgr_astar(many to one)
* pgr_astar(many to many)
* pgr_astarCost(one to one)
* pgr_astarCost(one to many)
* pgr_astarCost(many to one)
* pgr_astarCost(many to many)



.. rubric:: Deprecated Signatures

* pgr_bddijkstra - use pgr_bdDijkstra instead

.. rubric:: Bug fix

* Bug fixes on proposed functions

  * pgr_withPointsKSP: fixed ordering

.. _changelog_2_3_1:

pgRouting 2.3.1 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.3.1 <https://github.com/pgRouting/pgrouting/issues?q=is%3Aissue+milestone%3A%22Release+2.3.1%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Leaks on proposed max_flow functions
* Regression error on pgr_trsp
* Types discrepancy on pgr_createVerticesTable


.. _changelog_2_3_0:

pgRouting 2.3.0 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.3.0 <https://github.com/pgRouting/pgrouting/issues?q=is%3Aissue+milestone%3A%22Release+2.3.0%22+is%3Aclosed>`_ on Github.

.. rubric:: New Signatures

Indentifiers can be `ANY-INTEGER` and costs can be `ANY-NUMERICAL`

* pgr_TSP
* pgr_aStar

.. rubric:: New Functions

* pgr_eucledianTSP


.. rubric:: New Proposed functions

* pgr_dijkstraCostMatrix
* pgr_withPointsCostMatrix
* pgr_maxFlowPushRelabel
* pgr_maxFlowEdmondsKarp
* pgr_maxFlowBoykovKolmogorov 
* pgr_maximumCardinalityMatching
* pgr_edgeDisjointPaths
* pgr_contractGraph


.. rubric:: Deprecated Signatures

* pgr_tsp - use pgr_TSP or pgr_eucledianTSP instead
* pgr_astar - use pgr_aStar instead


.. rubric:: Deprecated Functions

* pgr_flip_edges
* pgr_vidsToDmatrix
* pgr_pointsToDMatrix
* pgr_textToPoints



.. _changelog_2_2_4:

pgRouting 2.2.4 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.2.4 <https://github.com/pgRouting/pgrouting/issues?q=is%3Aissue+milestone%3A%22Release+2.2.4%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Bogus uses of extern "C"
* Build error on Fedora 24 + GCC 6.0
* Regression error pgr_nodeNetwork

.. _changelog_2_2_3:

pgRouting 2.2.3 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.2.3 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.2.3%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Fixed compatibility issues with PostgreSQL 9.6.

.. _changelog_2_2_2:

pgRouting 2.2.2 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.2.2 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.2.2%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Fixed regression error on pgr_drivingDistance


.. _changelog_2_2_1:

pgRouting 2.2.1 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.2.1 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A2.2.1+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Server crash fix on pgr_alphaShape
* Bug fix on With Points family of functions


.. _changelog_2_2_0:

pgRouting 2.2.0 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.2.0 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=is%3Aissue+milestone%3A%22Release+2.2.0%22+is%3Aclosed>`_ on Github.


.. rubric:: Improvements

- pgr_nodeNetwork

  - Adding a row_where and outall optional parameters

- Signature fix

  - pgr_dijkstra  -- to match what is documented


.. rubric:: New Functions

- pgr_floydWarshall
- pgr_Johnson
- pgr_DijkstraCost

.. rubric:: Proposed functionality

- pgr_withPoints
- pgr_withPointsCost
- pgr_withPointsDD
- pgr_withPointsKSP
- pgr_dijkstraVia


.. rubric:: Deprecated functions:

- pgr_apspWarshall  use pgr_floydWarshall instead
- pgr_apspJohnson   use pgr_Johnson instead
- pgr_kDijkstraCost use pgr_dijkstraCost instead
- pgr_kDijkstraPath use pgr_dijkstra instead


.. _changelog_2_1_0:

pgRouting 2.1.0 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.1.0 <https://github.com/pgRouting/pgrouting/issues?q=is%3Aissue+milestone%3A%22Release+2.1.0%22+is%3Aclosed>`_ on Github.

.. rubric:: Refactored

- pgr_dijkstra
- pgr_ksp
- pgr_drivingDistance

.. rubric:: Improvements

- Alphashape function now can generate better (multi)polygon with holes and alpha parameter.

.. rubric:: Proposed functionality

- Proposed functions from Steve Woodbridge, (Classified as Convenience by the author.)

  - pgr_pointToEdgeNode - convert a point geometry to a vertex_id based on closest edge.
  - pgr_flipEdges - flip the edges in an array of geometries so the connect end to end.
  - pgr_textToPoints - convert a string of x,y;x,y;... locations into point geometries.
  - pgr_pointsToVids - convert an array of point geometries into vertex ids.
  - pgr_pointsToDMatrix - Create a distance matrix from an array of points.
  - pgr_vidsToDMatrix - Create a distance matrix from an array of vertix_id.
  - pgr_vidsToDMatrix - Create a distance matrix from an array of vertix_id.

- Added proposed functions from GSoc Projects:

  - pgr_vrppdtw

.. rubric:: No longer supported

- Removed the 1.x legacy functions

.. rubric:: Bug Fixes

- Some bug fixes in other functions


.. rubric:: Refactoring Internal Code

- A C and C++ library for developer was created

  - encapsulates postgreSQL related functions
  - encapsulates Boost.Graph graphs

    - Directed Boost.Graph
    - Undirected Boost.graph.

  - allow any-integer in the id's
  - allow any-numerical on the cost/reverse_cost columns

- Instead of generating many libraries:
  - All functions are encapsulated in one library
  - The library has a the prefix 2-1-0



.. _changelog_2_0_1:

pgRouting 2.0.1 Release Notes
===============================================================================

Minor bug fixes.

.. rubric:: Bug Fixes

* No track of the bug fixes were kept.


.. _changelog_2_0_0:

pgRouting 2.0.0 Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 2.0.0 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.0.0%22+is%3Aclosed>`_ on Github.

With the release of pgRouting 2.0.0 the library has abandoned backwards compatibility to :ref:`pgRouting 1.x <changelog_1_x>` releases.
The main Goals for this release are:

* Major restructuring of pgRouting.
* Standardiziation of the function naming
* Prepararation of the project for future development.
 
As a result of this effort:

* pgRouting has a simplified structure
* Significant new functionality has being added
* Documentation has being integrated
* Testing has being integrated
* And made it easier for multiple developers to make contributions.


.. rubric:: Important Changes

* Graph Analytics - tools for detecting and fixing connection some problems in a graph
* A collection of useful utility functions
* Two new All Pairs Short Path algorithms (pgr_apspJohnson, pgr_apspWarshall)
* Bi-directional Dijkstra and A-star search algorithms (pgr_bdAstar, pgr_bdDijkstra)
* One to many nodes search (pgr_kDijkstra)
* K alternate paths shortest path (pgr_ksp)
* New TSP solver that simplifies the code and the build process (pgr_tsp), dropped "Gaul Library" dependency
* Turn Restricted shortest path (pgr_trsp) that replaces Shooting Star
* Dropped support for Shooting Star
* Built a test infrastructure that is run before major code changes are checked in
* Tested and fixed most all of the outstanding bugs reported against 1.x that existing in the 2.0-dev code base.
* Improved build process for Windows
* Automated testing on Linux and Windows platforms trigger by every commit
* Modular library design
* Compatibility with PostgreSQL 9.1 or newer
* Compatibility with PostGIS 2.0 or newer
* Installs as PostgreSQL EXTENSION
* Return types refactored and unified
* Support for table SCHEMA in function parameters
* Support for ``st_`` PostGIS function prefix
* Added ``pgr_`` prefix to functions and types
* Better documentation: http://docs.pgrouting.org


.. _changelog_1_x:

pgRouting 1.x Release Notes
===============================================================================

To see the issues closed by this release see the `Git closed issues for 1.x <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+1.x%22+is%3Aclosed>`_ on Github.
The following release notes have been copied from the previous ``RELEASE_NOTES`` file and are kept as a reference.


Changes for release 1.05
-------------------------------------------------------------------------------

* Bugfixes


Changes for release 1.03
-------------------------------------------------------------------------------

* Much faster topology creation
* Bugfixes


Changes for release 1.02
-------------------------------------------------------------------------------

* Shooting* bugfixes
* Compilation problems solved


Changes for release 1.01
-------------------------------------------------------------------------------

* Shooting* bugfixes


Changes for release 1.0
-------------------------------------------------------------------------------

* Core and extra functions are separated
* Cmake build process
* Bugfixes


Changes for release 1.0.0b
-------------------------------------------------------------------------------

* Additional SQL file with more simple names for wrapper functions
* Bugfixes


Changes for release 1.0.0a
-------------------------------------------------------------------------------

* Shooting* shortest path algorithm for real road networks
* Several SQL bugs were fixed


Changes for release 0.9.9
-------------------------------------------------------------------------------

* PostgreSQL 8.2 support
* Shortest path functions return empty result if they couldn’t find any path


Changes for release 0.9.8
-------------------------------------------------------------------------------

* Renumbering scheme was added to shortest path functions
* Directed shortest path functions were added
* routing_postgis.sql was modified to use dijkstra in TSP search
