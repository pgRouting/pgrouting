BEGIN;
BEGIN
SET client_min_messages TO NOTICE;
SET
ROLLBACK;
ROLLBACK
# Notes on pgr_trsp for version 2.6.0
Table of contents
* [Introduction](#introduction)
  * [The restriction](#the-restriction)
* [The Vertices Signature Version](#the-vertices-signature-version)
  * [No path representation differences](#vertices-no-path-representation-differences)
  * [Routing from/to same location](#routing-fromto-same-location)
  * [Undirected graph](#vertices-undirected-graph)
* [The Edges Signature Version](#the-edges-signature-version)
  * [No path representation differences](#edges-no-path-representation-differences)
  * [Definition of a path](#definition-of-a-path)
  * [Points on the same edge](#points-on-the-same-edge)
  * [Undirected graph](#edges-undirected-graph)
  * [Using a points of interest table](#using-a-points-of-interest-table)
  * [Routing from/to the same point](#routing-fromto-the-same-point)
  * [Passing in front of other points](#passing-in-front-of-other-points)
  * [Routing from a vertex to a point](#edges-routing-from-a-vertex-to-a-point)
* [pgr_trspViaVertices](#pgr_trspviavertices)
  * [pgr_trspViaVertices No path representation differences](#pgr_trspviavertices-no-path-representation-differences)
  * [when a path does not exist on the route](#when-a-path-does-not-exist-on-the-route)
  * [from 2 to 3 to 2](#from-2-to-3-to-2)
* [pgr_trspViaEdges](#pgr_trspviaedges)
  * [pgr_trspViaEdges No path representation differences](#pgr_trspviaedges-no-path-representation-differences)
  * [Using a points of interest table](#pgr_trspviaedges-using-the-pointsofinterest-table)

# Introduction
pgr_trsp code has issues that are not being fixed yet, but as time passes and new functionality is added to pgRouting with wrappers to **hide** the issues, not to fix them.

For clarity on the queries:
* _pgr_trsp (with underscore) is the original code
* pgr_trsp (lower case) represents the wrapper calling the original code
* pgr_TRSP (upper case) represents the wrapper calling the replacement function, depending on the function, it can be:
  * pgr_dijkstra
  * pgr_dijkstraVia
  * pgr_withPoints
  * _pgr_withPointsVia

This page intentions is to compare the original code with the wrapped version of the trsp group of functions.
## The restriction
The restriction used in the examples does not have to do anything with the graph:
* No vertex has id: 25, 32 or 33
* No edge has id: 25, 32 or 33
```
SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path;
 to_cost | target_id | via_path
---------+-----------+----------
     100 |        25 | 32, 33
(1 row)

```
The new back end code has the restrictions as follows
```
SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path;
 id | cost | target_id |    path
----+------+-----------+------------
  1 |  100 |        25 | {33,32,25}
(1 row)

```
therefore the shortest path expected are as if there was no restriction involved
# The Vertices signature version
## (Vertices) No path representation differences
Original function code
* Sometimes it crasses the server when no path was found
* Sometimes represents with Error a no path found
* Forcing the user to use the wrapper or the replacement function

Calls to the original function of is no longer allowed without restrictions
```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15, true, true
);
ERROR:  function _pgr_trsp(unknown, integer, integer, boolean, boolean) does not exist
LINE 1: SELECT * FROM _pgr_trsp(
                      ^
HINT:  No function matches the given name and argument types. You might need to add explicit type casts.
```
dijkstra returns EMPTY SET to represent no path found
```
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
(0 rows)

```
pgr_trsp use the pgr_dijkstra when there are no restrictions
therefore returns EMPTY SET to represent no path found
```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15, true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
(0 rows)

```
pgr_trsp use the original code when there are restrictions
therefore throws Error to represent no path found
```
SELECT * FROM pgr_trsp(
     $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
     1, 15, true, true,
     $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
ERROR:  Error computing path: Path Not Found
```
## routing from/to same location
using dijkstra to verify (1 to 1)
```
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 1
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
(0 rows)

```
This call uses the replacement function because there are no restrictions (1 to 1)
therefore is expected to return EMPTY SET to represent no path found
```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1,  true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
(0 rows)

```
call forcing the use of the original code (1 to 1)
* not longer allowed without restrictions

```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1,  true, true
);
ERROR:  function _pgr_trsp(unknown, integer, integer, boolean, boolean) does not exist
LINE 1: SELECT * FROM _pgr_trsp(
                      ^
HINT:  No function matches the given name and argument types. You might need to add explicit type casts.
```
trsp with restrictions (1 to 1) use the original code
is expected to return Error to represent no path found
```
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1,
    true,
    true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |   1 |   1 |    1
   1 |   2 |   4 |    1
   2 |   5 |   8 |    1
   3 |   6 |   9 |    1
   4 |   9 |  16 |    1
   5 |   4 |   3 |    1
   6 |   3 |   2 |    1
   7 |   2 |   1 |    1
   8 |   1 |  -1 |    0
(9 rows)

```
trsp calling the original code with restrictions (1 to 1)
is expected to return Error to represent no path found
but "finds" a path when there should be no path.
```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    1, 1,
    true
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |    1 |    1 |    1 |        0
   2 |        2 |    2 |    4 |    1 |        1
   3 |        3 |    5 |    8 |    1 |        2
   4 |        4 |    6 |    9 |    1 |        3
   5 |        5 |    9 |   16 |    1 |        4
   6 |        6 |    4 |    3 |    1 |        5
   7 |        7 |    3 |    2 |    1 |        6
   8 |        8 |    2 |    1 |    1 |        7
   9 |        9 |    1 |   -1 |    0 |        8
(9 rows)

```
## (vertices) Undirected graph
using Dijkstra to verify the shortest path from (2 to 3) on undirected graph
```
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    2, 3, false
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |    2 |    2 |    1 |        0
   2 |        2 |    3 |   -1 |    0 |        1
(2 rows)

```
using the replacement function because there are no restrictions (2 to 3)
```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    2, 3,
    false,
    true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |   2 |   2 |    1
   1 |   3 |  -1 |    0
(2 rows)

```
call forcing the use of the original code
* not longer allowed without restrictions

```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    2, 3,
    false, true
);
ERROR:  function _pgr_trsp(unknown, integer, integer, boolean, boolean) does not exist
LINE 1: SELECT * FROM _pgr_trsp(
                      ^
HINT:  No function matches the given name and argument types. You might need to add explicit type casts.
```
trsp with restrictions (2 to 3)
does not find the shortest path
```
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    2, 3,
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |   2 |   2 |    1
   1 |   3 |  -1 |    0
(2 rows)

```
calling the original code with restrictions (2 to 3)
does not find the shortest path
```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    2, 3,
    false
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |    2 |    2 |    1 |        0
   2 |        2 |    3 |   -1 |    0 |        1
(2 rows)

```
# The Edges signature version
## (Edges) No path representation differences
Original function code
* Sometimes it crasses the server when no path was found
* Sometimes represents with Error a no path found
* Forcing the user to use the wrapper or the replacement function

Calls to the original function of is no longer allowed without restrictions
```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 17, 0.5, true, true
);
ERROR:  Error computing path: Path Not Found
```
pgr_withPoints returns EMPTY SET to represent no path found
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 1 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 17, 0.5)$$,
    -1, -2
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
(0 rows)

```
## Definition of a path
Remember that one characteristic of a path is that for a path of N edges it has N+1 vertices.

For this example, suppose points, where the pid are different even if the edge and fraction are different.
One point might be on the left side other on the right side, pgr_trsp does not take into account
the side of the point
calls forcing the use of the original code
* not longer allowed without restrictions

```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |    0
(1 row)

```
* with restrictions
```
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |    0
(1 row)

```
Using the *pgr_withPoints* it returns a path of N edge and N+1 vertices
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 1 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 1, 0.5)$$,
    -1, -2
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |   -1 |    1 |    0 |        0
   2 |        2 |   -2 |   -1 |    0 |        0
(2 rows)

```
The *pgr_withPoints* is used when there are no restrictions
```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |    0
   1 |  -2 |  -1 |    0
(2 rows)

```
The original *_pgr_trsp* code is used when there are restrictions
```
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |    0
(1 row)

```
note that pgr_withPoints returns an EMPTY SET when the point is the same
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 1 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 1, 0.5)$$,
    -1, -1
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
(0 rows)

```
## Points on the same edge
The *pgr_withPoints* is used when there are no restrictions
* it returns a path of N edges and N+1 vertices
```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |  0.3
   1 |  -2 |  -1 |    0
(2 rows)

```
The original *_pgr_trsp* is used when there are restrictions
* it returns a path of N edges and N vertex instead of N edge and N+1 vertices
```
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |  0.3
(1 row)

```
## (Edges) Undirected graph
the shortest path obtained with *pgr_withPoints*
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 4 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 2, 0.8)$$,
    -1, -2, directed:=false
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |   -1 |    4 |  0.5 |        0
   2 |        2 |    2 |    2 |  0.8 |      0.5
   3 |        3 |   -2 |   -1 |    0 |      1.3
(3 rows)

```
The original *_pgr_trsp* is used when there are restrictions

```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    4, 0.5, 2, 0.8, false, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   4 |  0.5
   1 |   2 |   2 |  0.8
   2 |  -2 |  -1 |    0
(3 rows)

```
when using restrictions the original *_pgr_trsp* is used internally
* it returns a path of N edges and N vertex instead of N edge and N+1 vertices
* it does not return the shortest path.

```
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    4, 0.5, 2, 0.8, false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   4 |  0.5
   1 |   5 |   8 |    1
   2 |   6 |   9 |    1
   3 |   9 |  16 |    1
   4 |   4 |   3 |    1
   5 |   3 |   2 |  0.2
(6 rows)

```
## Using a points of interest table
The points of interest
```
SELECT * FROM pointsOfInterest;
 pid |  x  |  y  | edge_id | side | fraction |                  the_geom                  |                  newpoint
-----+-----+-----+---------+------+----------+--------------------------------------------+--------------------------------------------
   1 | 1.8 | 0.4 |       1 | l    |      0.4 | 0101000000CDCCCCCCCCCCFC3F9A9999999999D93F | 010100000000000000000000409A9999999999D93F
   2 | 4.2 | 2.4 |      15 | r    |      0.4 | 0101000000CDCCCCCCCCCC10403333333333330340 | 010100000000000000000010403333333333330340
   3 | 2.6 | 3.2 |      12 | l    |      0.6 | 0101000000CDCCCCCCCCCC04409A99999999990940 | 0101000000CDCCCCCCCCCC04400000000000000840
   4 | 0.3 | 1.8 |       6 | r    |      0.3 | 0101000000333333333333D33FCDCCCCCCCCCCFC3F | 0101000000333333333333D33F0000000000000040
   5 | 2.9 | 1.8 |       5 | l    |      0.8 | 01010000003333333333330740CDCCCCCCCCCCFC3F | 01010000000000000000000840CDCCCCCCCCCCFC3F
   6 | 2.2 | 1.7 |       4 | b    |      0.7 | 01010000009A99999999990140333333333333FB3F | 01010000000000000000000040333333333333FB3F
(6 rows)

```
On *pgr_trsp*, to be able to use the table information:
* Each parameter has to be extracted explicitly from the table
* Regardles of the point pid original value
  * will always be -1 for the first point
  * will always be -2 for the second point
```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 6),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 6),
    true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |  0.6
   1 |   2 |   4 |  0.7
   2 |  -2 |  -1 |    0
(3 rows)

```
On *pgr_withPoints*, to be able to use the table information:
* select statement on the *pointsOfInterest* table
* *Negative* the id to represent the id is a pid
* The points pids in the result are not renumbered
* The points pids in the result are represented with negative sign
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -6
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |   -1 |    1 |  0.6 |        0
   2 |        2 |    2 |    4 |  0.7 |      0.6
   3 |        3 |   -6 |   -1 |    0 |      1.3
(3 rows)

```
## routing from/to the same point
using pgr_withPoints to check results
expecting EMPTY SET to represent no path found
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -1
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
(0 rows)

```
This call uses the replacement function because there are no restrictions
* Because the pid is not involved the points are considered different
* it returns a path

```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |    0
   1 |  -2 |  -1 |    0
(2 rows)

```
pgr_trsp with restrictions use the original code
* it returns a path of N edges and N vertex instead of N edge and N+1 vertices

```
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |    0
(1 row)

```
## passing in front of other points
using pgr_withPoints to verify the shortest path from pids (1 to 3)
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -3
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |   -1 |    1 |  0.6 |        0
   2 |        2 |    2 |    4 |    1 |      0.6
   3 |        3 |    5 |   10 |    1 |      1.6
   4 |        4 |   10 |   12 |  0.6 |      2.6
   5 |        5 |   -3 |   -1 |    0 |      3.2
(5 rows)

```
**pgr_withPoints** can be used to see when the route passes in front of other points
In this example point pid=6 is passed in front of
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -3, details:=true
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |   -1 |    1 |  0.6 |        0
   2 |        2 |    2 |    4 |  0.7 |      0.6
   3 |        3 |   -6 |    4 |  0.3 |      1.3
   4 |        4 |    5 |   10 |    1 |      1.6
   5 |        5 |   10 |   12 |  0.6 |      2.6
   6 |        6 |   -3 |   -1 |    0 |      3.2
(6 rows)

```
Can not be used to see if other points are passed in front of.
The pointsOfInterest table is not part of the parameter
```
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 3),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 3),
    true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   1 |  0.6
   1 |   2 |   4 |    1
   2 |   5 |  10 |    1
   3 |  10 |  12 |  0.6
   4 |  -2 |  -1 |    0
(5 rows)

```
## (edges) Routing from a vertex to a point
Showing *pgr_withPoints* results of the shortest path from vid 6 to pid 1
```
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    6, -1
);
 seq | path_seq | node | edge | cost | agg_cost
-----+----------+------+------+------+----------
   1 |        1 |    6 |    8 |    1 |        0
   2 |        2 |    5 |    4 |    1 |        1
   3 |        3 |    2 |    1 |  0.6 |        2
   4 |        4 |   -1 |   -1 |    0 |      2.6
(4 rows)

```
* Vertex 6 is on edge 8 at 1 fraction
* Vertex 6 is also edge 11 at 0 fraction
* Undefined behaviour when at least one of the "points" is an actual vertex

```
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    8, 1,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |  -1 |   8 |    1
   1 |   5 |   4 |    1
   2 |   2 |   1 |  0.6
   3 |  -2 |  -1 |    0
(4 rows)

SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    11, 0,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true
);
 seq | id1 | id2 | cost
-----+-----+-----+------
   0 |   6 |   8 |    1
   1 |   5 |   4 |    1
   2 |   2 |   1 |  0.6
   3 |  -2 |  -1 |    0
(4 rows)

```
# pgr_trspViaVertices
## pgr_trspViaVertices No path representation differences
pgr_trspViaVertices uses:
"* When there are restrictions: `_pgr_trsp(one to one)`"
"* When there are no restrictions: `pgr_dijkstraVia`"

**PLEASE: Use pgr_dijstraVia when there are no restrictions**

Representation of **no path found**:
"* Sometimes represents with Error a no path found"
"* Sometimes represents with EMPTY SET when no path found"
"* Forcing the user to use the wrapper or the replacement function"

Calls to the original function of is no longer allowed without restrictions
```
SELECT * FROM _pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 15, 2],
    false, true
);
ERROR:  Restrictions Missing
```
Calls to the wrapper function allowed without restrictions
```
SELECT * FROM pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[2, 3, 2],
    false, true
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
   1 |   1 |   2 |   2 |    1
   2 |   2 |   3 |   2 |    1
   3 |   2 |   2 |  -1 |    0
(3 rows)

```
But it uses pgr_dijkstraVia that gives more information on the result
```
SELECT * FROM pgr_dijkstraVia(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[2, 3, 2],
    false
);
 seq | path_id | path_seq | start_vid | end_vid | node | edge | cost | agg_cost | route_agg_cost
-----+---------+----------+-----------+---------+------+------+------+----------+----------------
   1 |       1 |        1 |         2 |       3 |    2 |    2 |    1 |        0 |              0
   2 |       1 |        2 |         2 |       3 |    3 |   -1 |    0 |        1 |              1
   3 |       2 |        1 |         3 |       2 |    3 |    2 |    1 |        0 |              1
   4 |       2 |        2 |         3 |       2 |    2 |   -2 |    0 |        1 |              2
(4 rows)

```
## when a path does not exist on the route
pgr_TRSPViaVertices gives different results even if restrictions are nt involved on the
shortest path(s) when restrictions are used VS when restrictions are not used:

Because there is no path from 1 to 1 then there is no complete route 1 to 1 to 2
therefore the expected result is EMPTY SET to represent no route was found
"* without restrictions"
```
SELECT * FROM pgr_TRSPViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 1, 2],
    false, true
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
(0 rows)

```
"* with restrictions"
Restrictions on the wrapper function, is the last parameter and its the old style:
```
SELECT * FROM pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 1, 2],
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
   1 |   1 |   1 |   1 |    1
   2 |   1 |   2 |   4 |    1
   3 |   1 |   5 |   8 |    1
   4 |   1 |   6 |   5 |    1
   5 |   1 |   3 |   2 |    1
   6 |   1 |   2 |   1 |    1
   7 |   2 |   1 |   1 |    1
   8 |   2 |   2 |  -1 |    0
(8 rows)

```
# pgr_trspViaEdges
*pgr_trspViaEdges* will use the original code when
* There are restrictions
* A point is a vertex in disguise (with pcts value of 0)

What it returns
* Error to represent no route
* the points are renumbered to -1, -2 .. -N
* if a point is part of a path it will **not** show on the path

Exaple execution
```
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 2, 1], ARRAY[0.1,0.5,0.5],
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
   1 |   1 |  -1 |   1 |  0.9
   2 |   1 |   2 |   4 |    1
   3 |   1 |   5 |   8 |    1
   4 |   1 |   6 |   9 |    1
   5 |   1 |   9 |  16 |    1
   6 |   1 |   4 |   3 |    1
   7 |   1 |   3 |   2 |    1
   8 |   2 |   2 |   1 |  0.5
(8 rows)

```
A temporay wraper function is used when:
* There are no restrictions
* Before: No point is a vertex in disguise (with pcts value of 0)
* Now: c$Undefined behaviour when a point is a vertex in disguise (with pcts value of 0)

Internaly:
* builds a new graph and calls pgr_dijkstraVia

What it returns
* returns EMPTY SET to represent no route
* edge = -1 in the result to represent the end of a intermediate path
* edge = -2 in the result to represent the end of the last path & route
* the points are renumbered to -1, -2 .. -N
* if a point is part of a path it will show on the path

Note: I do not mention the wrapper name due to the fact that this is not official documentation
Note: I will use *_pgr_withPointsVia* as the wrapper name just for sake of this notes
Example excution
```
SELECT * FROM _pgr_withPointsVia(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 2, 1], ARRAY[0.1,0.5,0.5],
    directed := false
);
 seq | path_id | path_seq | start_vid | end_vid | node | edge | cost | agg_cost | route_agg_cost
-----+---------+----------+-----------+---------+------+------+------+----------+----------------
   1 |       1 |        1 |        -1 |      -2 |   -1 |    1 |  0.1 |        0 |              0
   2 |       1 |        2 |        -1 |      -2 |    2 |    2 |  0.5 |      0.1 |            0.1
   3 |       1 |        3 |        -1 |      -2 |   -2 |   -1 |    0 |      0.6 |            0.6
   4 |       2 |        1 |        -2 |      -3 |   -2 |    2 |  0.5 |        0 |            0.6
   5 |       2 |        2 |        -2 |      -3 |    2 |    1 |  0.5 |      0.5 |            1.1
   6 |       2 |        3 |        -2 |      -3 |   -3 |   -2 |    0 |        1 |            1.6
(6 rows)

```
## pgr_trspViaEdges No path representation differences
This example no path is found (edge 17 is disconnected) from the big graph.
* There is a vertex in disguise (fraction 0 or 1)
* Undefined behaviour

```
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 17, 1], ARRAY[0,0.5,0.5],
    false, true
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
   1 |     |     |     |
(1 row)

```
This example no path is found (edge 17 is disconnected) from the big graph.
* Has a restriction
* *pgr_trspViaEdges* original code is used
* throws error to represent no route was not found

```
SELECT * FROM pgr_trspViaEdges(
$$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
ARRAY[1, 17, 1], ARRAY[0.5,0.5,0.5],
false, true,
$$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, 32, 33::TEXT AS via_path$$
);
server closed the connection unexpectedly
This probably means the server terminated abnormally
before or while processing the request.
The connection to the server was lost. Attempting reset: Failed.
```
This example no path is found (edge 17 is disconnected) from the big graph.
* *_pgr_withPointsVia* is used
* returns EMPTY SET to represent no route

```
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 17, 1], ARRAY[0.1,0.5,0.5],
    false,
    true
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
(0 rows)

```
## (pgr_trspViaEdges) Using the pointsOfInterest table
Only routing points
when *_pgr_withPointsVia* is used
* the renumbering still takes place

```
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 3),
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 6)],
    ARRAY[
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 3),
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 6)],
    false,
    true
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
   1 |   1 |  -1 |   1 |  0.4
   2 |   1 |   2 |   4 |  0.3
   3 |   1 |  -3 |   4 |  0.3
   4 |   1 |   5 |  10 |    1
   5 |   1 |  10 |  12 |  0.6
   6 |   1 |  -2 |  -1 |    0
   7 |   2 |  -2 |  12 |  0.6
   8 |   2 |  10 |  10 |    1
   9 |   2 |   5 |   4 |  0.3
  10 |   2 |  -3 |  -2 |    0
(10 rows)

```
Only routing points
when originalcode is used (because there is a restriction)

```
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 3),
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 6)],
    ARRAY[
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 3),
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 6)],
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
   1 |   1 |  -1 |   1 |  0.6
   2 |   1 |   2 |   4 |    1
   3 |   1 |   5 |  10 |    1
   4 |   1 |  10 |  12 |    1
   5 |   2 |  11 |  13 |    1
   6 |   2 |  12 |  15 |    1
   7 |   2 |   9 |   9 |    1
   8 |   2 |   6 |   8 |    1
   9 |   2 |   5 |   4 |  0.3
(9 rows)

```
Routing points & vertices
* vertex 6 is on edge 11 with fraction 0
* Undefined behavior
```
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
        (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 3),
        11],
    ARRAY[
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
        (SELECT fraction  FROM pointsOfInterest WHERE pid = 3),
        0],
    false,
    true
);
 seq | id1 | id2 | id3 | cost
-----+-----+-----+-----+------
   1 |   1 |  -1 |   1 |  0.6
   2 |   1 |   2 |   4 |    1
   3 |   1 |   5 |  10 |    1
   4 |   1 |  10 |  12 |  0.6
   5 |   2 |  -1 |  12 |  0.4
   6 |   2 |  11 |  11 |    1
   7 |   2 |   6 |  -1 |    0
(7 rows)

```
BEGIN;
BEGIN
ROLLBACK;
ROLLBACK
