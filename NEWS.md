To read all history of releases go to the latest [release notes](https://docs.pgrouting.org/latest/en/release_notes.html)

# pgRouting 3


## pgRouting 3.8



### pgRouting 3.8.0 Release Notes

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.8.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.8.0%22)

**Promotion to official function of pgRouting.**

* [#2772](https://github.com/pgRouting/pgrouting/issues/2772):
  Promoted to official pgr_extractVertices in version 3.8

  * Error messages adjustment.
  * Function promoted to official.

* [#2760](https://github.com/pgRouting/pgrouting/issues/2760):
  Promoted to official pgr_degree in version 3.8

  * Error messages adjustment.
  * New signature with only Edges SQL.
  * Function promoted to official.

* [#2774](https://github.com/pgRouting/pgrouting/issues/2774):
  Promoted to official pgr_findCloseEdges in version 3.8

  * Error messages adjustment.
  * ``partial`` option is removed.
  * Function promoted to official.

* [#2873](https://github.com/pgRouting/pgrouting/issues/2873):
  Promoted to official pgr_separateCrossing in version 3.8

**Proposed functions**

* Contraction

  * [#2790](https://github.com/pgRouting/pgrouting/issues/2790):
    pgr_contractionDeadEnd new contraction function
  * [#2791](https://github.com/pgRouting/pgrouting/issues/2791):
    pgr_contractionLinear new contraction function
  * [#2536](https://github.com/pgRouting/pgrouting/issues/2536):
    Support for contraction hierarchies (pgr_contractionHierarchies)

* Utilities

  * [#2848](https://github.com/pgRouting/pgrouting/issues/2848):
    Create pgr_separateCrossing new utility function
  * [#2849](https://github.com/pgRouting/pgrouting/issues/2849):
    Create of pgr_separateTouching new utility function

**Official functions changes**

* [#2786](https://github.com/pgRouting/pgrouting/issues/2786):
  pgr_contraction(edges) new signature

  * New signature:
    * Previously compulsory parameter **Contraction order** is now optional with
      name ``methods``.
    * New name and order of optional parameters.
  * Deprecated signature pgr_contraction(text,bigint[],integer,bigint[],boolean)

**C/C++ code enhancements**

* [#2802](https://github.com/pgRouting/pgrouting/issues/2802):
  Code reorganization on pgr_contraction
* Other enhancements:
  [#2869](https://github.com/pgRouting/pgrouting/issues/2869)

**SQL code enhancements**

* [#2850](https://github.com/pgRouting/pgrouting/issues/2850):
  Rewrite pgr_nodeNetwork

**Deprecation of SQL functions**

* [#2749](https://github.com/pgRouting/pgrouting/issues/2749):
  Deprecate pgr_AlphaShape in 3.8
* [#2750](https://github.com/pgRouting/pgrouting/issues/2750):
  Deprecate pgr_CreateTopology in 3.8
* [#2753](https://github.com/pgRouting/pgrouting/issues/2753):
  Deprecate pgr_analyzeGraph in 3.8
* [#2754](https://github.com/pgRouting/pgrouting/issues/2754):
  Deprecate pgr_analyzeOneWay in 3.8
* [#2826](https://github.com/pgRouting/pgrouting/issues/2826):
  Deprecate pgr_createVerticesTable in 3.8

In the deprecated functions:

- Migration section is created.
- The use of the functions is removed in the documentation.

## pgRouting 3.7

<details> <summary>pgRouting 3.7.3 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.7.3
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.7.3%22)

* [#2731](https://github.com/pgRouting/pgrouting/pull/2731) Build Failure on Ubuntu 22

</details>

<details> <summary>pgRouting 3.7.2 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.7.2
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.7.2%22)

**Build**

* [#2713](https://github.com/pgRouting/pgrouting/pull/2713) cmake missing
  some policies and min version

  - Using OLD policies: CMP0148, CMP0144, CMP0167
  - Minimum cmake version 3.12

**Bug fixes**

* [#2707](https://github.com/pgRouting/pgrouting/pull/2707) Build failure in
  pgRouting 3.7.1 on Alpine
* [#2706](https://github.com/pgRouting/pgrouting/pull/2706) winnie crashing
  on pgr_betweennessCentrality

</details>

<details> <summary>pgRouting 3.7.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.7.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.7.1%22)

**Bug fixes**

* [#2680](https://github.com/pgRouting/pgrouting/pull/2680) fails to compile
  under mingw64 gcc 13.2
* [#2689](https://github.com/pgRouting/pgrouting/pull/2689) When point is a
  vertex, the withPoints family do not return results.

**C/C++ code enhancemet**

* TRSP family

</details>

<details> <summary>pgRouting 3.7.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.7.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.7.0%22)

**Support**

* [#2656](https://github.com/pgRouting/pgrouting/pull/2656) Stop support of
  PostgreSQL12 on pgrouting v3.7

  * Stopping support of PostgreSQL 12
  * CI does not test for PostgreSQL 12

**New experimental functions**

* Metrics

  * pgr_betweennessCentrality

**Official functions changes**

* [#2605](https://github.com/pgRouting/pgrouting/pull/2605) Standardize
  spanning tree functions output

  * Functions:

    * ``pgr_kruskalDD``
    * ``pgr_kruskalDFS``
    * ``pgr_kruskalBFS``
    * ``pgr_primDD``
    * ``pgr_primDFS``
    * ``pgr_primBFS``

  * Standardizing output columns to ``(seq, depth, start_vid, pred, node, edge, cost, agg_cost)``
    * Added ``pred`` result columns.

**Experimental promoted to proposed.**

* [#2635](https://github.com/pgRouting/pgrouting/pull/2635) pgr_LineGraph
  ignores directed flag and use negative values for identifiers.

  * ``pgr_lineGraph``

    * Function promoted to proposed.
    * Works for directed and undirected graphs.

**Code enhancement**

* [#2599](https://github.com/pgRouting/pgrouting/pull/2599) Driving distance
  cleanup
* [#2607](https://github.com/pgRouting/pgrouting/pull/2607) Read postgresql
  data on C++
* [#2614](https://github.com/pgRouting/pgrouting/pull/2614) Clang tidy does
  not work

</details>

## pgRouting 3.6


<details> <summary>pgRouting 3.6.3 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.6.3
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.6.3%22)

**Build**

* Explicit minimum requirements:

  * postgres 11.0.0
  * postgis 3.0.0

* g++ 13+ is supported

**Code fixes**

* Fix warnings from cpplint.
* Fix warnings from clang 18.

**CI tests**

* Add a clang tidy test on changed files.
* Update test not done on versions: 3.0.1, 3.0.2, 3.0.3, 3.0.4, 3.1.0, 3.1.1,
  3.1.2

**Documentation**

* Results of documentation queries adujsted to  1.83.0 version:

  * pgr_edgeDisjointPaths
  * pgr_stoerWagner

**pgtap tests**

* bug fixes


</details>

<details> <summary>pgRouting 3.6.2 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.6.2
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.6.2%22)

**Upgrade fix**

* The upgrade was failing for same minor

**Code fixes**

* Fix warnings from cpplint

**Others**

* Adjust NEWS generator

  * Name change to `NEWS.md` for better visualization on GitHub

</details>

<details> <summary>pgRouting 3.6.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.6.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.6.1%22)

* [#2588](https://github.com/pgRouting/pgrouting/pull/2588) pgrouting 3.6.0
  fails to build on OSX

</details>

<details> <summary>pgRouting 3.6.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.6.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.6.0%22)

**Official functions changes**

* [#2516](https://github.com/pgRouting/pgrouting/pull/2516) Standardize output
  pgr_aStar

  * Standardize output columns to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

    * pgr_aStar(One to One) added ``start_vid`` and ``end_vid`` columns.
    * pgr_aStar(One to Many) added ``end_vid`` column.
    * pgr_aStar(Many to One) added ``start_vid`` column.

* [#2523](https://github.com/pgRouting/pgrouting/pull/2523) Standardize output
  pgr_bdAstar

  * Standardize output columns to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

    * pgr_bdAstar(One to One) added ``start_vid`` and ``end_vid``
      columns.
    * pgr_bdAstar(One to Many) added ``end_vid`` column.
    * pgr_bdAstar(Many to One) added ``start_vid`` column.

* [#2547](https://github.com/pgRouting/pgrouting/pull/2547) Standardize output
  and modifying signature pgr_KSP

  * Standardizing output columns to ``(seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
  * pgr_ksp(One to One)
    * Added ``start_vid`` and ``end_vid`` result columns.
  * New proposed signatures:
    * pgr_ksp(One to Many)
    * pgr_ksp(Many to One)
    * pgr_ksp(Many to Many)
    * pgr_ksp(Combinations)

* [#2548](https://github.com/pgRouting/pgrouting/pull/2548) Standardize output
  pgr_drivingDistance

  * Standardizing output columns to ``(seq, depth, start_vid, pred, node, edge, cost, agg_cost)``
    * pgr_drivingDistance(Single vertex)
      * Added ``depth`` and ``start_vid`` result columns.
    * pgr_drivingDistance(Multiple vertices)
      * Result column name change: ``from_v`` to ``start_vid``.
      * Added ``depth`` and ``pred`` result columns.

**Proposed functions changes**

* [#2544](https://github.com/pgRouting/pgrouting/pull/2544) Standardize output
  and modifying signature pgr_withPointsDD

  * Signature change: ``driving_side`` parameter changed from named optional to
    unnamed compulsory **driving side**.
    * pgr_withPointsDD(Single vertex)
    * pgr_withPointsDD(Multiple vertices)
  * Standardizing output columns to ``(seq, depth, start_vid, pred, node, edge, cost, agg_cost)``
    * pgr_withPointsDD(Single vertex)
      * Added ``depth``, ``pred`` and ``start_vid`` column.
    * pgr_withPointsDD(Multiple vertices)
      * Added ``depth``, ``pred`` columns.
  * When ``details`` is ``false``:
    * Only points that are visited are removed, that is, points reached within the
      distance are included
  * Deprecated signatures
    * pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)
    * pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)

* [#2546](https://github.com/pgRouting/pgrouting/pull/2546) Standardize output
  and modifying signature pgr_withPointsKSP

  * Standardizing output columns to ``(seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
  * pgr_withPointsKSP(One to One)
    * Signature change: ``driving_side`` parameter changed from named optional to
      unnamed compulsory **driving side**.
    * Added ``start_vid`` and ``end_vid`` result columns.
  * New proposed signatures:
    * pgr_withPointsKSP(One to Many)
    * pgr_withPointsKSP(Many to One)
    * pgr_withPointsKSP(Many to Many)
    * pgr_withPointsKSP(Combinations)
  * Deprecated signature
    * pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,char,boolean)``

**C/C++ code enhancements**

* [#2504](https://github.com/pgRouting/pgrouting/pull/2504) To C++ pg data get,
  fetch and check.

  * Stopping support for compilation with MSVC.

* [#2505](https://github.com/pgRouting/pgrouting/pull/2505) Using namespace.
* [#2512](https://github.com/pgRouting/pgrouting/pull/2512) [Dijkstra] Removing
  duplicate code on Dijkstra.
* [#2517](https://github.com/pgRouting/pgrouting/pull/2517) Astar code
  simplification.
* [#2521](https://github.com/pgRouting/pgrouting/pull/2521) Dijkstra code
  simplification.
* [#2522](https://github.com/pgRouting/pgrouting/pull/2522) bdAstar code
  simplification.

**Documentation**

* [#2490](https://github.com/pgRouting/pgrouting/pull/2490) Automatic page
  history links.

* ..rubric:: Standardize SQL

* [#2555](https://github.com/pgRouting/pgrouting/pull/2555) Standardize
  deprecated messages
* On new internal function: do not use named parameters and default parameters.

</details>

## pgRouting 3.5


<details> <summary>pgRouting 3.5.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.5.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.5.1%22)

**Documentation fixes**

Changes on the documentation to the following:

* pgr_degree
* pgr_dijkstra
* pgr_ksp
* Automatic page history links

  * using bootstrap_version 2 because 3+ does not do dropdowns

**Issue fixes**

* [#2565](https://github.com/pgRouting/pgrouting/issues/2565)
  pgr_lengauerTarjanDominatorTree triggers an assertion

**SQL enhancements**

* [#2561](https://github.com/pgRouting/pgrouting/issues/2561) Not use
  wildcards on SQL

**pgtap tests**

* [#2559](https://github.com/pgRouting/pgrouting/issues/2559) pgtap test using sampledata

**Build fixes**

* Fix winnie build

**Code fixes**

* Fix clang warnings

  * Grouping headers of postgres readers

</details>

<details> <summary>pgRouting 3.5.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.5.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.5.0%22)

**Official functions changes**

* Dijkstra

  * Standardize output columns to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

    * pgr_dijkstra(One to One) added ``start_vid`` and ``end_vid`` columns.
    * pgr_dijkstra(One to Many) added ``end_vid`` column.
    * pgr_dijkstra(Many to One) added ``start_vid`` column.

</details>

## pgRouting 3.4


<details> <summary>pgRouting 3.4.2 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.4.2
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.4.2%22)

**Issue fixes**

* [#2394](https://github.com/pgRouting/pgrouting/issues/2394):
  pgr_bdAstar accumulates heuristic cost in visited node cost.
* [#2427](https://github.com/pgRouting/pgrouting/issues/2427):
  pgr_createVerticesTable & pgr_createTopology, variable should be of type Record.

</details>

<details> <summary>pgRouting 3.4.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.4.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.4.1%22)

**Issue fixes**

* [#2401](https://github.com/pgRouting/pgrouting/issues/2401):
  pgRouting 3.4.0 do not build docs when sphinx is too low or missing
* [#2398](https://github.com/pgRouting/pgrouting/issues/2398):
  v3.4.0 does not upgrade from 3.3.3

</details>

<details> <summary>pgRouting 3.4.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.4.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.4.0%22)

**Issue fixes**

* [#1891](https://github.com/pgRouting/pgrouting/issues/1891):
  pgr_ksp doesn't give all correct shortest path

**New proposed functions.**

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

**New experimental functions**

* Ordering

  * pgr_cuthillMckeeOrdering

* Unclassified

  * pgr_hawickCircuits

**Official functions changes**

* Flow functions

  * pgr_maxCardinalityMatch(text)

    * Deprecating: pgr_maxCardinalityMatch(text,boolean)

**Deprecated Functions**

* Turn Restrictions

  * pgr_trsp(text,integer,integer,boolean,boolean,text)
  * pgr_trsp(text,integer,float8,integer,float8,boolean,boolean,text)
  * pgr_trspViaVertices(text,anyarray,boolean,boolean,text)
  * pgr_trspViaEdges(text,integer[],float[],boolean,boolean,text)

</details>

## pgRouting 3.3


<details> <summary>pgRouting 3.3.5 Release Notes</summary>

* [#2401](https://github.com/pgRouting/pgrouting/issues/2401):
  pgRouting 3.4.0 do not build docs when sphinx is too low or missing

</details>

<details> <summary>pgRouting 3.3.4 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.3.4
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.3.4%22)

**Issue fixes**

* [#2400](https://github.com/pgRouting/pgrouting/issues/2400):
  pgRouting 3.3.3 does not build in focal

</details>

<details> <summary>pgRouting 3.3.3 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.3.3
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.3.3%22)

**Issue fixes**

* [#1891](https://github.com/pgRouting/pgrouting/issues/1891):
  pgr_ksp doesn't give all correct shortest path

**Official functions changes**

* Flow functions

  * pgr_maxCardinalityMatch(text,boolean)

    * Ignoring optional boolean parameter, as the algorithm works only for
      undirected graphs.


</details>

<details> <summary>pgRouting 3.3.2 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.3.2
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.3.2%22)

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


**Issue fixes**

* [#2276](https://github.com/pgRouting/pgrouting/issues/2276):
  edgeDisjointPaths issues with start_vid and combinations
* [#2312](https://github.com/pgRouting/pgrouting/issues/2312):
  pgr_extractVertices error when target is not BIGINT
* [#2357](https://github.com/pgRouting/pgrouting/issues/2357):
  Apply clang-tidy performance-*

</details>

<details> <summary>pgRouting 3.3.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.3.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.3.1%22)
on Github.

**Issue fixes**

* [#2216](https://github.com/pgRouting/pgrouting/issues/2216): Warnings when using clang
* [#2266](https://github.com/pgRouting/pgrouting/issues/2266): Error processing restrictions


</details>

<details> <summary>pgRouting 3.3.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.3.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.3.0%22)
on Github.

**Issue fixes**

* [#2057](https://github.com/pgRouting/pgrouting/issues/2057): trspViaEdges columns in different order
* [#2087](https://github.com/pgRouting/pgrouting/issues/2087): pgr_extractVertices to proposed
* [#2201](https://github.com/pgRouting/pgrouting/issues/2201): pgr_depthFirstSearch to proposed
* [#2202](https://github.com/pgRouting/pgrouting/issues/2202): pgr_sequentialVertexColoring to proposed
* [#2203](https://github.com/pgRouting/pgrouting/issues/2203): pgr_dijkstraNear and pgr_dijkstraNearCost to proposed

**New experimental functions**

* Coloring

  * pgr_edgeColoring

**Experimental promoted to Proposed**

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

</details>

## pgRouting 3.2


<details> <summary>pgRouting 3.2.2 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.2.2
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.2.2%22)
on Github.

**Issue fixes**

* [#2093](https://github.com/pgRouting/pgrouting/issues/2093): Compilation on Visual Studio
* [#2189](https://github.com/pgRouting/pgrouting/issues/2189): Build error on RHEL 7

</details>

<details> <summary>pgRouting 3.2.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.2.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.2.1%22)
on Github.

**Issue fixes**

* [#1883](https://github.com/pgRouting/pgrouting/issues/1883): pgr_TSPEuclidean crashes connection on Windows

  * The solution is to use Boost::graph::metric_tsp_approx
  * To not break user's code the optional parameters related to the TSP Annaeling are ignored
  * The function with the annaeling optional parameters is deprecated


</details>

<details> <summary>pgRouting 3.2.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.2.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.2.0%22)
on Github.

**Build**

* [#1850](https://github.com/pgRouting/pgrouting/issues/1850): Change Boost
  min version to 1.56

  * Removing support for Boost v1.53, v1.54 & v1.55

**New experimental functions**

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

**New proposed functions.**

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

</details>

## pgRouting 3.1


<details> <summary>pgRouting 3.1.4 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.1.4
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.1.4%22)
on Github.

**Issues fixes**

* [#2189](https://github.com/pgRouting/pgrouting/issues/2189): Build error on
  RHEL 7


</details>

<details> <summary>pgRouting 3.1.3 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.1.3
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.1.3%22)
on Github.

**Issues fixes**

* [#1825](https://github.com/pgRouting/pgrouting/issues/1825): Boost versions
  are not honored
* [#1849](https://github.com/pgRouting/pgrouting/issues/1849): Boost 1.75.0
  geometry "point_xy.hpp" build error on macOS environment
* [#1861](https://github.com/pgRouting/pgrouting/issues/1861): vrp functions
  crash server


</details>

<details> <summary>pgRouting 3.1.2 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.1.2
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.1.2%22)
on Github.

**Issues fixes**

* [#1304](https://github.com/pgRouting/pgrouting/issues/1304): FreeBSD 12
  64-bit crashes on pgr_vrOneDepot tests Experimental Function
* [#1356](https://github.com/pgRouting/pgrouting/issues/1356):
  tools/testers/pg_prove_tests.sh fails when PostgreSQL port is not passed
* [#1725](https://github.com/pgRouting/pgrouting/issues/1725): Server crash
  on pgr_pickDeliver and pgr_vrpOneDepot on openbsd
* [#1760](https://github.com/pgRouting/pgrouting/issues/1760): TSP server
  crash on ubuntu 20.04 #1760
* [#1770](https://github.com/pgRouting/pgrouting/issues/1770): Remove
  warnings when using clang compiler


</details>

<details> <summary>pgRouting 3.1.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.1.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.1.1%22)
on Github.

**Issues fixes**

* [#1733](https://github.com/pgRouting/pgrouting/issues/1733): pgr_bdAstar
  fails when source or target vertex does not exist in the graph
* [#1647](https://github.com/pgRouting/pgrouting/issues/1647): Linear
  Contraction contracts self loops
* [#1640](https://github.com/pgRouting/pgrouting/issues/1640): pgr_withPoints
  fails when points_sql is empty
* [#1616](https://github.com/pgRouting/pgrouting/issues/1616): Path
  evaluation on C++ not updated before the results go back to C
* [#1300](https://github.com/pgRouting/pgrouting/issues/1300):
  pgr_chinesePostman crash on test data



</details>

<details> <summary>pgRouting 3.1.0 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.1.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.1.0%22)
on Github.

**New proposed functions.**

* pgr_dijkstra(combinations)
* pgr_dijkstraCost(combinations)

**Build changes**

* Minimal requirement for Sphinx: version 1.8

</details>

## pgRouting 3.0


<details> <summary>pgRouting 3.0.6 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.0.6
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.0.6%22)
on Github.

**Issues fixes**

* [#2189](https://github.com/pgRouting/pgrouting/issues/2189): Build error on
  RHEL 7


</details>

<details> <summary>pgRouting 3.0.5 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.0.5
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.0.5%22)
on Github.

**Backport issue fixes**

* [#1825](https://github.com/pgRouting/pgrouting/issues/1825): Boost versions
  are not honored
* [#1849](https://github.com/pgRouting/pgrouting/issues/1849): Boost 1.75.0
  geometry "point_xy.hpp" build error on macOS environment
* [#1861](https://github.com/pgRouting/pgrouting/issues/1861): vrp functions
  crash server


</details>

<details> <summary>pgRouting 3.0.4 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.0.4
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.0.4%22)
on Github.

**Backport issue fixes**

* [#1304](https://github.com/pgRouting/pgrouting/issues/1304): FreeBSD 12
  64-bit crashes on pgr_vrOneDepot tests Experimental Function
* [#1356](https://github.com/pgRouting/pgrouting/issues/1356):
  tools/testers/pg_prove_tests.sh fails when PostgreSQL port is not passed
* [#1725](https://github.com/pgRouting/pgrouting/issues/1725): Server crash
  on pgr_pickDeliver and pgr_vrpOneDepot on openbsd
* [#1760](https://github.com/pgRouting/pgrouting/issues/1760): TSP server
  crash on ubuntu 20.04 #1760
* [#1770](https://github.com/pgRouting/pgrouting/issues/1770): Remove
  warnings when using clang compiler



</details>

<details> <summary>pgRouting 3.0.3 Release Notes</summary>

**Backport issue fixes**

* [#1733](https://github.com/pgRouting/pgrouting/issues/1733): pgr_bdAstar
  fails when source or target vertex does not exist in the graph
* [#1647](https://github.com/pgRouting/pgrouting/issues/1647): Linear
  Contraction contracts self loops
* [#1640](https://github.com/pgRouting/pgrouting/issues/1640): pgr_withPoints
  fails when points_sql is empty
* [#1616](https://github.com/pgRouting/pgrouting/issues/1616): Path
  evaluation on C++ not updated before the results go back to C
* [#1300](https://github.com/pgRouting/pgrouting/issues/1300):
  pgr_chinesePostman crash on test data



</details>

<details> <summary>pgRouting 3.0.2 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.0.2
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.0.2%22)
on Github.

**Issues fixes**

* [#1378](https://github.com/pgRouting/pgrouting/issues/1378): Visual Studio
  build failing


</details>

<details> <summary>pgRouting 3.0.1 Release Notes</summary>

To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.0.1
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.0.1%22)
on Github.

**Issues fixes**

* [#232](https://github.com/pgRouting/pgrouting/issues/232): Honor client
  cancel requests in C /C++ code


</details>

<details> <summary>pgRouting 3.0.0 Release Notes</summary>


To see all issues & pull requests closed by this release see the [Git closed
milestone for 3.0.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%203.0.0%22)
on Github.

**Fixed Issues**

* [#1153](https://github.com/pgRouting/pgrouting/issues/1153): Renamed
  pgr_eucledianTSP to pgr_TSPeuclidean
* [#1188](https://github.com/pgRouting/pgrouting/issues/1188): Removed CGAL
  dependency
* [#1002](https://github.com/pgRouting/pgrouting/issues/1002): Fixed
  contraction issues:

  * [#1004](https://github.com/pgRouting/pgrouting/issues/1004): Contracts
    when forbidden vertices do not belong to graph
  * [#1005](https://github.com/pgRouting/pgrouting/issues/1005): Intermideate
    results eliminated
  * [#1006](https://github.com/pgRouting/pgrouting/issues/1006): No loss of
    information

**New Functions**

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


**Proposed moved to official on pgRouting**

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


**New experimental functions**

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

**Moved to legacy**

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

</details>
