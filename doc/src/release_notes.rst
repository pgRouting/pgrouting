:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2016-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

|


Release Notes
===============================================================================

To see the full list of changes check the list of `Git commits
<https://github.com/pgRouting/pgrouting/commits>`_ on Github.

.. contents:: Mayors
   :local:
   :depth: 1

pgRouting 4
*******************************************************************************

.. contents:: Minors 4.x
   :local:
   :depth: 1

pgRouting 4.1
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

.. current

pgRouting 4.1.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`4.1.0`

.. rubric:: Code enhancements

* :issue:`3049`: Use enumeration on drivers and process.
* :issue:`3054`: Change error message when edges_sql is empty
* :issue:`3055`: bdDijkstra: use the shortest_path process and driver
* :issue:`3056`: edwardMoore: use the shortest_path process and driver
* :issue:`3060`: dagShortestPath: use the shortest_path process and driver
* :issue:`3064`: Astar: create an use a process & driverAstar

pgRouting 4.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 4.0.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`4.0.1`

.. rubric:: Bug Fixes

* :issue:`2966`: pgr_withPoints does not pick optimal route when fraction = 1
* :issue:`3034`: metrics driver should not be using new

.. rubric:: Code enhancements

* :issue:`3044`: Check and fix assert.hpp for cppcoreguidelines-explicit-virtual-functions
* :issue:`3040`: Check and fix cppcoreguidelines-virtual-class-destructor
* :issue:`3039`: Check and fix cppcoreguidelines-special-member-functions
* :issue:`3038`: Check and fix cppcoreguidelines-prefer-member-initializer
* :issue:`3037`: Check and fix cppcoreguidelines-init-variables
* :issue:`3035`: Pass by reference on existing drivers

Administration
...............................................................................

* :issue:`3031`: pgtap license to Creative commons and License texts
  standardization

pgRouting 4.0.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`4.0.0`

.. contents:: Contents
   :local:
   :depth: 1

Build
...............................................................................

* C++ standard is std17

  * Using this standard, all supported toolchains across our CI matrix will work.
  * The code is not yet modified to use std17:

    * If needed: ``-DCMAKE_CXX_STANDARD=14`` to lower the standard.

* The user's documentation is built by default.
* The doxygen documentation is built by default.

For developers:

* Set `-DUSE_CLANG_TIDY=ON` for clang tidy checks.
* Tidy checks are done on CI.

Documentation build
...............................................................................

* The doxygen documentation is built by default
* The HTML documentation is built by default
* The translated languages (en, es, sv, zh_Hans) HTML documentation are built by
  default
* `WITH-DOC` is not used anymore

User Documentation is not built when

* Sphinx is not found
* When all Sphinx formats are OFF

  * To not build HTML default format: `-DBUILD_HTML=OFF`

* When all languages are OFF

  * To build only English: `-DES=OFF -DSV=OFF -DZH_HANS=OFF`

* Documentation output location: ``build/doc/_build/<format>``

  * For example: for HTML output is on `build/doc/_build/html` directory

Developers' Documentation is not built when

* Doxygen is not found
* To not build Doxygen documentation: `-DBUILD_DOXY=OFF`

Summary of changes by function
...............................................................................

* pgr_aStar

  .. include:: pgr_aStar.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_aStarCost

  .. include:: pgr_aStarCost.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_bandwidth

  .. include:: pgr_bandwidth.rst
     :start-after: Version 4.0.0
     :end-before: Description

* pgr_bdAstar

  .. include:: pgr_bdAstar.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_bdAstarCost

  .. include:: pgr_bdAstarCost.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_bdDijkstra

  .. include:: pgr_bdDijkstra.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_bdDijkstraCost

  .. include:: pgr_bdDijkstraCost.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_bellmanFord

  .. include:: pgr_bellmanFord.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_binaryBreadthFirstSearch

  .. include:: pgr_binaryBreadthFirstSearch.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_bipartite

  .. include:: pgr_bipartite.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_boykovKolmogorov

  .. include:: pgr_boykovKolmogorov.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_breadthFirstSearch

  .. include:: pgr_breadthFirstSearch.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_contraction

  .. include:: pgr_contraction.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_dagShortestPath

  .. include:: pgr_dagShortestPath.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_depthFirstSearch

  .. include:: pgr_depthFirstSearch.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_dijkstra

  .. include:: pgr_dijkstra.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_dijkstraCost

  .. include:: pgr_dijkstraCost.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_dijkstraNear

  .. include:: pgr_dijkstraNear.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_dijkstraNearCost

  .. include:: pgr_dijkstraNearCost.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_dijkstraVia

  .. include:: pgr_dijkstraVia.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_edgeColoring

  .. include:: pgr_edgeColoring.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_edgeDisjointPaths

  .. include:: pgr_edgeDisjointPaths.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_edmondsKarp

  .. include:: pgr_edmondsKarp.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_edwardMoore

  .. include:: pgr_edwardMoore.rst
     :start-after: Version 4.0.0
     :end-before: Description

* pgr_kingOrdering

  .. include:: pgr_kingOrdering.rst
     :start-after: Version 4.0.0
     :end-before: Description

* pgr_KSP

  .. include:: pgr_KSP.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_maxCardinalityMatch

  .. include:: pgr_maxCardinalityMatch.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_maxFlow

  .. include:: pgr_maxFlow.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_pushRelabel

  .. include:: pgr_pushRelabel.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_sequentialVertexColoring

  .. include:: pgr_sequentialVertexColoring.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_sloanOrdering

  .. include:: pgr_sloanOrdering.rst
     :start-after: Version 4.0.0
     :end-before: Description

* pgr_topologicalSort

  .. include:: pgr_topologicalSort.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_transitiveClosure

  .. include:: pgr_transitiveClosure.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_trsp

  .. include:: pgr_trsp.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_trspVia

  .. include:: pgr_trspVia.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_trspVia_withPoints

  .. include:: pgr_trspVia_withPoints.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_trsp_withPoints

  .. include:: pgr_trsp_withPoints.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_TSP

  .. include:: pgr_TSP.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_TSPeuclidean

  .. include:: pgr_TSPeuclidean.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_turnRestrictedPath

  .. include:: pgr_turnRestrictedPath.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_withPoints

  .. include:: pgr_withPoints.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_withPointsCost

  .. include:: pgr_withPointsCost.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_withPointsCostMatrix

  .. include:: pgr_withPointsCostMatrix.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_withPointsDD

  .. include:: pgr_withPointsDD.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_withPointsKSP

  .. include:: pgr_withPointsKSP.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

* pgr_withPointsVia

  .. include:: pgr_withPointsVia.rst
     :start-after: Version 4.0.0
     :end-before: .. rubric

Functions promoted to official
...............................................................................

* :issue:`2701`: pgr_trsp
* :issue:`2701`: pgr_trsp
* :issue:`2701`: pgr_trspVia
* :issue:`2701`: pgr_trspVia_withPoints
* :issue:`2701`: pgr_trsp_withPoints
* :issue:`2700`: pgr_withPoints
* :issue:`2700`: pgr_withPointsCost
* :issue:`2700`: pgr_withPointsCostMatrix
* :issue:`2700`: pgr_withPointsDD
* :issue:`2700`: pgr_withPointsKSP
* :issue:`2700`: pgr_withPointsVia
* :issue:`2983`: pgr_dijkstraNear
* :issue:`2983`: pgr_dijkstraNearCost
* :issue:`2983`: pgr_dijkstraVia

Signatures promoted to official
...............................................................................

* :issue:`2718`: pgr_aStar(Combinations)
* :issue:`2718`: pgr_aStarCost(Combinations)
* :issue:`2718`: pgr_bdAstar(Combinations)
* :issue:`2718`: pgr_bdAstarCost(Combinations)
* :issue:`2718`: pgr_bdDijkstra(Combinations)
* :issue:`2718`: pgr_bdDijkstraCost(Combinations)
* :issue:`2718`: pgr_dijkstra(Combinations)
* :issue:`2718`: pgr_dijkstraCost(Combinations)
* :issue:`2718`: pgr_KSP(All signatures)
* :issue:`2718`: pgr_boykovKolmogorov(Combinations)
* :issue:`2718`: pgr_edmondsKarp(Combinations)
* :issue:`2718`: pgr_maxFlow(Combinations)
* :issue:`2718`: pgr_pushRelabel(Combinations)

New experimental functions.
...............................................................................

* Metrics

  * :issue:`2951`: pgr_bandwidth

* Ordering

  * :issue:`2954`: pgr_kingOrdering
  * :issue:`2955`: pgr_sloanOrdering

SQL signatures and output standardization
...............................................................................

:issue:`2904`: Standardize output columns of functions with different output
               columns within overloads

.. rubric:: Standardized to |short-generic-result|

* :issue:`2905`: pgr_withPoints
* :issue:`2906`: pgr_bdDijkstra
* :issue:`2907`: pgr_bellmanFord
* :issue:`2908`: pgr_binaryBreadthFirstSearch
* :issue:`2910`: pgr_edwardMoore
* :issue:`2913`: pgr_dagShortestPath

.. rubric:: Standardized to |matrix-result|

* :issue:`2905`: pgr_withPointsCost
* :issue:`2905`: pgr_withPointsCostMatrix

.. rubric:: Standardized to |generic-result|

* :issue:`2909`: pgr_edgeDisjointPaths
* :issue:`2909`: pgr_turnRestrictedPath

.. rubric:: Standardized to |result_edge_color|

* :issue:`2924`: pgr_edgeColoring

.. rubric:: Standardized to |result_node_color|

* :issue:`2924`: pgr_bipartite
* :issue:`2927`: pgr_sequentialVertexColoring

.. rubric:: Standardized to |result-spantree|

* :issue:`2931`: pgr_breadthFirstSearch
* :issue:`2931`: pgr_depthFirstSearch

.. rubric:: Standardized to |result_node_order|

* :issue:`2934`: pgr_topologicalSort

.. rubric:: Standardized to |result-closure|

* :issue:`2934`: pgr_transitiveClosure

Removal of SQL deprecated signatures
...............................................................................

* :issue:`2798`: pgr_contraction

  .. include:: pgr_contraction.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2683`: pgr_trsp

  .. include:: pgr_trsp.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2683`: pgr_trspVia

  .. include:: pgr_trspVia.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2700`: pgr_withPointsVia

  .. include:: pgr_withPointsVia.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2888`: pgr_findCloseEdges

  * pgr_findcloseedges(text,geometry,double precision,integer,boolean,boolean)
  * pgr_findcloseedges(text,geometry[],double precision,integer,boolean,boolean)

* :issue:`2890`: pgr_withPointsDD

  .. include:: pgr_withPointsDD.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2895`: pgr_withPointsKSP

  .. include:: pgr_withPointsKSP.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2899`: pgr_maxCardinalityMatch

  .. include:: pgr_maxCardinalityMatch.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2901`: pgr_TSP

  .. include:: pgr_TSP.rst
     :start-after: Breaking change
     :end-before: .. rubric

* :issue:`2901`: pgr_TSPeuclidean

  .. include:: pgr_TSPeuclidean.rst
     :start-after: Breaking change
     :end-before: .. rubric


Removal of SQL deprecated functions
...............................................................................

* :issue:`2681`: pgr_trspViaedges
* :issue:`2682`: pgr_trspViaVertices
* :issue:`2748`: pgr_alphaShape
* :issue:`2751`: pgr_createTopology
* :issue:`2752`: pgr_analyzeGraph
* :issue:`2755`: pgr_analyzeOneWay
* :issue:`2827`: pgr_createVerticesTable
* :issue:`2886`: pgr_nodeNetwork

Removal of SQL deprecated internal functions
...............................................................................

* :issue:`2748` _pgr_alphaShape(text,double precision)
* :issue:`2861` _pgr_checkVertTab(text,text[],integer,text)
* :issue:`2861` _pgr_createIndex(text,text,text,integer,text)
* :issue:`2861` _pgr_createIndex(text,text,text,text,integer,text)
* :issue:`2913` _pgr_dagShortestPath(text,anyarray,anyarray,boolean,boolean)
* :issue:`2913` _pgr_dagShortestPath(text,text,boolean,boolean)
* :issue:`2730` _pgr_dijkstraNear(text,anyarray,anyarray,bigint,boolean)
* :issue:`2730` _pgr_dijkstraNear(text,anyarray,bigint,bigint,boolean)
* :issue:`2730` _pgr_dijkstraNear(text,bigint,anyarray,bigint,boolean)
* :issue:`2730` _pgr_dijkstra(text,anyarray,anyarray,boolean,boolean,boolean,bigint)
* :issue:`2730` _pgr_dijkstra(text,anyarray,anyarray,boolean,boolean,boolean,bigint,boolean)
* :issue:`2730` _pgr_dijkstra(text,text,boolean,boolean,bigint,boolean)
* :issue:`2730` _pgr_dijkstra(text,text,boolean,boolean,boolean)
* :issue:`2735` _pgr_drivingDistance(text,anyarray,double precision,boolean,boolean)
* :issue:`2861` _pgr_endPoint(geometry)
* :issue:`2861` __pgr_getColumnName(text,text,integer,text)
* :issue:`2861` __pgr_getColumnName(text,text,text,integer,text)
* :issue:`2861` __pgr_getColumnType(text,text,integer,text)
* :issue:`2861` __pgr_getColumnType(text,text,text,integer,text)
* :issue:`2861` __pgr_getTableName(text,integer,text)
* :issue:`2861` _pgr_isColumnIndexed(text,text,integer,text)
* :issue:`2861` _pgr_isColumnIndexed(text,text,text,integer,text)
* :issue:`2861` _pgr_isColumnIntable(text,text)
* :issue:`2745` _pgr_kruskal(text,anyarray,text,bigint,double precision)
* :issue:`2897` _pgr_ksp(text,anyarray,anyarray,integer,boolean,boolean,boolean)
* :issue:`2897` _pgr_ksp(text,bigint,bigint,integer,boolean,boolean)
* :issue:`2897` _pgr_ksp(text,text,integer,boolean,boolean)
* :issue:`2899` _pgr_maxCardinalityMatch(text,boolean)
* :issue:`2861` _pgr_msg(integer,text,text)
* :issue:`2861` _pgr_onerror(boolean,integer,text,text,text,text)
* :issue:`2861` _pgr_pointtoid(geometry,double precision,text,integer)
* :issue:`2743` _pgr_prim(text,anyarray,text,bigint,double precision)
* :issue:`2861` _pgr_quote_ident(text)
* :issue:`2861` _pgr_startPoint(geometry)
* :issue:`2683` _pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
* :issue:`2683` _pgr_trsp(text,text,anyarray,anyarray,boolean)
* :issue:`2683` _pgr_trsp(text,text,anyarray,bigint,boolean)
* :issue:`2683` _pgr_trsp(text,text,bigint,anyarray,boolean)
* :issue:`2683` _pgr_trsp(text,text,bigint,bigint,boolean)
* :issue:`2682` _pgr_trspViaVertices(text,integer[],boolean,boolean,text)
* :issue:`2919` _pgr_trspVia_withPoints(text,text,text,anyarray,boolean,boolean,boolean,character,boolean)
* :issue:`2919` _pgr_trsp_withPoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
* :issue:`2919` _pgr_trsp_withPoints(text,text,text,text,boolean,character,boolean)
* :issue:`2901` _pgr_tspEuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* :issue:`2901` _pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* :issue:`2861` _pgr_versionLess(text,text)
* :issue:`2890` _pgr_withPointsDD(text,text,anyarray,double precision,boolean,character,boolean,boolean)
* :issue:`2895` _pgr_withPointsKSP(text,text,anyarray,anyarray,integer,character,boolean,boolean,boolean,boolean)
* :issue:`2895` _pgr_withPointsKSP(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)
* :issue:`2895` _pgr_withPointsKSP(text,text,text,integer,character,boolean,boolean,boolean)
* :issue:`2741` _pgr_withPointsVia(text,bigint[],double precision[],boolean)
* :issue:`2741` _pgr_withPointsVia(text,text,anyarray,boolean,boolean,boolean,character,boolean)
* :issue:`2683` _trsp(text,text,anyarray,anyarray,boolean)
* :issue:`2683` _v4trsp(text,text,anyarray,anyarray,boolean)
* :issue:`2683` _v4trsp(text,text,text,boolean)

Summary of functions and signatures no longer on pgrouting
...............................................................................

* :issue:`2748` pgr_alphashape(geometry,double precision)
* :issue:`2752` pgr_analyzegraph(text,double precision,text,text,text,text,text)
* :issue:`2755` pgr_analyzeoneway(text,text[],text[],text[],text[],boolean,text,text,text)
* :issue:`2798` pgr_contraction(text,bigint[],integer,bigint[],boolean)
* :issue:`2751` pgr_createtopology(text,double precision,text,text,text,text,text,boolean)
* :issue:`2827` pgr_createverticestable(text,text,text,text,text)
* :issue:`2888` pgr_findcloseedges(text,geometry,double precision,integer,boolean,boolean)
* :issue:`2888` pgr_findcloseedges(text,geometry[],double precision,integer,boolean,boolean)
* :issue:`2899` pgr_maxCardinalityMatch(text,boolean)
* :issue:`2886` pgr_nodenetwork(text,double precision,text,text,text,text,boolean)
* :issue:`2683` pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
* :issue:`2683` pgr_trsp(text,integer,integer,boolean,boolean,text)
* :issue:`2681` pgr_trspViaedges(text,integer[],double precision[],boolean,boolean,text)
* :issue:`2682` pgr_trspViaVertices(text,anyarray,boolean,boolean,text)
* :issue:`2919` pgr_trspVia_withPoints(text,text,text,anyarray,boolean,boolean,boolean,character,boolean)
* :issue:`2919` pgr_trsp_withPoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
* :issue:`2919` pgr_trsp_withPoints(text,text,text,anyarray,bigint,boolean,character,boolean)
* :issue:`2919` pgr_trsp_withPoints(text,text,text,bigint,anyarray,boolean,character,boolean)
* :issue:`2919` pgr_trsp_withPoints(text,text,text,bigint,bigint,boolean,character,boolean)
* :issue:`2919` pgr_trsp_withPoints(text,text,text,text,boolean,character,boolean)
* :issue:`2901` pgr_tspEuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* :issue:`2901` pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* :issue:`2919` pgr_withPointsCostMatrix(text,text,anyarray,boolean,character)
* :issue:`2919` pgr_withPointsCost(text,text,anyarray,anyarray,boolean,character)
* :issue:`2919` pgr_withPointsCost(text,text,anyarray,bigint,boolean,character)
* :issue:`2919` pgr_withPointsCost(text,text,bigint,anyarray,boolean,character)
* :issue:`2919` pgr_withPointsCost(text,text,bigint,bigint,boolean,character)
* :issue:`2919` pgr_withPointsCost(text,text,text,boolean,character)
* :issue:`2890` pgr_withPointsDD(text,text,anyarray,double precision,boolean,character,boolean,boolean)
* :issue:`2890` pgr_withPointsDD(text,text,bigint,double precision,boolean,character,boolean)
* :issue:`2895` pgr_withPointsKSP(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)
* :issue:`2919` pgr_withPoints(text,text,anyarray,anyarray,boolean,character,boolean)
* :issue:`2919` pgr_withPoints(text,text,anyarray,bigint,boolean,character,boolean)
* :issue:`2919` pgr_withPoints(text,text,bigint,anyarray,boolean,character,boolean)
* :issue:`2919` pgr_withPoints(text,text,bigint,bigint,boolean,character,boolean)
* :issue:`2919` pgr_withPoints(text,text,text,boolean,character,boolean)
* :issue:`2919` pgr_withPointsVia(text,text,anyarray,boolean,boolean,boolean,character,boolean)

Code enhancements
...............................................................................

* Removal of unused C/C++ code
* Refactor the Script to build the update PostgreSQL file.
* One process & driver for:

  * allpairs: johnson and Floyd-Warshall
  * Shortest path: Dijkstra and withPoints using Dijkstra

.. rubric:: Deprecation of internal C/C++ functions

Deprecated functions are substituted by new function.

* _pgr_drivingDistance => _pgr_drivingDistancev4
* _pgr_withPointsDD => _pgr_withPointsddv4
* _pgr_kruskal => _pgr_kruskalv4
* _pgr_prim => _pgr_primv4
* _pgr_dijkstra => _pgr_dijkstra_v4
* _pgr_withPointsKSP => _pgr_withPointsKSP_v4
* _pgr_trspVia_withPoints => _pgr_trspVia_withPoints_v4
* _pgr_trsp_withPoints => _pgr_trsp_withPoints_v4
* _pgr_withPointsVia => _pgr_withPointsvia_v4

.. rubric:: Internal C/C++ functions in legacy

* :issue:`2683` _trsp
* :issue:`2683` _v4trsp
* :issue:`2748` _pgr_alphaShape
* :issue:`2913` _pgr_dagShortestPath

pgRouting 3
*******************************************************************************

.. contents:: Minors 3.x
   :local:
   :depth: 1

pgRouting 3.8
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.8.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.8.0`

.. rubric:: Promotion to official function of pgRouting.

.. rubric:: Metric

* :issue:`2760`: Promoted to official pgr_degree in version 3.8

  .. include:: pgr_degree.rst
     :start-after: Version 3.8.0
     :end-before: .. rubric

.. rubric:: Utilities

* :issue:`2772`: Promoted to official pgr_extractVertices in version 3.8

  .. include:: pgr_extractVertices.rst
     :start-after: Version 3.8.0
     :end-before: .. rubric

* :issue:`2774`: Promoted to official pgr_findCloseEdges in version 3.8

  .. include:: pgr_findCloseEdges.rst
     :start-after: Version 3.8.0
     :end-before: .. rubric

* :issue:`2873`: Promoted to official pgr_separateCrossing in version 3.8

  .. include:: pgr_separateCrossing.rst
     :start-after: Version 3.8.0
     :end-before: Description

* :issue:`2874`: Promoted to official pgr_separateTouching in version 3.8

  .. include:: pgr_separateTouching.rst
     :start-after: Version 3.8.0
     :end-before: Description

.. rubric:: Proposed functions

.. rubric:: Contraction

* :issue:`2790`: pgr_contractionDeadEnd new contraction function
* :issue:`2791`: pgr_contractionLinear new contraction function
* :issue:`2536`: Support for contraction hierarchies (pgr_contractionHierarchies)

.. rubric:: Utilities

* :issue:`2848`: Create pgr_separateCrossing new utility function
* :issue:`2849`: Create of pgr_separateTouching new utility function

.. rubric:: Official functions changes

* :issue:`2786`: pgr_contraction(edges) new signature

  .. include:: pgr_contraction.rst
     :start-after: Version 3.8.0
     :end-before: .. rubric

.. rubric:: C/C++ code enhancements

* :issue:`2802`: Code reorganization on pgr_contraction
* Other enhancements:

  * :issue:`2869`:

.. rubric:: SQL code enhancements

* :issue:`2850`: Rewrite pgr_nodeNetwork

.. rubric:: Deprecation of SQL functions

* :issue:`2749`: Deprecate pgr_AlphaShape in 3.8
* :issue:`2750`: Deprecate pgr_CreateTopology in 3.8
* :issue:`2753`: Deprecate pgr_analyzeGraph in 3.8
* :issue:`2754`: Deprecate pgr_analyzeOneWay in 3.8
* :issue:`2826`: Deprecate pgr_createVerticesTable in 3.8

In the deprecated functions:

- Migration section is created.
- The use of the functions is removed in the documentation.

pgRouting 3.7
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.7.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.7.3`

* :issue:`2731`: Build Failure on Ubuntu 22

pgRouting 3.7.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.7.2`

.. rubric:: Build

* :issue:`2713`: cmake missing
  some policies and min version

  - Using OLD policies: CMP0148, CMP0144, CMP0167
  - Minimum cmake version 3.12

.. rubric:: Bug fixes

* :issue:`2707`: Build failure in pgRouting 3.7.1 on Alpine
* :issue:`2706`: winnie crashing on pgr_betweennessCentrality

pgRouting 3.7.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.7.1`

.. rubric:: Bug fixes

* :issue:`2680`: fails to compile under mingw64 gcc 13.2
* :issue:`2689`: When point is a vertex, the withPoints family do not return
  results.

.. rubric:: C/C++ code enhancemet

* TRSP family

pgRouting 3.7.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.7.0`

.. rubric:: Support

* :issue:`2656`: Stop support of PostgreSQL12 on pgrouting v3.7

  * Stopping support of PostgreSQL 12
  * CI does not test for PostgreSQL 12

.. rubric:: New experimental functions.

* Metrics

  * pgr_betweennessCentrality

.. rubric:: Official functions changes

* :issue:`2605`: Standardize spanning tree functions output

  * Functions:

    * ``pgr_kruskalDD``
    * ``pgr_kruskalDFS``
    * ``pgr_kruskalBFS``
    * ``pgr_primDD``
    * ``pgr_primDFS``
    * ``pgr_primBFS``

  .. include:: pgr_primDD.rst
     :start-after: Version 3.7.0
     :end-before: .. rubric

.. rubric:: Experimental promoted to proposed.

* :issue:`2635`: pgr_LineGraph ignores directed flag and use negative values for
  identifiers.

  * ``pgr_lineGraph``

  .. include:: pgr_lineGraph.rst
     :start-after: Version 3.7.0
     :end-before: Version 2.5.0

.. rubric:: Code enhancement

* :issue:`2599`: Driving distance
  cleanup
* :issue:`2607`: Read postgresql
  data on C++
* :issue:`2614`: Clang tidy does
  not work

pgRouting 3.6
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.6.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.6.3`

.. rubric:: Build

* Explicit minimum requirements:

  * postgres 11.0.0
  * postgis 3.0.0

* g++ 13+ is supported

.. rubric:: Code fixes

* Fix warnings from cpplint.
* Fix warnings from clang 18.

.. rubric:: CI tests

* Add a clang tidy test on changed files.
* Update test not done on versions: 3.0.1, 3.0.2, 3.0.3, 3.0.4, 3.1.0, 3.1.1,
  3.1.2

.. rubric:: Documentation

* Results of documentation queries adujsted to boost 1.83.0 version:

  * pgr_edgeDisjointPaths
  * pgr_stoerWagner

.. rubric:: pgtap tests

* bug fixes


pgRouting 3.6.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.6.2`

.. rubric:: Upgrade fix

* The upgrade was failing for same minor

.. rubric:: Code fixes

* Fix warnings from cpplint

.. rubric:: Others

* Adjust NEWS generator

  * Name change to `NEWS.md` for better visualization on GitHub

pgRouting 3.6.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.6.1`

* :issue:`2588`: pgrouting 3.6.0
  fails to build on OSX

pgRouting 3.6.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.6.0`

.. rubric:: Official functions changes

* :issue:`2516`: Standardize output pgr_aStar

  * Standardize output columns to |short-generic-result|

    * pgr_aStar(One to One) added ``start_vid`` and ``end_vid`` columns.
    * pgr_aStar(One to Many) added ``end_vid`` column.
    * pgr_aStar(Many to One) added ``start_vid`` column.

* :issue:`2523`: Standardize output pgr_bdAstar

  * Standardize output columns to |short-generic-result|

    * pgr_bdAstar(One to One) added ``start_vid`` and ``end_vid`` columns.
    * pgr_bdAstar(One to Many) added ``end_vid`` column.
    * pgr_bdAstar(Many to One) added ``start_vid`` column.

* :issue:`2547`: Standardize output and modifying signature pgr_KSP

  .. include:: pgr_KSP.rst
     :start-after: Version 3.6.0
     :end-before: .. rubric

* :issue:`2548`: Standardize output pgr_drivingDistance

  .. include:: pgr_drivingDistance.rst
     :start-after: Version 3.6.0
     :end-before: .. rubric

.. rubric:: Proposed functions changes

* :issue:`2544`: Standardize output and modifying signature pgr_withPointsDD

  .. include:: pgr_withPointsDD.rst
     :start-after: Version 3.6.0
     :end-before: .. rubric

* :issue:`2546`: Standardize output and modifying signature pgr_withPointsKSP

  .. include:: pgr_withPointsKSP.rst
     :start-after: Version 3.6.0
     :end-before: .. rubric

.. rubric:: C/C++ code enhancements

* :issue:`2504`: To C++ pg data get,
  fetch and check.

  * Stopping support for compilation with MSVC.

* :issue:`2505`: Using namespace.
* :issue:`2512`: [Dijkstra] Removing duplicate code on Dijkstra.
* :issue:`2517`: Astar code simplification.
* :issue:`2521`: Dijkstra code simplification.
* :issue:`2522`: bdAstar code simplification.

.. rubric:: Documentation

* :issue:`2490`: Automatic page
  history links.

* ..rubric:: Standardize SQL

* :issue:`2555`: Standardize
  deprecated messages
* On new internal function: do not use named parameters and default parameters.

pgRouting 3.5
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.5.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.5.1`

.. rubric:: Documentation fixes

Changes on the documentation to the following:

* pgr_degree
* pgr_dijkstra
* pgr_ksp
* Automatic page history links

  * using bootstrap_version 2 because 3+ does not do dropdowns

.. rubric:: Issue fixes

* :issue:`2565`: pgr_lengauerTarjanDominatorTree triggers an assertion

.. rubric:: SQL enhancements

* :issue:`2561`: Not use wildcards on SQL

.. rubric:: pgtap tests

* :issue:`2559`: pgtap test using sampledata

.. rubric:: Build fixes

* Fix winnie build

.. rubric:: Code fixes

* Fix clang warnings

  * Grouping headers of postgres readers

pgRouting 3.5.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.5.0`

.. rubric:: Official functions changes

* Dijkstra

  * Standardize output columns to |short-generic-result|

    * pgr_dijkstra(One to One) added ``start_vid`` and ``end_vid`` columns.
    * pgr_dijkstra(One to Many) added ``end_vid`` column.
    * pgr_dijkstra(Many to One) added ``start_vid`` column.

pgRouting 3.4
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.4.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.4.2`

.. rubric:: Issue fixes

* :issue:`2394`: pgr_bdAstar accumulates heuristic cost in visited node cost.
* :issue:`2427`: pgr_createVerticesTable & pgr_createTopology, variable should be of type Record.

pgRouting 3.4.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.4.1`

.. rubric:: Issue fixes

* :issue:`2401`: pgRouting 3.4.0 do not build docs when sphinx is too low or missing
* :issue:`2398`: v3.4.0 does not upgrade from 3.3.3

pgRouting 3.4.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.4.0`

.. rubric:: Issue fixes

* :issue:`1891`: pgr_ksp doesn't give all correct shortest path

.. rubric:: New proposed functions.

* With points

  * pgr_withPointsVia(One Via)

* Turn Restrictions

  * Via with turn restrictions

    * pgr_trspVia(One Via)
    * pgr_trspVia_withPoints(One Via)

  * pgr_trsp

    * pgr_trsp(One to One)
    * pgr_trsp(One to Many)
    * pgr_trsp(Many to One)
    * pgr_trsp(Many to Many)
    * pgr_trsp(Combinations)

  * ``pgr_trsp_withPoints``

    * pgr_trsp_withPoints(One to One)
    * pgr_trsp_withPoints(One to Many)
    * pgr_trsp_withPoints(Many to One)
    * pgr_trsp_withPoints(Many to Many)
    * pgr_trsp_withPoints(Combinations)

* Topology

  * pgr_degree

* Utilities

  * pgr_findCloseEdges(One point)
  * pgr_findCloseEdges(Many points)

.. rubric:: New experimental functions.

* Ordering

  * pgr_cuthillMckeeOrdering

* Unclassified

  * pgr_hawickCircuits

.. rubric:: Official functions changes

* Flow functions

  * pgr_maxCardinalityMatch(text)

    * Deprecating: pgr_maxCardinalityMatch(text,boolean)

.. rubric:: Deprecated Functions

* Turn Restrictions

  * pgr_trsp(text,integer,integer,boolean,boolean,text)
  * pgr_trsp(text,integer,float8,integer,float8,boolean,boolean,text)
  * pgr_trspViaVertices(text,anyarray,boolean,boolean,text)
  * pgr_trspViaEdges(text,integer[],float[],boolean,boolean,text)

pgRouting 3.3
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.3.5 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.3.5`

.. rubric:: Documentation

* :issue:`2401`: pgRouting 3.4.0 do not build docs when sphinx is too low or
  missing

pgRouting 3.3.4 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.3.4`

.. rubric:: Issue fixes

* :issue:`2400`: pgRouting 3.3.3 does not build in focal

pgRouting 3.3.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.3.3`

.. rubric:: Issue fixes

* :issue:`1891`: pgr_ksp doesn't give all correct shortest path

.. rubric:: Official functions changes

* Flow functions

  * pgr_maxCardinalityMatch(text,boolean)

    * Ignoring optional boolean parameter, as the algorithm works only for
      undirected graphs.


pgRouting 3.3.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.3.2`

* Revised documentation

  * Simplifying table names and table columns, for example:

    * ``edges`` instead of ``edge_table``

      * Removing unused columns ``category_id`` and ``reverse_category_id``.

    * ``combinations`` instead of ``combinations_table``

     * Using PostGIS standard for geometry column.

       * ``geom`` instead of ``the_geom``

  * Avoiding usage of functions that modify indexes, columns etc on tables.

    * Using ``pgr_extractVertices`` to create a routing topology

  * Restructure of the pgRouting concepts page.


.. rubric:: Issue fixes

* :issue:`2276`: edgeDisjointPaths issues with start_vid and combinations
* :issue:`2312`: pgr_extractVertices error when target is not BIGINT
* :issue:`2357`: Apply clang-tidy performance-*

pgRouting 3.3.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.3.1`

.. rubric:: Issue fixes

* :issue:`2216`: Warnings when using clang
* :issue:`2266`: Error processing restrictions


pgRouting 3.3.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.3.0`

.. rubric:: Issue fixes

* :issue:`2057`: trspViaEdges columns in different order
* :issue:`2087`: pgr_extractVertices to proposed
* :issue:`2201`: pgr_depthFirstSearch to proposed
* :issue:`2202`: pgr_sequentialVertexColoring to proposed
* :issue:`2203`: pgr_dijkstraNear and pgr_dijkstraNearCost to proposed

.. rubric:: New experimental functions.

* Coloring

  * pgr_edgeColoring

.. rubric:: Experimental promoted to Proposed

* Dijkstra

  * pgr_dijkstraNear

    * pgr_dijkstraNear(Combinations)
    * pgr_dijkstraNear(Many to Many)
    * pgr_dijkstraNear(Many to One)
    * pgr_dijkstraNear(One to Many)

  * pgr_dijkstraNearCost

    * pgr_dijkstraNearCost(Combinations)
    * pgr_dijkstraNearCost(Many to Many)
    * pgr_dijkstraNearCost(Many to One)
    * pgr_dijkstraNearCost(One to Many)

* Coloring

  * pgr_sequentialVertexColoring

* Topology

  * pgr_extractVertices

* Traversal

  * pgr_depthFirstSearch(Multiple vertices)
  * pgr_depthFirstSearch(Single vertex)

pgRouting 3.2
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.2.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.2.2`

.. rubric:: Issue fixes

* :issue:`2093`: Compilation on Visual Studio
* :issue:`2189`: Build error on RHEL 7

pgRouting 3.2.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.2.1`

.. rubric:: Issue fixes

* :issue:`1883`: pgr_TSPEuclidean crashes connection on Windows

  * The solution is to use Boost::graph::metric_tsp_approx
  * To not break user's code the optional parameters related to the TSP
    Annaeling are ignored
  * The function with the annaeling optional parameters is deprecated


pgRouting 3.2.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.2.0`

.. rubric:: Build

* :issue:`1850`: Change Boost
  min version to 1.56

  * Removing support for Boost v1.53, v1.54 & v1.55

.. rubric:: New experimental functions.

* pgr_bellmanFord(Combinations)
* pgr_binaryBreadthFirstSearch(Combinations)
* pgr_bipartite
* pgr_dagShortestPath(Combinations)
* pgr_depthFirstSearch
* Dijkstra Near

  * pgr_dijkstraNear

    * pgr_dijkstraNear(One to Many)
    * pgr_dijkstraNear(Many to One)
    * pgr_dijkstraNear(Many to Many)
    * pgr_dijkstraNear(Combinations)

  * pgr_dijkstraNearCost

    * pgr_dijkstraNearCost(One to Many)
    * pgr_dijkstraNearCost(Many to One)
    * pgr_dijkstraNearCost(Many to Many)
    * pgr_dijkstraNearCost(Combinations)

* pgr_edwardMoore(Combinations)
* pgr_isPlanar
* pgr_lengauerTarjanDominatorTree
* pgr_makeConnected
* Flow

  * pgr_maxFlowMinCost(Combinations)
  * pgr_maxFlowMinCost_Cost(Combinations)

* pgr_sequentialVertexColoring

.. rubric:: New proposed functions.

* Astar

  * pgr_aStar(Combinations)
  * pgr_aStarCost(Combinations)

* Bidirectional Astar

  * pgr_bdAstar(Combinations)
  * pgr_bdAstarCost(Combinations)

* Bidirectional Dijkstra

  * pgr_bdDijkstra(Combinations)
  * pgr_bdDijkstraCost(Combinations)

* Flow

  * pgr_boykovKolmogorov(Combinations)
  * pgr_edgeDisjointPaths(Combinations)
  * pgr_edmondsKarp(Combinations)
  * pgr_maxFlow(Combinations)
  * pgr_pushRelabel(Combinations)

* pgr_withPoints(Combinations)
* pgr_withPointsCost(Combinations)

pgRouting 3.1
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.1.4 Release Notes
--------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.1.4`

.. rubric:: Issues fixes

* :issue:`2189`: Build error on
  RHEL 7


pgRouting 3.1.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.1.3`

.. rubric:: Issues fixes

* :issue:`1825`: Boost versions are not honored
* :issue:`1849`: Boost 1.75.0 geometry "point_xy.hpp" build error on macOS
  environment
* :issue:`1861`: vrp functions crash server


pgRouting 3.1.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.1.2`

.. rubric:: Issues fixes

* :issue:`1304`: FreeBSD 12
  64-bit crashes on pgr_vrOneDepot tests Experimental Function
* :issue:`1356`: tools/testers/pg_prove_tests.sh fails when PostgreSQL port is not passed
* :issue:`1725`: Server crash
  on pgr_pickDeliver and pgr_vrpOneDepot on openbsd
* :issue:`1760`: TSP server
  crash on ubuntu 20.04 #1760
* :issue:`1770`: Remove
  warnings when using clang compiler


pgRouting 3.1.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.1.1`

.. rubric:: Issues fixes

* :issue:`1733`: pgr_bdAstar fails when source or target vertex does not exist in the graph
* :issue:`1647`: Linear Contraction contracts self loops
* :issue:`1640`: pgr_withPoints fails when points_sql is empty
* :issue:`1616`: Path evaluation on C++ not updated before the results go back to C
* :issue:`1300`: pgr_chinesePostman crash on test data

pgRouting 3.1.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.1.0`

.. rubric:: New proposed functions.

* pgr_dijkstra(combinations)
* pgr_dijkstraCost(combinations)

.. rubric:: Build changes

* Minimal requirement for Sphinx: version 1.8

pgRouting 3.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 3.0.6 Release Notes
--------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.0.6`

.. rubric:: Issues fixes

* :issue:`2189`: Build error on RHEL 7


pgRouting 3.0.5 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.0.5`

.. rubric:: Backport issue fixes

* :issue:`1825`: Boost versions are not honored
* :issue:`1849`: Boost 1.75.0 geometry "point_xy.hpp" build error on macOS environment
* :issue:`1861`: vrp functions crash server


pgRouting 3.0.4 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.0.4`

.. rubric:: Backport issue fixes

* :issue:`1304`: FreeBSD 12 64-bit crashes on pgr_vrOneDepot tests Experimental Function
* :issue:`1356`: tools/testers/pg_prove_tests.sh fails when PostgreSQL port is not passed
* :issue:`1725`: Server crash on pgr_pickDeliver and pgr_vrpOneDepot on openbsd
* :issue:`1760`: TSP server crash on ubuntu 20.04 #1760
* :issue:`1770`: Remove warnings when using clang compiler



pgRouting 3.0.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.0.3`

.. rubric:: Backport issue fixes

* :issue:`1733`: pgr_bdAstar fails when source or target vertex does not exist in the graph
* :issue:`1647`: Linear Contraction contracts self loops
* :issue:`1640`: pgr_withPoints fails when points_sql is empty
* :issue:`1616`: Path evaluation on C++ not updated before the results go back to C
* :issue:`1300`: pgr_chinesePostman crash on test data



pgRouting 3.0.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.0.2`

.. rubric:: Issues fixes

* :issue:`1378`: Visual Studio
  build failing


pgRouting 3.0.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.0.1`

.. rubric:: Issues fixes

* :issue:`232`: Honor client
  cancel requests in C /C++ code


pgRouting 3.0.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`3.0.0`

.. rubric:: Fixed Issues

* :issue:`1153`: Renamed pgr_eucledianTSP to pgr_TSPeuclidean
* :issue:`1188`: Removed CGAL dependency
* :issue:`1002`: Fixed contraction issues:

  * :issue:`1004`: Contracts when forbidden vertices do not belong to graph
  * :issue:`1005`: Intermideate results eliminated
  * :issue:`1006`: No loss of information

.. rubric:: New Functions

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

  * pgr_aStar(One to Many)
  * pgr_aStar(Many to One)
  * pgr_aStar(Many to Many)
  * pgr_aStarCost(One to One)
  * pgr_aStarCost(One to Many)
  * pgr_aStarCost(Many to One)
  * pgr_aStarCost(Many to Many)
  * pgr_aStarCostMatrix

* bdAstar Family

  * pgr_bdAstar(One to Many)
  * pgr_bdAstar(Many to One)
  * pgr_bdAstar(Many to Many)
  * pgr_bdAstarCost(One to One)
  * pgr_bdAstarCost(One to Many)
  * pgr_bdAstarCost(Many to One)
  * pgr_bdAstarCost(Many to Many)
  * pgr_bdAstarCostMatrix

* bdDijkstra Family

  * pgr_bdDijkstra(One to Many)
  * pgr_bdDijkstra(Many to One)
  * pgr_bdDijkstra(Many to Many)
  * pgr_bdDijkstraCost(One to One)
  * pgr_bdDijkstraCost(One to Many)
  * pgr_bdDijkstraCost(Many to One)
  * pgr_bdDijkstraCost(Many to Many)
  * pgr_bdDijkstraCostMatrix

* Flow Family

  * pgr_pushRelabel(One to One)
  * pgr_pushRelabel(One to Many)
  * pgr_pushRelabel(Many to One)
  * pgr_pushRelabel(Many to Many)
  * pgr_edmondsKarp(One to One)
  * pgr_edmondsKarp(One to Many)
  * pgr_edmondsKarp(Many to One)
  * pgr_edmondsKarp(Many to Many)
  * pgr_boykovKolmogorov (One to One)
  * pgr_boykovKolmogorov (One to Many)
  * pgr_boykovKolmogorov (Many to One)
  * pgr_boykovKolmogorov (Many to Many)
  * pgr_maxCardinalityMatching
  * pgr_maxFlow
  * pgr_edgeDisjointPaths(One to One)
  * pgr_edgeDisjointPaths(One to Many)
  * pgr_edgeDisjointPaths(Many to One)
  * pgr_edgeDisjointPaths(Many to Many)

* Components family

  * pgr_connectedComponents
  * pgr_strongComponents
  * pgr_biconnectedComponents
  * pgr_articulationPoints
  * pgr_bridges

* Contraction:

  * Removed unnecessary column seq
  * Bug Fixes


.. rubric:: New experimental functions.

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

  * pgr_labelGraph - Use the components family of functions instead.
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

pgRouting 2
*******************************************************************************

.. contents:: Minors 2.x
   :local:
   :depth: 1

pgRouting 2.6
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 2.6.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.6.3`

.. rubric:: Bug fixes

* :issue:`1219`: Implicit cast for via_path integer to text
* :issue:`1193`: Fixed pgr_pointsAsPolygon breaking when comparing strings in
  WHERE clause
* :issue:`1185`: Improve FindPostgreSQL.cmake

pgRouting 2.6.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.6.2`

.. rubric:: Bug fixes

* :issue:`1152`: Fixes driving distance when vertex is not part of the graph
* :issue:`1098`: Fixes windows test
* :issue:`1165`: Fixes build for python3 and perl5


pgRouting 2.6.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.6.1`

* Fixes server crash on several functions.

  * pgr_floydWarshall
  * pgr_johnson
  * pgr_aStar
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


pgRouting 2.6.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.6.0`

.. rubric:: New experimental functions.

* pgr_lineGraphFull

.. rubric:: Bug fixes

* Fix pgr_trsp(text,integer,double precision,integer,double
  precision,boolean,boolean[,text])

  * without restrictions

    * calls pgr_dijkstra when both end points have a fraction IN (0,1)
    * calls pgr_withPoints when at least one fraction NOT IN (0,1)

  * with restrictions

    * calls original trsp code

.. rubric:: Internal code

* Cleaned the internal code of trsp(text,integer,integer,boolean,boolean [,
  text])

  * Removed the use of pointers
  * Internal code can accept BIGINT

* Cleaned the internal code of withPoints

pgRouting 2.5
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 2.5.5 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.5.5`

.. rubric:: Bug fixes

* Fixes driving distance when vertex is not part of the graph
* Fixes windows test
* Fixes build for python3 and perl5


pgRouting 2.5.4 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.5.4`

* Fixes server crash on several functions.

  * pgr_floydWarshall
  * pgr_johnson
  * pgr_aStar
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


pgRouting 2.5.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.5.3`

.. rubric:: Bug fixes

* Fix for postgresql 11: Removed a compilation error when compiling with
  postgreSQL


pgRouting 2.5.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.5.2`

.. rubric:: Bug fixes

* Fix for postgresql 10.1: Removed a compiler condition

pgRouting 2.5.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.5.1`

.. rubric:: Bug fixes

* Fixed prerequisite minimum version of: cmake

pgRouting 2.5.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.5.0`

.. rubric:: enhancement:

* pgr_version is now on SQL language

.. rubric:: Breaking change on:

* pgr_edgeDisjointPaths:

  * Added path_id, cost and agg_cost columns on the result
  * Parameter names changed
  * The many version results are the union of the One to One version

.. rubric:: New Signatures

* pgr_bdAstar(One to One)

.. rubric:: New proposed functions.

* pgr_bdAstar(One to Many)
* pgr_bdAstar(Many to One)
* pgr_bdAstar(Many to Many)
* pgr_bdAstarCost(One to One)
* pgr_bdAstarCost(One to Many)
* pgr_bdAstarCost(Many to One)
* pgr_bdAstarCost(Many to Many)
* pgr_bdAstarCostMatrix
* pgr_bdDijkstra(One to Many)
* pgr_bdDijkstra(Many to One)
* pgr_bdDijkstra(Many to Many)
* pgr_bdDijkstraCost(One to One)
* pgr_bdDijkstraCost(One to Many)
* pgr_bdDijkstraCost(Many to One)
* pgr_bdDijkstraCost(Many to Many)
* pgr_bdDijkstraCostMatrix
* pgr_lineGraph
* pgr_lineGraphFull
* pgr_connectedComponents
* pgr_strongComponents
* pgr_biconnectedComponents
* pgr_articulationPoints
* pgr_bridges

.. rubric:: Deprecated signatures

* pgr_bdastar - use pgr_bdAstar instead

.. rubric:: Renamed functions

* pgr_maxFlowPushRelabel - use pgr_pushRelabel instead
* pgr_maxFlowEdmondsKarp -use pgr_edmondsKarp instead
* pgr_maxFlowBoykovKolmogorov - use pgr_boykovKolmogorov instead
* pgr_maximumCardinalityMatching - use pgr_maxCardinalityMatch instead

.. rubric:: Deprecated Function

* pgr_pointToEdgeNode


pgRouting 2.4
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 2.4.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.4.2`

.. rubric:: Improvement

* Works for postgreSQL 10

.. rubric:: Bug fixes

* Fixed: Unexpected error column "cname"
* Replace __linux__ with __GLIBC__ for glibc-specific headers and functions




pgRouting 2.4.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.4.1`

.. rubric:: Bug fixes

* Fixed compiling error on macOS
* Condition error on pgr_withPoints


pgRouting 2.4.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.4.0`

.. rubric:: New Functions

* pgr_bdDijkstra


.. rubric:: New proposed signatures:

* pgr_maxFlow
* pgr_aStar(One to Many)
* pgr_aStar(Many to One)
* pgr_aStar(Many to Many)
* pgr_aStarCost(One to One)
* pgr_aStarCost(One to Many)
* pgr_aStarCost(Many to One)
* pgr_aStarCost(Many to Many)
* pgr_aStarCostMatrix

.. rubric:: Deprecated signatures.

* pgr_bddijkstra - use pgr_bdDijkstra instead

.. rubric:: Deprecated Functions

* pgr_pointsToVids

.. rubric:: Bug fixes

* Bug fixes on proposed functions

  * pgr_withPointsKSP: fixed ordering

* TRSP original code is used with no changes on the compilation warnings

pgRouting 2.3
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

pgRouting 2.3.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.3.2`

.. rubric:: Bug Fixes

* Fixed pgr_gsoc_vrppdtw crash when all orders fit on one truck.
* Fixed pgr_trsp:

  * Alternate code is not executed when the point is in reality a vertex
  * Fixed ambiguity on seq



pgRouting 2.3.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.3.1`

.. rubric:: Bug Fixes

* Leaks on proposed max_flow functions
* Regression error on pgr_trsp
* Types discrepancy on pgr_createVerticesTable



pgRouting 2.3.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.3.0`

.. rubric:: New Signatures

* pgr_TSP
* pgr_aStar

.. rubric:: New Functions

* pgr_eucledianTSP


.. rubric:: New proposed functions.

* pgr_dijkstraCostMatrix
* pgr_withPointsCostMatrix
* pgr_maxFlowPushRelabel(One to One)
* pgr_maxFlowPushRelabel(One to Many)
* pgr_maxFlowPushRelabel(Many to One)
* pgr_maxFlowPushRelabel(Many to Many)
* pgr_maxFlowEdmondsKarp(One to One)
* pgr_maxFlowEdmondsKarp(One to Many)
* pgr_maxFlowEdmondsKarp(Many to One)
* pgr_maxFlowEdmondsKarp(Many to Many)
* pgr_maxFlowBoykovKolmogorov (One to One)
* pgr_maxFlowBoykovKolmogorov (One to Many)
* pgr_maxFlowBoykovKolmogorov (Many to One)
* pgr_maxFlowBoykovKolmogorov (Many to Many)
* pgr_maximumCardinalityMatching
* pgr_edgeDisjointPaths(One to One)
* pgr_edgeDisjointPaths(One to Many)
* pgr_edgeDisjointPaths(Many to One)
* pgr_edgeDisjointPaths(Many to Many)
* pgr_contractGraph


.. rubric:: Deprecated signatures

* pgr_tsp - use pgr_TSP or pgr_eucledianTSP instead
* pgr_aStar - use pgr_aStar instead


.. rubric:: Deprecated Functions

* pgr_flip_edges
* pgr_vidsToDmatrix
* pgr_pointsToDMatrix
* pgr_textToPoints

pgRouting 2.2
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 2.2.4 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.2.4`

.. rubric:: Bug Fixes

* Bogus uses of extern "C"
* Build error on Fedora 24 + GCC 6.0
* Regression error pgr_nodeNetwork


pgRouting 2.2.3 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.2.3`

.. rubric:: Bug Fixes

* Fixed compatibility issues with PostgreSQL 9.6.


pgRouting 2.2.2 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.2.2`

.. rubric:: Bug Fixes

* Fixed regression error on pgr_drivingDistance



pgRouting 2.2.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.2.1`

.. rubric:: Bug Fixes

* Server crash fix on pgr_alphaShape
* Bug fix on With Points family of functions

pgRouting 2.2.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.2.0`

.. rubric:: Improvements

- pgr_nodeNetwork

  - Adding a row_where and outall optional parameters

- Signature fix

  - pgr_dijkstra -- to match what is documented


.. rubric:: New Functions

- pgr_floydWarshall
- pgr_Johnson
- pgr_dijkstraCost(One to One)
- pgr_dijkstraCost(One to Many)
- pgr_dijkstraCost(Many to One)
- pgr_dijkstraCost(Many to Many)

.. rubric:: Proposed Functionality

- pgr_withPoints(One to One)
- pgr_withPoints(One to Many)
- pgr_withPoints(Many to One)
- pgr_withPoints(Many to Many)
- pgr_withPointsCost(One to One)
- pgr_withPointsCost(One to Many)
- pgr_withPointsCost(Many to One)
- pgr_withPointsCost(Many to Many)
- pgr_withPointsDD(single vertex)
- pgr_withPointsDD(multiple vertices)
- pgr_withPointsKSP
- pgr_dijkstraVia


.. rubric:: Deprecated Functions

- pgr_apspWarshall use pgr_floydWarshall instead
- pgr_apspJohnson use pgr_Johnson instead
- pgr_kDijkstraCost use pgr_dijkstraCost instead
- pgr_kDijkstraPath use pgr_dijkstra instead

.. rubric:: Renamed and Deprecated Function

- pgr_makeDistanceMatrix renamed to _pgr_makeDistanceMatrix

pgRouting 2.1
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 2.1.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.1.0`

.. rubric:: New Signatures

- pgr_dijkstra(One to Many)
- pgr_dijkstra(Many to One)
- pgr_dijkstra(Many to Many)
- pgr_drivingDistance(multiple vertices)

.. rubric:: Refactored

- pgr_dijkstra(One to One)
- pgr_ksp
- pgr_drivingDistance(single vertex)

.. rubric:: Improvements

- pgr_alphaShape function now can generate better (multi)polygon with holes and
  alpha parameter.

.. rubric:: Proposed Functionality

- Proposed functions from Steve Woodbridge, (Classified as Convenience by the
  author.)

  - pgr_pointToEdgeNode - convert a point geometry to a vertex_id based on
    closest edge.
  - pgr_flipEdges - flip the edges in an array of geometries so the connect end
    to end.
  - pgr_textToPoints - convert a string of x,y;x,y;... locations into point
    geometries.
  - pgr_pointsToVids - convert an array of point geometries into vertex ids.
  - pgr_pointsToDMatrix - Create a distance matrix from an array of points.
  - pgr_vidsToDMatrix - Create a distance matrix from an array of vertix_id.
  - pgr_vidsToDMatrix - Create a distance matrix from an array of vertix_id.

- Added proposed functions from GSoc Projects:

  - pgr_vrppdtw
  - pgr_vrponedepot

.. rubric:: Deprecated Functions

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


pgRouting 2.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

pgRouting 2.0.1 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.0.1`

Minor bug fixes.

.. rubric:: Bug Fixes

* No track of the bug fixes were kept.

pgRouting 2.0.0 Release Notes
-------------------------------------------------------------------------------

To see all issues & pull requests closed by this release see the
:milestone:`2.0.0`

With the release of pgRouting 2.0.0 the library has abandoned backwards
compatibility to `pgRouting 1.0`_ releases.
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

* Graph Analytics - tools for detecting and fixing connection some problems in a
  graph
* A collection of useful utility functions
* Two new All Pairs Short Path algorithms (pgr_apspJohnson, pgr_apspWarshall)
* Bi-directional Dijkstra and A-star search algorithms (pgr_bdAstar,
  pgr_bdDijkstra)
* One to many nodes search (pgr_kDijkstra)
* K alternate paths shortest path (pgr_ksp)
* New TSP solver that simplifies the code and the build process (pgr_tsp),
  dropped "Gaul Library" dependency
* Turn Restricted shortest path (pgr_trsp) that replaces Shooting Star
* Dropped support for Shooting Star
* Built a test infrastructure that is run before major code changes are checked
  in
* Tested and fixed most all of the outstanding bugs reported against 1.x that
  existing in the 2.0-dev code base.
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

pgRouting 1
*******************************************************************************

pgRouting 1.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. contents:: Contents
   :local:
   :depth: 1

To see the issues closed by this release see the `Git closed issues for 1.x
<https://github.com/pgRouting/pgrouting/issues?q=milestone%3A%22Release+1.x%22+is%3Aclosed>`_
on Github.
The following release notes have been copied from the previous ``RELEASE_NOTES``
file and are kept as a reference.


Changes for release 1.05
-------------------------------------------------------------------------------

* Bug fixes


Changes for release 1.03
-------------------------------------------------------------------------------

* Much faster topology creation
* Bug fixes


Changes for release 1.02
-------------------------------------------------------------------------------

* Shooting* bug fixes
* Compilation problems solved

Changes for release 1.01
-------------------------------------------------------------------------------

* Shooting* bug fixes


Changes for release 1.0
-------------------------------------------------------------------------------

* Core and extra functions are separated
* Cmake build process
* Bug fixes


Changes for release 1.0.0b
-------------------------------------------------------------------------------

* Additional SQL file with more simple names for wrapper functions
* Bug fixes


Changes for release 1.0.0a
-------------------------------------------------------------------------------

* Shooting* shortest path algorithm for real road networks
* Several SQL bugs were fixed


Changes for release 0.9.9
-------------------------------------------------------------------------------

* PostgreSQL 8.2 support
* Shortest path functions return empty result if they could not find any path


Changes for release 0.9.8
-------------------------------------------------------------------------------

* Renumbering scheme was added to shortest path functions
* Directed shortest path functions were added
* routing_postgis.sql was modified to use dijkstra in TSP search
