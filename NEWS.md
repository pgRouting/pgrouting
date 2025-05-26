To read all history of releases go to the latest [release notes](https://docs.pgrouting.org/latest/en/release_notes.html)

# pgRouting 4


## pgRouting 4.0



### pgRouting 4.0.0 Release Notes

To see all issues & pull requests closed by this release see the [Git closed
milestone for 4.0.0
](https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%204.0.0%22)

**Functions promoted to official**

* pgr_trsp
* pgr_trspVia
* pgr_trspVia_withPoints
* pgr_trsp_withPoints
* pgr_withPoints
* pgr_withPointsCost
* pgr_withPointsCostMatrix
* pgr_withPointsDD
* pgr_withPointsKSP
* pgr_withPointsVia

**Signatures promoted to official**

* pgr_aStar(Combinations)
* pgr_aStarCost(Combinations)
* pgr_bdAstar(Combinations)
* pgr_bdAstarCost(Combinations)
* pgr_bdDijkstra(Combinations)
* pgr_bdDijkstraCost(Combinations)
* pgr_dijkstra(Combinations)
* pgr_dijkstraCost(Combinations)
* pgr_KSP(All signatures)
* pgr_boykovKolmogorov(Combinations)
* pgr_edmondsKarp(Combinations)
* pgr_maxFlow(Combinations)
* pgr_pushRelabel(Combinations)

**code enhancements:**

* Removal of unused C/C++ code

**Removal of SQL deprecated signatures**

* [#2798](https://github.com/pgRouting/pgrouting/issues/2798):
  pgr_contraction

  * pgr_contraction(text,bigint[],integer,bigint[],boolean)

* [#2683](https://github.com/pgRouting/pgrouting/issues/2683): pgr_trsp

  * pgr_trsp(text,integer,integer,boolean,boolean,text)
  * pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)

* [#2888](https://github.com/pgRouting/pgrouting/issues/2888):
  pgr_findCloseEdges

  * pgr_findcloseedges(text,geometry,double precision,integer,boolean,boolean)
  * pgr_findcloseedges(text,geometry[],double precision,integer,boolean,boolean)

* [#2890](https://github.com/pgRouting/pgrouting/issues/2890):
  pgr_withPointsDD

  * pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)
  * pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)

* [#2895](https://github.com/pgRouting/pgrouting/issues/2895):
  pgr_withPointsKSP

  * pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)

* [#2899](https://github.com/pgRouting/pgrouting/issues/2899):
  pgr_maxCardinalityMatch

  * pgr_maxcardinalitymatch(text,boolean)

* [#2901](https://github.com/pgRouting/pgrouting/issues/2901): TSP family

  * pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
  * pgr_tspeuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)

**Removal of SQL deprecated functions**

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

**Removal of SQL deprecated internal functions**

Related issues: [#2897](https://github.com/pgRouting/pgrouting/issues/2897)

* _pgr_dijkstranear(text,anyarray,anyarray,bigint,boolean)
* _pgr_dijkstranear(text,anyarray,bigint,bigint,boolean)
* _pgr_dijkstranear(text,bigint,anyarray,bigint,boolean)
* _pgr_dijkstra(text,anyarray,anyarray,boolean,boolean,boolean,bigint)
* _pgr_dijkstra(text,text,boolean,boolean,boolean)
* _pgr_drivingdistance(text,anyarray,double precision,boolean,boolean)
* _pgr_ksp(text,anyarray,anyarray,integer,boolean,boolean,boolean)
* _pgr_ksp(text,bigint,bigint,integer,boolean,boolean)
* _pgr_ksp(text,text,integer,boolean,boolean)
* _pgr_kruskal(text,anyarray,text,bigint,double precision)
* _pgr_maxcardinalitymatch(text,boolean)
* _pgr_prim(text,anyarray,text,bigint,double precision)
* _pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
* _pgr_trsp(text,text,anyarray,anyarray,boolean)
* _pgr_trsp(text,text,anyarray,bigint,boolean)
* _pgr_trsp(text,text,bigint,anyarray,boolean)
* _pgr_trsp(text,text,bigint,bigint,boolean)
* _pgr_trspviavertices(text,integer[],boolean,boolean,text)
* _pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)
* _pgr_withpointsksp(text,text,anyarray,anyarray,integer,character,boolean,boolean,boolean,boolean)
* _pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,character,boolean)
* _pgr_withpointsksp(text,text,text,integer,character,boolean,boolean,boolean)
* _pgr_withpointsvia(text,bigint[],double precision[],boolean)
* _trsp(text,text,anyarray,anyarray,boolean)
* _v4trsp(text,text,anyarray,anyarray,boolean)
* _v4trsp(text,text,text,boolean)
* _pgr_tspeuclidean(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)
* _pgr_tsp(text,bigint,bigint,double precision,integer,integer,integer,double precision,double precision,double precision,boolean)


* [#2861](https://github.com/pgRouting/pgrouting/issues/2861):
  Remove unused internal functions

  - _pgr_checkverttab(text,text[],integer,text)
  - _pgr_createindex(text,text,text,integer,text)
  - _pgr_createindex(text,text,text,text,integer,text)
  - _pgr_endpoint(geometry)
  - _pgr_getcolumnname(text,text,integer,text)
  - _pgr_getcolumnname(text,text,text,integer,text)
  - _pgr_getcolumntype(text,text,integer,text)
  - _pgr_getcolumntype(text,text,text,integer,text)
  - _pgr_gettablename(text,integer,text)
  - _pgr_iscolumnindexed(text,text,integer,text)
  - _pgr_iscolumnindexed(text,text,text,integer,text)
  - _pgr_iscolumnintable(text,text)
  - _pgr_msg(integer,text,text)
  - _pgr_onerror(boolean,integer,text,text,text,text)
  - _pgr_pointtoid(geometry,double precision,text,integer)
  - _pgr_quote_ident(text)
  - _pgr_startpoint(geometry)
  - _pgr_versionless(text,text)


**Deprecation of internal C/C++ functions**

* _pgr_drivingdistance(text,anyarray,double precision,boolean,boolean)
* _pgr_kruskal(text,anyarray,text,bigint,double precision)
* _pgr_prim(text,anyarray,text,bigint,double precision)

**Internal C/C++ functions in legacy**

* _trsp(text,text,anyarray,anyarray,boolean)
* _v4trsp(text,text,anyarray,anyarray,boolean)
* _v4trsp(text,text,text,boolean)
* _pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)

</details>
