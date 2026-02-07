To read all history of releases go to the latest [release notes](https://docs.pgrouting.org/latest/en/release_notes.html)

# pgRouting 4


## pgRouting 4.1



### pgRouting 4.1.0 Release Notes

To see all issues & pull requests closed by this release see the
[#4.1.0](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%204.1.0%22)

**Code enhancements**

* [#3049](https://github.com/pgRouting/pgrouting/issues/3049): Use enumeration on drivers and process.
* [#3054](https://github.com/pgRouting/pgrouting/issues/3054): Change error message when edges_sql is empty
* [#3055](https://github.com/pgRouting/pgrouting/issues/3055): bdDijkstra: use the shortest_path process and driver
* [#3056](https://github.com/pgRouting/pgrouting/issues/3056): edwardMoore: use the shortest_path process and driver
* [#3060](https://github.com/pgRouting/pgrouting/issues/3060): dagShortestPath: use the shortest_path process and driver
* [#3064](https://github.com/pgRouting/pgrouting/issues/3064): Astar: create an use a process & driverAstar

## pgRouting 4.0


<details> <summary>pgRouting 4.0.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the
[#4.0.1](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%204.0.1%22)

**Bug Fixes**

* [#2966](https://github.com/pgRouting/pgrouting/issues/2966): pgr_withPoints does not pick optimal route when fraction = 1
* [#3034](https://github.com/pgRouting/pgrouting/issues/3034): metrics driver should not be using new

**Code enhancements**

* [#3044](https://github.com/pgRouting/pgrouting/issues/3044): Check and fix assert.hpp for cppcoreguidelines-explicit-virtual-functions
* [#3040](https://github.com/pgRouting/pgrouting/issues/3040): Check and fix cppcoreguidelines-virtual-class-destructor
* [#3039](https://github.com/pgRouting/pgrouting/issues/3039): Check and fix cppcoreguidelines-special-member-functions
* [#3038](https://github.com/pgRouting/pgrouting/issues/3038): Check and fix cppcoreguidelines-prefer-member-initializer
* [#3037](https://github.com/pgRouting/pgrouting/issues/3037): Check and fix cppcoreguidelines-init-variables
* [#3035](https://github.com/pgRouting/pgrouting/issues/3035): Pass by reference on existing drivers

Administration

* [#3031](https://github.com/pgRouting/pgrouting/issues/3031): pgtap license to Creative commons and License texts
  standardization

</details>

<details> <summary>pgRouting 4.0.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the
[#4.0.0](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%204.0.0%22)


Build

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

* Documentation output location: [`build/doc/_build/](format)`

  * For example: for HTML output is on `build/doc/_build/html` directory

Developers' Documentation is not built when

* Doxygen is not found
* To not build Doxygen documentation: `-DBUILD_DOXY=OFF`

Summary of changes by function

* pgr_aStar

  * Combinations signature promoted to official.

* pgr_aStarCost

  * Combinations signature promoted to official.

* pgr_bandwidth

  * New experimental function.

* pgr_bdAstar

  * Combinations signature promoted to official.

* pgr_bdAstarCost

  * Combinations signature promoted to official.

* pgr_bdDijkstra

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
  * Combinations signature promoted to official.

* pgr_bdDijkstraCost

  * Combinations signature promoted to official.

* pgr_bellmanFord

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_binaryBreadthFirstSearch

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_bipartite

  * Output columns standardized to ``(node, color)``

* pgr_boykovKolmogorov

  * Combinations signature promoted to official.

* pgr_breadthFirstSearch

  * Standardizing output columns to ``(seq, depth, start_vid, pred, node, edge, cost, agg_cost)``

* pgr_contraction

  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_contraction(text,bigint[],integer,bigint[],boolean)

* pgr_dagShortestPath

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_depthFirstSearch

  * Standardizing output columns to ``(seq, depth, start_vid, pred, node, edge, cost, agg_cost)``

* pgr_dijkstra

  * Combinations signature promoted to official.

* pgr_dijkstraCost

  * Combinations signature promoted to official.

* pgr_dijkstraNear

  * Function promoted to official.

* pgr_dijkstraNearCost

  * Function promoted to official.

* pgr_dijkstraVia

  * Function promoted to official.

* pgr_edgeColoring

  * Output columns standardized to ``(edge, color)``

* pgr_edgeDisjointPaths

  * Output columns standardized to ``(seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
  * Combinations signature promoted to official.

* pgr_edmondsKarp

  * Combinations signature promoted to official.

* pgr_edwardMoore

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
  .. rubric:: Version 3.2.0
  * New experimental signature:
    * pgr_edwardMoore(Combinations)
  .. rubric:: Version 3.0.0
  * New experimental function.

* pgr_kingOrdering

  * New experimental function.

* pgr_KSP

  * All signatures promoted to official.

* pgr_maxCardinalityMatch

  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_maxCardinalityMatch(text,boolean)

* pgr_maxFlow

  * Combinations signature promoted to official.

* pgr_pushRelabel

  * Combinations signature promoted to official.

* pgr_sequentialVertexColoring

  * Output columns standardized to ``(node, color)``

* pgr_sloanOrdering

  * New experimental function.

* pgr_topologicalSort

  * Standardize output to ``(seq, node)``

* pgr_transitiveClosure

  * Standardized output to ``(node,targets)``

* pgr_trsp

  * Function promoted to official.
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_trsp(text,integer,integer,boolean,boolean,text)
    * pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)

* pgr_trspVia

  * Function promoted to official.
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_trspviavertices(text,anyarray,boolean,boolean,text)

* pgr_trspVia_withPoints

  * Function promoted to official.
  * **Driving side** parameter is positional unnamed.
    * Valid values depend on kind of graph
  * Breaking change, signatures no longer available:
    * pgr_trspvia_withpoints(text,text,text,anyarray,boolean,boolean,boolean,character,boolean)
    * pgr_trspviaedges(text,integer[],double precision[],boolean,boolean,text)

* pgr_trsp_withPoints

  * Function promoted to official.
  * **Driving side** parameter is positional unnamed and compulsory.
    * Valid values depend on kind of graph
  * Breaking change, signatures no longer available:
    * pgr_trsp_withpoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
    * pgr_trsp_withpoints(text,text,text,anyarray,bigint,boolean,character,boolean)
    * pgr_trsp_withpoints(text,text,text,bigint,anyarray,boolean,character,boolean)
    * pgr_trsp_withpoints(text,text,text,bigint,bigint,boolean,character,boolean)
    * pgr_trsp_withpoints(text,text,text,text,boolean,character,boolean)

* pgr_TSP

  * Results change depending on input order
  * Only for undirected graphs
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double
      precision,double precision,double precision,boolean)

* pgr_TSPeuclidean

  * Results change depending on input order
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_tspeuclidean(text,bigint,bigint,double precision,integer,integer,
      integer,double precision,double precision,double precision,boolean)

* pgr_turnRestrictedPath

  * Output columns standardized to ``(seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_withPoints

  * Function promoted to official.
  * **Driving side** parameter is positional unnamed and compulsory.
    * Valid values depend on kind of graph
  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_withpoints(text,text,anyarray,anyarray,boolean,character,boolean)
    * pgr_withpoints(text,text,anyarray,bigint,boolean,character,boolean)
    * pgr_withpoints(text,text,bigint,anyarray,boolean,character,boolean)
    * pgr_withpoints(text,text,bigint,bigint,boolean,character,boolean)
    * pgr_withpoints(text,text,text,boolean,character,boolean)

* pgr_withPointsCost

  * Function promoted to official.
  * **Driving side** parameter is unnamed and compulsory.
    * Valid values depend on kind of graph
  * Output columns standardized to ``(start_vid, end_vid, agg_cost)``
  * Breaking change, signatures no longer available:
    * pgr_withpointscost(text,text,anyarray,anyarray,boolean,character)
    * pgr_withpointscost(text,text,anyarray,bigint,boolean,character)
    * pgr_withpointscost(text,text,bigint,anyarray,boolean,character)
    * pgr_withpointscost(text,text,bigint,bigint,boolean,character)
    * pgr_withpointscost(text,text,text,boolean,character)

* pgr_withPointsCostMatrix

  * Function promoted to official.
  * **Driving side** parameter is positional unnamed and compulsory.
    * Valid values depend on kind of graph
  * Output columns standardized to ``(start_vid, end_vid, agg_cost)``
  * Breaking change, signatures no longer available:
    * pgr_withpointscostmatrix(text,text,anyarray,boolean,character)

* pgr_withPointsDD

  * Function promoted to official.
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)
    * pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)

* pgr_withPointsKSP

  * Function promoted to official.
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)

* pgr_withPointsVia

  * Function promoted to official.
  * **Driving side** parameter is positional unnamed and compulsory.
    * Valid values depend on kind of graph
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_withpointsvia(text,text,anyarray,boolean,boolean,boolean,character,boolean)

Functions promoted to official

* [#2701](https://github.com/pgRouting/pgrouting/issues/2701): pgr_trsp
* [#2701](https://github.com/pgRouting/pgrouting/issues/2701): pgr_trsp
* [#2701](https://github.com/pgRouting/pgrouting/issues/2701): pgr_trspVia
* [#2701](https://github.com/pgRouting/pgrouting/issues/2701): pgr_trspVia_withPoints
* [#2701](https://github.com/pgRouting/pgrouting/issues/2701): pgr_trsp_withPoints
* [#2700](https://github.com/pgRouting/pgrouting/issues/2700): pgr_withPoints
* [#2700](https://github.com/pgRouting/pgrouting/issues/2700): pgr_withPointsCost
* [#2700](https://github.com/pgRouting/pgrouting/issues/2700): pgr_withPointsCostMatrix
* [#2700](https://github.com/pgRouting/pgrouting/issues/2700): pgr_withPointsDD
* [#2700](https://github.com/pgRouting/pgrouting/issues/2700): pgr_withPointsKSP
* [#2700](https://github.com/pgRouting/pgrouting/issues/2700): pgr_withPointsVia
* [#2983](https://github.com/pgRouting/pgrouting/issues/2983): pgr_dijkstraNear
* [#2983](https://github.com/pgRouting/pgrouting/issues/2983): pgr_dijkstraNearCost
* [#2983](https://github.com/pgRouting/pgrouting/issues/2983): pgr_dijkstraVia

Signatures promoted to official

* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_aStar(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_aStarCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_bdAstar(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_bdAstarCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_bdDijkstra(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_bdDijkstraCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_dijkstra(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_dijkstraCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_KSP(All signatures)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_boykovKolmogorov(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_edmondsKarp(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_maxFlow(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2718): pgr_pushRelabel(Combinations)

New experimental functions.

* Metrics

  * [#2951](https://github.com/pgRouting/pgrouting/issues/2951): pgr_bandwidth

* Ordering

  * [#2954](https://github.com/pgRouting/pgrouting/issues/2954): pgr_kingOrdering
  * [#2955](https://github.com/pgRouting/pgrouting/issues/2955): pgr_sloanOrdering

SQL signatures and output standardization

[#2904](https://github.com/pgRouting/pgrouting/issues/2904): Standardize output columns of functions with different output
               columns within overloads

**Standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``**

* [#2905](https://github.com/pgRouting/pgrouting/issues/2905): pgr_withPoints
* [#2906](https://github.com/pgRouting/pgrouting/issues/2906): pgr_bdDijkstra
* [#2907](https://github.com/pgRouting/pgrouting/issues/2907): pgr_bellmanFord
* [#2908](https://github.com/pgRouting/pgrouting/issues/2908): pgr_binaryBreadthFirstSearch
* [#2910](https://github.com/pgRouting/pgrouting/issues/2910): pgr_edwardMoore
* [#2913](https://github.com/pgRouting/pgrouting/issues/2913): pgr_dagShortestPath

**Standardized to ``(start_vid, end_vid, agg_cost)``**

* [#2905](https://github.com/pgRouting/pgrouting/issues/2905): pgr_withPointsCost
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905): pgr_withPointsCostMatrix

**Standardized to ``(seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``**

* [#2909](https://github.com/pgRouting/pgrouting/issues/2909): pgr_edgeDisjointPaths
* [#2909](https://github.com/pgRouting/pgrouting/issues/2909): pgr_turnRestrictedPath

**Standardized to ``(edge, color)``**

* [#2924](https://github.com/pgRouting/pgrouting/issues/2924): pgr_edgeColoring

**Standardized to ``(node, color)``**

* [#2924](https://github.com/pgRouting/pgrouting/issues/2924): pgr_bipartite
* [#2927](https://github.com/pgRouting/pgrouting/issues/2927): pgr_sequentialVertexColoring

**Standardized to ``(seq, depth, start_vid, pred, node, edge, cost, agg_cost)``**

* [#2931](https://github.com/pgRouting/pgrouting/issues/2931): pgr_breadthFirstSearch
* [#2931](https://github.com/pgRouting/pgrouting/issues/2931): pgr_depthFirstSearch

**Standardized to ``(seq, node)``**

* [#2934](https://github.com/pgRouting/pgrouting/issues/2934): pgr_topologicalSort

**Standardized to ``(node,targets)``**

* [#2934](https://github.com/pgRouting/pgrouting/issues/2934): pgr_transitiveClosure

Removal of SQL deprecated signatures

* [#2798](https://github.com/pgRouting/pgrouting/issues/2798): pgr_contraction

    * pgr_contraction(text,bigint[],integer,bigint[],boolean)

* [#2683](https://github.com/pgRouting/pgrouting/issues/2683): pgr_trsp

    * pgr_trsp(text,integer,integer,boolean,boolean,text)
    * pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)

* [#2683](https://github.com/pgRouting/pgrouting/issues/2683): pgr_trspVia

    * pgr_trspviavertices(text,anyarray,boolean,boolean,text)

* [#2700](https://github.com/pgRouting/pgrouting/issues/2700): pgr_withPointsVia

    * pgr_withpointsvia(text,text,anyarray,boolean,boolean,boolean,character,boolean)

* [#2888](https://github.com/pgRouting/pgrouting/issues/2888): pgr_findCloseEdges

  * pgr_findcloseedges(text,geometry,double precision,integer,boolean,boolean)
  * pgr_findcloseedges(text,geometry[],double precision,integer,boolean,boolean)

* [#2890](https://github.com/pgRouting/pgrouting/issues/2890): pgr_withPointsDD

    * pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)
    * pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)

* [#2895](https://github.com/pgRouting/pgrouting/issues/2895): pgr_withPointsKSP

    * pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)

* [#2899](https://github.com/pgRouting/pgrouting/issues/2899): pgr_maxCardinalityMatch

    * pgr_maxCardinalityMatch(text,boolean)

* [#2901](https://github.com/pgRouting/pgrouting/issues/2901): pgr_TSP

    * pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double
      precision,double precision,double precision,boolean)

* [#2901](https://github.com/pgRouting/pgrouting/issues/2901): pgr_TSPeuclidean

    * pgr_tspeuclidean(text,bigint,bigint,double precision,integer,integer,
      integer,double precision,double precision,double precision,boolean)


Removal of SQL deprecated functions

* [#2681](https://github.com/pgRouting/pgrouting/issues/2681): pgr_trspViaedges
* [#2682](https://github.com/pgRouting/pgrouting/issues/2682): pgr_trspViaVertices
* [#2748](https://github.com/pgRouting/pgrouting/issues/2748): pgr_alphaShape
* [#2751](https://github.com/pgRouting/pgrouting/issues/2751): pgr_createTopology
* [#2752](https://github.com/pgRouting/pgrouting/issues/2752): pgr_analyzeGraph
* [#2755](https://github.com/pgRouting/pgrouting/issues/2755): pgr_analyzeOneWay
* [#2827](https://github.com/pgRouting/pgrouting/issues/2827): pgr_createVerticesTable
* [#2886](https://github.com/pgRouting/pgrouting/issues/2886): pgr_nodeNetwork

Removal of SQL deprecated internal functions

* [#2748](https://github.com/pgRouting/pgrouting/issues/2748) _pgr_alphaShape(text,double precision)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_checkVertTab(text,text[],integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_createIndex(text,text,text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_createIndex(text,text,text,text,integer,text)
* [#2913](https://github.com/pgRouting/pgrouting/issues/2913) _pgr_dagShortestPath(text,anyarray,anyarray,boolean,boolean)
* [#2913](https://github.com/pgRouting/pgrouting/issues/2913) _pgr_dagShortestPath(text,text,boolean,boolean)
* [#2730](https://github.com/pgRouting/pgrouting/issues/2730) _pgr_dijkstraNear(text,anyarray,anyarray,bigint,boolean)
* [#2730](https://github.com/pgRouting/pgrouting/issues/2730) _pgr_dijkstraNear(text,anyarray,bigint,bigint,boolean)
* [#2730](https://github.com/pgRouting/pgrouting/issues/2730) _pgr_dijkstraNear(text,bigint,anyarray,bigint,boolean)
* [#2730](https://github.com/pgRouting/pgrouting/issues/2730) _pgr_dijkstra(text,anyarray,anyarray,boolean,boolean,boolean,bigint)
* [#2730](https://github.com/pgRouting/pgrouting/issues/2730) _pgr_dijkstra(text,anyarray,anyarray,boolean,boolean,boolean,bigint,boolean)
* [#2730](https://github.com/pgRouting/pgrouting/issues/2730) _pgr_dijkstra(text,text,boolean,boolean,bigint,boolean)
* [#2730](https://github.com/pgRouting/pgrouting/issues/2730) _pgr_dijkstra(text,text,boolean,boolean,boolean)
* [#2735](https://github.com/pgRouting/pgrouting/issues/2735) _pgr_drivingDistance(text,anyarray,double precision,boolean,boolean)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_endPoint(geometry)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) __pgr_getColumnName(text,text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) __pgr_getColumnName(text,text,text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) __pgr_getColumnType(text,text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) __pgr_getColumnType(text,text,text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) __pgr_getTableName(text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_isColumnIndexed(text,text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_isColumnIndexed(text,text,text,integer,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_isColumnIntable(text,text)
* [#2745](https://github.com/pgRouting/pgrouting/issues/2745) _pgr_kruskal(text,anyarray,text,bigint,double precision)
* [#2897](https://github.com/pgRouting/pgrouting/issues/2897) _pgr_ksp(text,anyarray,anyarray,integer,boolean,boolean,boolean)
* [#2897](https://github.com/pgRouting/pgrouting/issues/2897) _pgr_ksp(text,bigint,bigint,integer,boolean,boolean)
* [#2897](https://github.com/pgRouting/pgrouting/issues/2897) _pgr_ksp(text,text,integer,boolean,boolean)
* [#2899](https://github.com/pgRouting/pgrouting/issues/2899) _pgr_maxCardinalityMatch(text,boolean)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_msg(integer,text,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_onerror(boolean,integer,text,text,text,text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_pointtoid(geometry,double precision,text,integer)
* [#2743](https://github.com/pgRouting/pgrouting/issues/2743) _pgr_prim(text,anyarray,text,bigint,double precision)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_quote_ident(text)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_startPoint(geometry)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _pgr_trsp(text,text,anyarray,anyarray,boolean)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _pgr_trsp(text,text,anyarray,bigint,boolean)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _pgr_trsp(text,text,bigint,anyarray,boolean)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _pgr_trsp(text,text,bigint,bigint,boolean)
* [#2682](https://github.com/pgRouting/pgrouting/issues/2682) _pgr_trspViaVertices(text,integer[],boolean,boolean,text)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) _pgr_trspVia_withPoints(text,text,text,anyarray,boolean,boolean,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) _pgr_trsp_withPoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) _pgr_trsp_withPoints(text,text,text,text,boolean,character,boolean)
* [#2901](https://github.com/pgRouting/pgrouting/issues/2901) _pgr_tspEuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* [#2901](https://github.com/pgRouting/pgrouting/issues/2901) _pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* [#2861](https://github.com/pgRouting/pgrouting/issues/2861) _pgr_versionLess(text,text)
* [#2890](https://github.com/pgRouting/pgrouting/issues/2890) _pgr_withPointsDD(text,text,anyarray,double precision,boolean,character,boolean,boolean)
* [#2895](https://github.com/pgRouting/pgrouting/issues/2895) _pgr_withPointsKSP(text,text,anyarray,anyarray,integer,character,boolean,boolean,boolean,boolean)
* [#2895](https://github.com/pgRouting/pgrouting/issues/2895) _pgr_withPointsKSP(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)
* [#2895](https://github.com/pgRouting/pgrouting/issues/2895) _pgr_withPointsKSP(text,text,text,integer,character,boolean,boolean,boolean)
* [#2741](https://github.com/pgRouting/pgrouting/issues/2741) _pgr_withPointsVia(text,bigint[],double precision[],boolean)
* [#2741](https://github.com/pgRouting/pgrouting/issues/2741) _pgr_withPointsVia(text,text,anyarray,boolean,boolean,boolean,character,boolean)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _trsp(text,text,anyarray,anyarray,boolean)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _v4trsp(text,text,anyarray,anyarray,boolean)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _v4trsp(text,text,text,boolean)

Summary of functions and signatures no longer on pgrouting

* [#2748](https://github.com/pgRouting/pgrouting/issues/2748) pgr_alphashape(geometry,double precision)
* [#2752](https://github.com/pgRouting/pgrouting/issues/2752) pgr_analyzegraph(text,double precision,text,text,text,text,text)
* [#2755](https://github.com/pgRouting/pgrouting/issues/2755) pgr_analyzeoneway(text,text[],text[],text[],text[],boolean,text,text,text)
* [#2798](https://github.com/pgRouting/pgrouting/issues/2798) pgr_contraction(text,bigint[],integer,bigint[],boolean)
* [#2751](https://github.com/pgRouting/pgrouting/issues/2751) pgr_createtopology(text,double precision,text,text,text,text,text,boolean)
* [#2827](https://github.com/pgRouting/pgrouting/issues/2827) pgr_createverticestable(text,text,text,text,text)
* [#2888](https://github.com/pgRouting/pgrouting/issues/2888) pgr_findcloseedges(text,geometry,double precision,integer,boolean,boolean)
* [#2888](https://github.com/pgRouting/pgrouting/issues/2888) pgr_findcloseedges(text,geometry[],double precision,integer,boolean,boolean)
* [#2899](https://github.com/pgRouting/pgrouting/issues/2899) pgr_maxCardinalityMatch(text,boolean)
* [#2886](https://github.com/pgRouting/pgrouting/issues/2886) pgr_nodenetwork(text,double precision,text,text,text,text,boolean)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) pgr_trsp(text,integer,integer,boolean,boolean,text)
* [#2681](https://github.com/pgRouting/pgrouting/issues/2681) pgr_trspViaedges(text,integer[],double precision[],boolean,boolean,text)
* [#2682](https://github.com/pgRouting/pgrouting/issues/2682) pgr_trspViaVertices(text,anyarray,boolean,boolean,text)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_trspVia_withPoints(text,text,text,anyarray,boolean,boolean,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_trsp_withPoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_trsp_withPoints(text,text,text,anyarray,bigint,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_trsp_withPoints(text,text,text,bigint,anyarray,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_trsp_withPoints(text,text,text,bigint,bigint,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_trsp_withPoints(text,text,text,text,boolean,character,boolean)
* [#2901](https://github.com/pgRouting/pgrouting/issues/2901) pgr_tspEuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* [#2901](https://github.com/pgRouting/pgrouting/issues/2901) pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPointsCostMatrix(text,text,anyarray,boolean,character)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPointsCost(text,text,anyarray,anyarray,boolean,character)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPointsCost(text,text,anyarray,bigint,boolean,character)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPointsCost(text,text,bigint,anyarray,boolean,character)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPointsCost(text,text,bigint,bigint,boolean,character)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPointsCost(text,text,text,boolean,character)
* [#2890](https://github.com/pgRouting/pgrouting/issues/2890) pgr_withPointsDD(text,text,anyarray,double precision,boolean,character,boolean,boolean)
* [#2890](https://github.com/pgRouting/pgrouting/issues/2890) pgr_withPointsDD(text,text,bigint,double precision,boolean,character,boolean)
* [#2895](https://github.com/pgRouting/pgrouting/issues/2895) pgr_withPointsKSP(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPoints(text,text,anyarray,anyarray,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPoints(text,text,anyarray,bigint,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPoints(text,text,bigint,anyarray,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPoints(text,text,bigint,bigint,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPoints(text,text,text,boolean,character,boolean)
* [#2919](https://github.com/pgRouting/pgrouting/issues/2919) pgr_withPointsVia(text,text,anyarray,boolean,boolean,boolean,character,boolean)

Code enhancements

* Removal of unused C/C++ code
* Refactor the Script to build the update PostgreSQL file.
* One process & driver for:

  * allpairs: johnson and Floyd-Warshall
  * Shortest path: Dijkstra and withPoints using Dijkstra

**Deprecation of internal C/C++ functions**

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

**Internal C/C++ functions in legacy**

* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _trsp
* [#2683](https://github.com/pgRouting/pgrouting/issues/2683) _v4trsp
* [#2748](https://github.com/pgRouting/pgrouting/issues/2748) _pgr_alphaShape
* [#2913](https://github.com/pgRouting/pgrouting/issues/2913) _pgr_dagShortestPath

</details>
