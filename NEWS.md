To read all history of releases go to the latest [release notes](https://docs.pgrouting.org/latest/en/release_notes.html)

# pgRouting 4


## pgRouting 4.0



### pgRouting 4.0.0 Release Notes

To see all issues & pull requests closed by this release see the [Git closed
milestone for 4.0.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%204.0.0%22)


Summary of changes by function

* pgr_contraction

  * Breaking change, signatures no longer available:
    * pgr_contraction(text,bigint[],integer,bigint[],boolean)

* pgr_bdDijkstra

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
  * Combinations signature promoted to official.

* pgr_bellmanFord

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_binaryBreadthFirstSearch

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_edwardMoore

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_DAGshortestPath

  * Output columns standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

* pgr_trsp

  * Function promoted to official.
  .. Breaking change
  * Breaking change, signatures no longer available:
    * pgr_trsp(text,integer,integer,boolean,boolean,text)
    * pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)

* pgr_trspVia

  * Function promoted to official.
  * Breaking change, signatures no longer available:
    * pgr_trspviavertices(text,anyarray,boolean,boolean,text)

* pgr_trspVia_withPoints

  * Function promoted to official.
  * **Driving side** parameter is positional unnamed and compulsory.
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

* [#2701](https://github.com/pgRouting/pgrouting/issues/2701)
  pgr_trsp
* [#2701](https://github.com/pgRouting/pgrouting/issues/2701)
  pgr_trspVia
* [#2701](https://github.com/pgRouting/pgrouting/issues/2701)
  pgr_trspVia_withPoints
* [#2701](https://github.com/pgRouting/pgrouting/issues/2701)
  pgr_trsp_withPoints
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPoints
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPointsCost
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPointsCostMatrix
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPointsDD
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPointsKSP
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPointsVia

Signatures promoted to official

* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_aStar(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_aStarCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_bdAstar(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_bdAstarCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_bdDijkstra(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_bdDijkstraCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_dijkstra(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_dijkstraCost(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_KSP(All signatures)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_boykovKolmogorov(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_edmondsKarp(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_maxFlow(Combinations)
* [#2718](https://github.com/pgRouting/pgrouting/issues/2918)
  pgr_pushRelabel(Combinations)

SQL signatures and output standardization

[#2904](https://github.com/pgRouting/pgrouting/issues/2904)
Standardize output columns of functions with different output columns within overloads

**Standardized to ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``**

* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPoints
* [#2906](https://github.com/pgRouting/pgrouting/issues/2906)
  pgr_bdDijkstra
* [#2907](https://github.com/pgRouting/pgrouting/issues/2907)
  pgr_bellmanFord
* [#2908](https://github.com/pgRouting/pgrouting/issues/2908)
  pgr_binaryBreadthFirstSearch
* [#2910](https://github.com/pgRouting/pgrouting/issues/2910)
  pgr_edwardMoore
* [#2913](https://github.com/pgRouting/pgrouting/issues/2913)
  pgr_DAGshortestPath

**Standardized to ``(start_vid, end_vid, agg_cost)``**

* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPointsCost
* [#2905](https://github.com/pgRouting/pgrouting/issues/2905)
  pgr_withPointsCostMatrix

Removal of SQL deprecated signatures

* [#2798](https://github.com/pgRouting/pgrouting/issues/2798):
  pgr_contraction

    * pgr_contraction(text,bigint[],integer,bigint[],boolean)

* [#2683](https://github.com/pgRouting/pgrouting/issues/2683): pgr_trsp

    * pgr_trsp(text,integer,integer,boolean,boolean,text)
    * pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)

* [#2700](https://github.com/pgRouting/pgrouting/issues/2700):
  pgr_withPointsVia

    * pgr_withpointsvia(text,text,anyarray,boolean,boolean,boolean,character,boolean)

* [#2888](https://github.com/pgRouting/pgrouting/issues/2888):
  pgr_findCloseEdges

  * pgr_findcloseedges(text,geometry,double precision,integer,boolean,boolean)
  * pgr_findcloseedges(text,geometry[],double precision,integer,boolean,boolean)

* [#2890](https://github.com/pgRouting/pgrouting/issues/2890):
  pgr_withPointsDD

    * pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)
    * pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)

* [#2895](https://github.com/pgRouting/pgrouting/issues/2895):
  pgr_withPointsKSP

    * pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)

* [#2899](https://github.com/pgRouting/pgrouting/issues/2899):
  pgr_maxCardinalityMatch

    * pgr_maxCardinalityMatch(text,boolean)

* [#2901](https://github.com/pgRouting/pgrouting/issues/2901):
  pgr_TSP

    * pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double
      precision,double precision,double precision,boolean)

* [#2901](https://github.com/pgRouting/pgrouting/issues/2901):
  pgr_TSPeuclidean

    * pgr_tspeuclidean(text,bigint,bigint,double precision,integer,integer,
      integer,double precision,double precision,double precision,boolean)


Removal of SQL deprecated functions

* [#2681](https://github.com/pgRouting/pgrouting/issues/2681):
  pgr_trspviaedges
* [#2682](https://github.com/pgRouting/pgrouting/issues/2682):
  pgr_trspviavertices
* [#2748](https://github.com/pgRouting/pgrouting/issues/2748):
  pgr_alphaShape
* [#2751](https://github.com/pgRouting/pgrouting/issues/2851):
  pgr_createTopology
* [#2752](https://github.com/pgRouting/pgrouting/issues/2752):
  pgr_analyzeGraph
* [#2755](https://github.com/pgRouting/pgrouting/issues/2755):
  pgr_analyzeOneWay
* [#2827](https://github.com/pgRouting/pgrouting/issues/2827):
  pgr_createVerticesTable
* [#2886](https://github.com/pgRouting/pgrouting/issues/2886):
  pgr_nodeNetwork

Removal of SQL deprecated internal functions

* #2748 _pgr_alphashape(text,double precision)
* #2861 _pgr_checkverttab(text,text[],integer,text)
* #2861 _pgr_createindex(text,text,text,integer,text)
* #2861 _pgr_createindex(text,text,text,text,integer,text)
* #2913 _pgr_dagshortestpath(text,anyarray,anyarray,boolean,boolean)
* #2913 _pgr_dagshortestpath(text,text,boolean,boolean)
* #2730 _pgr_dijkstranear(text,anyarray,anyarray,bigint,boolean)
* #2730 _pgr_dijkstranear(text,anyarray,bigint,bigint,boolean)
* #2730 _pgr_dijkstranear(text,bigint,anyarray,bigint,boolean)
* #2730 _pgr_dijkstra(text,anyarray,anyarray,boolean,boolean,boolean,bigint)
* #2730 _pgr_dijkstra(text,anyarray,anyarray,boolean,boolean,boolean,bigint,boolean)
* #2730 _pgr_dijkstra(text,text,boolean,boolean,bigint,boolean)
* #2730 _pgr_dijkstra(text,text,boolean,boolean,boolean)
* #2735 _pgr_drivingdistance(text,anyarray,double precision,boolean,boolean)
* #2861 _pgr_endpoint(geometry)
* #2861 _pgr_getcolumnname(text,text,integer,text)
* #2861 _pgr_getcolumnname(text,text,text,integer,text)
* #2861 _pgr_getcolumntype(text,text,integer,text)
* #2861 _pgr_getcolumntype(text,text,text,integer,text)
* #2861 _pgr_gettablename(text,integer,text)
* #2861 _pgr_iscolumnindexed(text,text,integer,text)
* #2861 _pgr_iscolumnindexed(text,text,text,integer,text)
* #2861 _pgr_iscolumnintable(text,text)
* #2745 _pgr_kruskal(text,anyarray,text,bigint,double precision)
* #2897 _pgr_ksp(text,anyarray,anyarray,integer,boolean,boolean,boolean)
* #2897 _pgr_ksp(text,bigint,bigint,integer,boolean,boolean)
* #2897 _pgr_ksp(text,text,integer,boolean,boolean)
* #2899 _pgr_maxcardinalitymatch(text,boolean)
* #2861 _pgr_msg(integer,text,text)
* #2861 _pgr_onerror(boolean,integer,text,text,text,text)
* #2861 _pgr_pointtoid(geometry,double precision,text,integer)
* #2743 _pgr_prim(text,anyarray,text,bigint,double precision)
* #2861 _pgr_quote_ident(text)
* #2861 _pgr_startpoint(geometry)
* #2683 _pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
* #2683 _pgr_trsp(text,text,anyarray,anyarray,boolean)
* #2683 _pgr_trsp(text,text,anyarray,bigint,boolean)
* #2683 _pgr_trsp(text,text,bigint,anyarray,boolean)
* #2683 _pgr_trsp(text,text,bigint,bigint,boolean)
* #2682 _pgr_trspviavertices(text,integer[],boolean,boolean,text)
* #2919 _pgr_trspvia_withpoints(text,text,text,anyarray,boolean,boolean,boolean,character,boolean)
* #2919 _pgr_trsp_withpoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
* #2919 _pgr_trsp_withpoints(text,text,text,text,boolean,character,boolean)
* #2901 _pgr_tspeuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* #2901 _pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* #2861 _pgr_versionless(text,text)
* #2890 _pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)
* #2895 _pgr_withpointsksp(text,text,anyarray,anyarray,integer,character,boolean,boolean,boolean,boolean)
* #2895 _pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)
* #2895 _pgr_withpointsksp(text,text,text,integer,character,boolean,boolean,boolean)
* #2741 _pgr_withpointsvia(text,bigint[],double precision[],boolean)
* #2741 _pgr_withpointsvia(text,text,anyarray,boolean,boolean,boolean,character,boolean)
* #2683 _trsp(text,text,anyarray,anyarray,boolean)
* #2683 _v4trsp(text,text,anyarray,anyarray,boolean)
* #2683 _v4trsp(text,text,text,boolean)

Summary of functions and signatures no longer on pgrouting

* #2748 pgr_alphashape(geometry,double precision)
* #2752 pgr_analyzegraph(text,double precision,text,text,text,text,text)
* #2755 pgr_analyzeoneway(text,text[],text[],text[],text[],boolean,text,text,text)
* #2798 pgr_contraction(text,bigint[],integer,bigint[],boolean)
* #2751 pgr_createtopology(text,double precision,text,text,text,text,text,boolean)
* #2827 pgr_createverticestable(text,text,text,text,text)
* #2888 pgr_findcloseedges(text,geometry,double precision,integer,boolean,boolean)
* #2888 pgr_findcloseedges(text,geometry[],double precision,integer,boolean,boolean)
* #2899 pgr_maxcardinalitymatch(text,boolean)
* #2886 pgr_nodenetwork(text,double precision,text,text,text,text,boolean)
* #2683 pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
* #2683 pgr_trsp(text,integer,integer,boolean,boolean,text)
* #2681 pgr_trspviaedges(text,integer[],double precision[],boolean,boolean,text)
* #2682 pgr_trspviavertices(text,anyarray,boolean,boolean,text)
* #2919 pgr_trspvia_withpoints(text,text,text,anyarray,boolean,boolean,boolean,character,boolean)
* #2919 pgr_trsp_withpoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
* #2919 pgr_trsp_withpoints(text,text,text,anyarray,bigint,boolean,character,boolean)
* #2919 pgr_trsp_withpoints(text,text,text,bigint,anyarray,boolean,character,boolean)
* #2919 pgr_trsp_withpoints(text,text,text,bigint,bigint,boolean,character,boolean)
* #2919 pgr_trsp_withpoints(text,text,text,text,boolean,character,boolean)
* #2901 pgr_tspeuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* #2901 pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* #2919 pgr_withpointscostmatrix(text,text,anyarray,boolean,character)
* #2919 pgr_withpointscost(text,text,anyarray,anyarray,boolean,character)
* #2919 pgr_withpointscost(text,text,anyarray,bigint,boolean,character)
* #2919 pgr_withpointscost(text,text,bigint,anyarray,boolean,character)
* #2919 pgr_withpointscost(text,text,bigint,bigint,boolean,character)
* #2919 pgr_withpointscost(text,text,text,boolean,character)
* #2890 pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)
* #2890 pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)
* #2895 pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)
* #2919 pgr_withpoints(text,text,anyarray,anyarray,boolean,character,boolean)
* #2919 pgr_withpoints(text,text,anyarray,bigint,boolean,character,boolean)
* #2919 pgr_withpoints(text,text,bigint,anyarray,boolean,character,boolean)
* #2919 pgr_withpoints(text,text,bigint,bigint,boolean,character,boolean)
* #2919 pgr_withpoints(text,text,text,boolean,character,boolean)
* #2919 pgr_withpointsvia(text,text,anyarray,boolean,boolean,boolean,character,boolean)

**Internal functions**


Code enhancements

* Removal of unused C/C++ code
* Refactor the Script to build the update PostgreSQL file.
* One process & driver for:

  * allpairs: johnson and Floyd-Warshall
  * Shortest path: Dijkstra and withPoints using Dijkstra

**Deprecation of internal C/C++ functions**

Deprecated functions are substitued by new function.

* _pgr_drivingdistance => _pgr_drivingdistancev4
* _pgr_withpointsdd => _pgr_withpointsddv4
* _pgr_kruskal => _pgr_kruskalv4
* _pgr_prim => _pgr_primv4
* _pgr_dijkstra => _pgr_dijkstra_v4
* _pgr_withpointsksp => _pgr_withpointsksp_v4
* _pgr_trspvia_withpoints => _pgr_trspvia_withpoints_v4
* _pgr_trsp_withpoints => _pgr_trsp_withpoints_v4
* _pgr_withpointsvia => _pgr_withpointsvia_v4

**Internal C/C++ functions in legacy**

* #2683 _trsp
* #2683 _v4trsp
* #2683 _v4trsp
* #2748 _pgr_alphashape
* #2913 _pgr_dagshortestpath

</details>
