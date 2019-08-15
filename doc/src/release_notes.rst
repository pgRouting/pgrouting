..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _change_log:

Release Notes
===============================================================================

To see the full list of changes check the list of `Git commits <https://github.com/pgRouting/pgrouting/commits>`_ on Github.

.. rubric:: Table of contents

.. changelog start

* :ref:`changelog_3_0_0`
* :ref:`changelog_2_6_2`
* :ref:`changelog_2_6_1`
* :ref:`changelog_2_6_0`
* :ref:`changelog_2_5_5`
* :ref:`changelog_2_5_4`
* :ref:`changelog_2_5_3`
* :ref:`changelog_2_5_2`
* :ref:`changelog_2_5_1`
* :ref:`changelog_2_5_0`
* :ref:`changelog_2_4_2`
* :ref:`changelog_2_4_1`
* :ref:`changelog_2_4_0`
* :ref:`changelog_2_3_2`
* :ref:`changelog_2_3_1`
* :ref:`changelog_2_3_0`
* :ref:`changelog_2_2_4`
* :ref:`changelog_2_2_3`
* :ref:`changelog_2_2_2`
* :ref:`changelog_2_2_1`
* :ref:`changelog_2_2_0`
* :ref:`changelog_2_1_0`
* :ref:`changelog_2_0_1`
* :ref:`changelog_2_0_0`
* :ref:`changelog_1_x`

.. changelog end

.. _changelog_3_0_0:

pgRouting 3.0.0 Release Notes
-------------------------------------------------------------------------------

.. rubric:: Fixed Issues

* `#1153 <https://github.com/pgRouting/pgrouting/issues/1153>`__: Renamed pgr_eucledianTSP to pgr_TSPeuclidean
* `#1188 <https://github.com/pgRouting/pgrouting/issues/1188>`__: Removed CGAL dependency
* `#1002 <https://github.com/pgRouting/pgrouting/issues/1002>`__: Fixed contraction issues:

  * `#1004 <https://github.com/pgRouting/pgrouting/issues/1004>`__: Contracts when forbidden vertices do not belong to graph
  * `#1005 <https://github.com/pgRouting/pgrouting/issues/1005>`__: Intermideate results eliminated
  * `#1006 <https://github.com/pgRouting/pgrouting/issues/1006>`__: No loss of information

.. rubric:: New functions

* Kruskal family

  * pgr_kruskal
  * pgr_kruskalBFS
  * pgr_kruskalDD
  * pgr_kruskalDFS

* Prim family

  * pgr_prim
  * pgr_primDD
  * pgr_primDFS
  * pgr_primBFS


.. rubric:: Proposed moved to official on pgRouting

* aStar Family

  * pgr_aStar(one to many)
  * pgr_aStar(many to one)
  * pgr_aStar(many to many)
  * pgr_aStarCost(one to one)
  * pgr_aStarCost(one to many)
  * pgr_aStarCost(many to one)
  * pgr_aStarCost(many to many)
  * pgr_aStarCostMatrix(one to one)
  * pgr_aStarCostMatrix(one to many)
  * pgr_aStarCostMatrix(many to one)
  * pgr_aStarCostMatrix(many to many)

* bdAstar Family

  * pgr_bdAstar(one to many)
  * pgr_bdAstar(many to one)
  * pgr_bdAstar(many to many)
  * pgr_bdAstarCost(one to one)
  * pgr_bdAstarCost(one to many)
  * pgr_bdAstarCost(many to one)
  * pgr_bdAstarCost(many to many)
  * pgr_bdAstarCostMatrix(one to one)
  * pgr_bdAstarCostMatrix(one to many)
  * pgr_bdAstarCostMatrix(many to one)
  * pgr_bdAstarCostMatrix(many to many)

* bdDijkstra Family

  * pgr_bdDijkstra(one to many)
  * pgr_bdDijkstra(many to one)
  * pgr_bdDijkstra(many to many)
  * pgr_bdDijkstraCost(one to one)
  * pgr_bdDijkstraCost(one to many)
  * pgr_bdDijkstraCost(many to one)
  * pgr_bdDijkstraCost(many to many)
  * pgr_bdDijkstraCostMatrix(one to one)
  * pgr_bdDijkstraCostMatrix(one to many)
  * pgr_bdDijkstraCostMatrix(many to one)
  * pgr_bdDijkstraCostMatrix(many to many)

* Flow Family

  * pgr_pushRelabel(one to one)
  * pgr_pushRelabel(one to many)
  * pgr_pushRelabel(many to one)
  * pgr_pushRelabel(many to many)
  * pgr_edmondsKarp(one to one)
  * pgr_edmondsKarp(one to many)
  * pgr_edmondsKarp(many to one)
  * pgr_edmondsKarp(many to many)
  * pgr_boykovKolmogorov (one to one)
  * pgr_boykovKolmogorov (one to many)
  * pgr_boykovKolmogorov (many to one)
  * pgr_boykovKolmogorov (many to many)
  * pgr_maxCardinalityMatching
  * pgr_maxFlow
  * pgr_edgeDisjointPaths(one to one)
  * pgr_edgeDisjointPaths(one to many)
  * pgr_edgeDisjointPaths(many to one)
  * pgr_edgeDisjointPaths(many to many)

* Components family

  * pgr_connectedComponents
  * pgr_strongComponents
  * pgr_biconnectedComponents
  * pgr_articulationPoints
  * pgr_bridges

* Contraction:

  * Removed unnecessary column seq
  * Bug Fixes


.. rubric:: New Experimental functions

* pgr_maxFlowMinCost
* pgr_maxFlowMinCost_Cost
* pgr_extractVertices
* pgr_turnRestrictedPath
* pgr_stoerWagner
* pgr_dagShortestpath
* pgr_topologicalSort
* pgr_transitiveClosure
* VRP category

  * pgr_pickDeliverEuclidean
  * pgr_pickDeliver

* Chinese Postman family

  * pgr_chinesePostman
  * pgr_chinesePostmanCost

* Breadth First Search family

  * pgr_breadthFirstSearch
  * pgr_binaryBreadthFirstSearch

* Bellman Ford family

  * pgr_bellmanFord
  * pgr_edwardMoore

.. rubric:: Moved to legacy

* Experimental functions

  * pgr_labelGraph  -  Use the components family of functions instead.
  * Max flow - functions were renamed on v2.5.0

    * pgr_maxFlowPushRelabel
    * pgr_maxFlowBoykovKolmogorov
    * pgr_maxFlowEdmondsKarp
    * pgr_maximumcardinalitymatching

  * VRP

    * pgr_gsoc_vrppdtw

* TSP old signatures
* pgr_pointsAsPolygon
* pgr_alphaShape old signature



.. _changelog_2_6_2:

pgRouting 2.6.2 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.6.2 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.6.2%22%20>`_ on Github.

.. rubric:: Bug fixes

* `#1152 <https://github.com/pgRouting/pgrouting/issues/1152>`__ Fixes driving distance when vertex is not part of the graph
* `#1098 <https://github.com/pgRouting/pgrouting/issues/1098>`__ Fixes windows test
* `#1165 <https://github.com/pgRouting/pgrouting/issues/1165>`__ Fixes build for python3 and perl5

.. _changelog_2_6_1:

pgRouting 2.6.1 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.6.1 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.6.1%22%20>`_ on Github.

* Fixes server crash on several functions.

  * pgr_floydWarshall
  * pgr_johnson
  * pgr_astar
  * pgr_bdAstar
  * pgr_bdDijstra
  * pgr_alphashape
  * pgr_dijkstraCostMatrix
  * pgr_dijkstra
  * pgr_dijkstraCost
  * pgr_drivingDistance
  * pgr_KSP
  * pgr_dijkstraVia (proposed)
  * pgr_boykovKolmogorov (proposed)
  * pgr_edgeDisjointPaths (proposed)
  * pgr_edmondsKarp (proposed)
  * pgr_maxCardinalityMatch (proposed)
  * pgr_maxFlow (proposed)
  * pgr_withPoints (proposed)
  * pgr_withPointsCost (proposed)
  * pgr_withPointsKSP (proposed)
  * pgr_withPointsDD (proposed)
  * pgr_withPointsCostMatrix (proposed)
  * pgr_contractGraph (experimental)
  * pgr_pushRelabel (experimental)
  * pgr_vrpOneDepot (experimental)
  * pgr_gsoc_vrppdtw (experimental)
  * Fixes for deprecated functions where also applied but not tested

* Removed compilation warning for g++8
* Fixed a fallthrugh on Astar and bdAstar.

.. _changelog_2_6_0:

pgRouting 2.6.0 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.6.0 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.6.0%22%20>`_ on Github.


.. rubric:: New fexperimental functions

*  pgr_lineGraphFull

.. rubric:: Bug fixes

* Fix pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean[,text])

  * without restrictions

    * calls pgr_dijkstra when both end points have a fraction IN (0,1)
    * calls pgr_withPoints when at least one fraction NOT IN (0,1)

  * with restrictions

    * calls original trsp code

.. rubric:: Internal code

* Cleaned the internal code of trsp(text,integer,integer,boolean,boolean [, text])

  * Removed the use of pointers
  * Internal code can accept BIGINT

* Cleaned the internal code of withPoints

.. _changelog_2_5_5:

pgRouting 2.5.5 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.5.5 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.5.5%22%20>`_ on Github.

.. rubric:: Bug fixes

* Fixes driving distance when vertex is not part of the graph
* Fixes windows test
* Fixes build for python3 and perl5

.. _changelog_2_5_4:

pgRouting 2.5.4 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.5.4 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.5.4%22%20>`_ on Github.

* Fixes server crash on several functions.

  * pgr_floydWarshall
  * pgr_johnson
  * pgr_astar
  * pgr_bdAstar
  * pgr_bdDijstra
  * pgr_alphashape
  * pgr_dijkstraCostMatrix
  * pgr_dijkstra
  * pgr_dijkstraCost
  * pgr_drivingDistance
  * pgr_KSP
  * pgr_dijkstraVia (proposed)
  * pgr_boykovKolmogorov (proposed)
  * pgr_edgeDisjointPaths (proposed)
  * pgr_edmondsKarp (proposed)
  * pgr_maxCardinalityMatch (proposed)
  * pgr_maxFlow (proposed)
  * pgr_withPoints (proposed)
  * pgr_withPointsCost (proposed)
  * pgr_withPointsKSP (proposed)
  * pgr_withPointsDD (proposed)
  * pgr_withPointsCostMatrix (proposed)
  * pgr_contractGraph (experimental)
  * pgr_pushRelabel (experimental)
  * pgr_vrpOneDepot (experimental)
  * pgr_gsoc_vrppdtw (experimental)
  * Fixes for deprecated functions where also applied but not tested

* Removed compilation warning for g++8
* Fixed a fallthrugh on Astar and bdAstar.

.. _changelog_2_5_3:

pgRouting 2.5.3 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.5.3 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.5.3%22%20>`_ on Github.

.. rubric:: Bug fixes

* Fix for postgresql 11: Removed a compilation error when compiling with postgreSQL

.. _changelog_2_5_2:

pgRouting 2.5.2 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.5.2 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.5.2%22%20>`_ on Github.

.. rubric:: Bug fixes

* Fix for postgresql 10.1: Removed a compiler condition


.. _changelog_2_5_1:

pgRouting 2.5.1 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.5.1 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.5.1%22%20>`_ on Github.

.. rubric:: Bug fixes

* Fixed prerequisite minimum version of: cmake


.. _changelog_2_5_0:

pgRouting 2.5.0 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.5.0 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.5.0%22+is%3Aclosed>`_ on Github.


.. rubric:: enhancement:

* pgr_version is now on SQL language

.. rubric:: Breaking change on:

* pgr_edgeDisjointPaths:

  * Added path_id, cost and agg_cost columns on the result
  * Parameter names changed
  * The many version results are the union of the one to one version

.. rubric:: New Signatures:

* pgr_bdAstar(one to one)

.. rubric:: New Proposed functions

* pgr_bdAstar(one to many)
* pgr_bdAstar(many to one)
* pgr_bdAstar(many to many)
* pgr_bdAstarCost(one to one)
* pgr_bdAstarCost(one to many)
* pgr_bdAstarCost(many to one)
* pgr_bdAstarCost(many to many)
* pgr_bdAstarCostMatrix
* pgr_bdDijkstra(one to many)
* pgr_bdDijkstra(many to one)
* pgr_bdDijkstra(many to many)
* pgr_bdDijkstraCost(one to one)
* pgr_bdDijkstraCost(one to many)
* pgr_bdDijkstraCost(many to one)
* pgr_bdDijkstraCost(many to many)
* pgr_bdDijkstraCostMatrix
* pgr_lineGraph
* pgr_lineGraphFull
* pgr_connectedComponents
* pgr_strongComponents
* pgr_biconnectedComponents
* pgr_articulationPoints
* pgr_bridges

.. rubric:: Deprecated Signatures

* pgr_bdastar - use pgr_bdAstar instead

.. rubric:: Renamed Functions

* pgr_maxFlowPushRelabel - use pgr_pushRelabel instead
* pgr_maxFlowEdmondsKarp -use pgr_edmondsKarp instead
* pgr_maxFlowBoykovKolmogorov - use pgr_boykovKolmogorov instead
* pgr_maximumCardinalityMatching - use pgr_maxCardinalityMatch instead

.. rubric:: Deprecated function

* pgr_pointToEdgeNode


.. _changelog_2_4_2:

pgRouting 2.4.2 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.4.2 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.4.2%22%20>`_ on Github.

.. rubric:: Improvement

* Works for postgreSQL 10

.. rubric:: Bug fixes

* Fixed: Unexpected error column "cname"
* Replace __linux__ with __GLIBC__ for glibc-specific headers and functions



.. _changelog_2_4_1:

pgRouting 2.4.1 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed milestone for 2.4.1 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%202.4.1%22%20>`_ on Github.

.. rubric:: Bug fixes

* Fixed compiling error on macOS
* Condition error on pgr_withPoints

.. _changelog_2_4_0:

pgRouting 2.4.0 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.4.0 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.4.0%22+is%3Aclosed>`_ on Github.

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
* pgr_astarCostMatrix

.. rubric:: Deprecated Signatures

* pgr_bddijkstra - use pgr_bdDijkstra instead

.. rubric:: Deprecated Functions

* pgr_pointsToVids

.. rubric:: Bug fixes

* Bug fixes on proposed functions

  * pgr_withPointsKSP: fixed ordering

* TRSP original code is used with no changes on the compilation warnings

.. _changelog_2_3_2:

pgRouting 2.3.2 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.3.2 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.3.2%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Fixed pgr_gsoc_vrppdtw crash when all orders fit on one truck.
* Fixed pgr_trsp:

  * Alternate code is not executed when the point is in reality a vertex
  * Fixed ambiguity on seq


.. _changelog_2_3_1:

pgRouting 2.3.1 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.3.1 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.3.1%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Leaks on proposed max_flow functions
* Regression error on pgr_trsp
* Types discrepancy on pgr_createVerticesTable


.. _changelog_2_3_0:

pgRouting 2.3.0 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.3.0 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.3.0%22+is%3Aclosed>`_ on Github.

.. rubric:: New Signatures

* pgr_TSP
* pgr_aStar

.. rubric:: New Functions

* pgr_eucledianTSP


.. rubric:: New Proposed functions

* pgr_dijkstraCostMatrix
* pgr_withPointsCostMatrix
* pgr_maxFlowPushRelabel(one to one)
* pgr_maxFlowPushRelabel(one to many)
* pgr_maxFlowPushRelabel(many to one)
* pgr_maxFlowPushRelabel(many to many)
* pgr_maxFlowEdmondsKarp(one to one)
* pgr_maxFlowEdmondsKarp(one to many)
* pgr_maxFlowEdmondsKarp(many to one)
* pgr_maxFlowEdmondsKarp(many to many)
* pgr_maxFlowBoykovKolmogorov (one to one)
* pgr_maxFlowBoykovKolmogorov (one to many)
* pgr_maxFlowBoykovKolmogorov (many to one)
* pgr_maxFlowBoykovKolmogorov (many to many)
* pgr_maximumCardinalityMatching
* pgr_edgeDisjointPaths(one to one)
* pgr_edgeDisjointPaths(one to many)
* pgr_edgeDisjointPaths(many to one)
* pgr_edgeDisjointPaths(many to many)
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
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.2.4 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.2.4%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Bogus uses of extern "C"
* Build error on Fedora 24 + GCC 6.0
* Regression error pgr_nodeNetwork

.. _changelog_2_2_3:

pgRouting 2.2.3 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.2.3 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.2.3%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Fixed compatibility issues with PostgreSQL 9.6.

.. _changelog_2_2_2:

pgRouting 2.2.2 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.2.2 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.2.2%22+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Fixed regression error on pgr_drivingDistance


.. _changelog_2_2_1:

pgRouting 2.2.1 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.2.1 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A2.2.1+is%3Aclosed>`_ on Github.

.. rubric:: Bug Fixes

* Server crash fix on pgr_alphaShape
* Bug fix on With Points family of functions


.. _changelog_2_2_0:

pgRouting 2.2.0 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.2.0 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.2.0%22+is%3Aclosed>`_ on Github.


.. rubric:: Improvements

- pgr_nodeNetwork

  - Adding a row_where and outall optional parameters

- Signature fix

  - pgr_dijkstra  -- to match what is documented


.. rubric:: New Functions

- pgr_floydWarshall
- pgr_Johnson
- pgr_dijkstraCost(one to one)
- pgr_dijkstraCost(one to many)
- pgr_dijkstraCost(many to one)
- pgr_dijkstraCost(many to many)

.. rubric:: Proposed functionality

- pgr_withPoints(one to one)
- pgr_withPoints(one to many)
- pgr_withPoints(many to one)
- pgr_withPoints(many to many)
- pgr_withPointsCost(one to one)
- pgr_withPointsCost(one to many)
- pgr_withPointsCost(many to one)
- pgr_withPointsCost(many to many)
- pgr_withPointsDD(single vertex)
- pgr_withPointsDD(multiple vertices)
- pgr_withPointsKSP
- pgr_dijkstraVia


.. rubric:: Deprecated functions:

- pgr_apspWarshall  use pgr_floydWarshall instead
- pgr_apspJohnson   use pgr_Johnson instead
- pgr_kDijkstraCost use pgr_dijkstraCost instead
- pgr_kDijkstraPath use pgr_dijkstra instead

.. rubric:: Renamed and deprecated function

- pgr_makeDistanceMatrix renamed to _pgr_makeDistanceMatrix


.. _changelog_2_1_0:

pgRouting 2.1.0 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.1.0 <https://github.com/pgRouting/pgrouting/issues?q=is%3Aissue+milestone%3A%22Release+2.1.0%22+is%3Aclosed>`_ on Github.

.. rubric:: New Signatures

- pgr_dijkstra(one to many)
- pgr_dijkstra(many to one)
- pgr_dijkstra(many to many)
- pgr_drivingDistance(multiple vertices)

.. rubric:: Refactored

- pgr_dijkstra(one to one)
- pgr_ksp
- pgr_drivingDistance(single vertex)

.. rubric:: Improvements

- pgr_alphaShape function now can generate better (multi)polygon with holes and alpha parameter.

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
  - pgr_vrponedepot

.. rubric:: Deprecated functions

- pgr_getColumnName
- pgr_getTableName
- pgr_isColumnCndexed
- pgr_isColumnInTable
- pgr_quote_ident
- pgr_versionless
- pgr_startPoint
- pgr_endPoint
- pgr_pointToId

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
  - The library has the prefix 2-1-0



.. _changelog_2_0_1:

pgRouting 2.0.1 Release Notes
-------------------------------------------------------------------------------

Minor bug fixes.

.. rubric:: Bug Fixes

* No track of the bug fixes were kept.


.. _changelog_2_0_0:

pgRouting 2.0.0 Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 2.0.0 <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+2.0.0%22+is%3Aclosed>`_ on Github.

With the release of pgRouting 2.0.0 the library has abandoned backwards compatibility to :ref:`pgRouting 1.x <changelog_1_x>` releases.
The main Goals for this release are:

* Major restructuring of pgRouting.
* Standardization of the function naming
* Preparation of the project for future development.

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
* Return types re factored and unified
* Support for table SCHEMA in function parameters
* Support for ``st_`` PostGIS function prefix
* Added ``pgr_`` prefix to functions and types
* Better documentation: https://docs.pgrouting.org
* shooting_star is discontinued



.. _changelog_1_x:

pgRouting 1.x Release Notes
-------------------------------------------------------------------------------

To see the issues closed by this release see the `Git closed issues for 1.x <https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+1.x%22+is%3Aclosed>`_ on Github.
The following release notes have been copied from the previous ``RELEASE_NOTES`` file and are kept as a reference.


Changes for release 1.05
...............................................................................

* Bug fixes


Changes for release 1.03
...............................................................................

* Much faster topology creation
* Bug fixes


Changes for release 1.02
...............................................................................

* Shooting* bug fixes
* Compilation problems solved


Changes for release 1.01
...............................................................................

* Shooting* bug fixes


Changes for release 1.0
...............................................................................

* Core and extra functions are separated
* Cmake build process
* Bug fixes


Changes for release 1.0.0b
...............................................................................

* Additional SQL file with more simple names for wrapper functions
* Bug fixes


Changes for release 1.0.0a
...............................................................................

* Shooting* shortest path algorithm for real road networks
* Several SQL bugs were fixed


Changes for release 0.9.9
...............................................................................

* PostgreSQL 8.2 support
* Shortest path functions return empty result if they could not find any path


Changes for release 0.9.8
...............................................................................

* Renumbering scheme was added to shortest path functions
* Directed shortest path functions were added
* routing_postgis.sql was modified to use dijkstra in TSP search
