ROLLBACK;
\echo # Notes on pgr_trsp for version 2.6.0

\echo Table of contents

\echo * [Introduction](#introduction)
\echo '  * [The restriction](#the-restriction)'
\echo * [The Vertices Signature Version](#the-vertices-signature-version)
\echo '  * [No path representation differences](#vertices-no-path-representation-differences)'
\echo '  * [Routing from/to same location](#routing-fromto-same-location)'
\echo '  * [Undirected graph](#vertices-undirected-graph)'
\echo * [The Edges Signature Version](#the-edges-signature-version)
\echo '  * [No path representation differences](#edges-no-path-representation-differences)'
\echo '  * [Definition of a path](#definition-of-a-path)'
\echo '  * [Points on the same edge](#points-on-the-same-edge)'
\echo '  * [Undirected graph](#edges-undirected-graph)'
\echo '  * [Using a points of interest table](#using-a-points-of-interest-table)'
\echo '  * [Routing from/to the same point](#routing-fromto-the-same-point)'
\echo '  * [Passing in front of other points](#passing-in-front-of-other-points)'
\echo '  * [Routing from a vertex to a point](#edges-routing-from-a-vertex-to-a-point)'
\echo * [pgr_trspViaVertices](#pgr_trspviavertices)
\echo '  * [pgr_trspViaVertices No path representation differences](#pgr_trspviavertices-no-path-representation-differences)'
\echo '  * [when a path does not exist on the route](#when-a-path-does-not-exist-on-the-route)'
\echo '  * [from 2 to 3 to 2](#from-2-to-3-to-2)'
\echo * [pgr_trspViaEdges](#pgr_trspviaedges)
\echo '  * [pgr_trspViaEdges No path representation differences](#pgr_trspviaedges-no-path-representation-differences)'
\echo '  * [Using a points of interest table](#pgr_trspviaedges-using-the-pointsofinterest-table)'
\echo



\echo # Introduction
\echo pgr_trsp code has issues that are not being fixed yet, but as time passes and new functionality is added to pgRouting with wrappers to **hide** the issues, not to fix them.
\echo
\echo For clarity on the queries:
\echo * _pgr_trsp (with underscore) is the original code
\echo * pgr_trsp (lower case) represents the wrapper calling the original code
\echo * pgr_TRSP (upper case) represents the wrapper calling the replacement function, depending on the function, it can be:
\echo '  * pgr_dijkstra'
\echo '  * pgr_dijkstraVia'
\echo '  * pgr_withPoints'
\echo '  * _pgr_withPointsVia'
\echo

\echo This page intentions is to compare the original code with the wrapped version of the trsp group of functions.
\echo ## The restriction

\echo The restriction used in the examples does not have to do anything with the graph:
\echo * No vertex has id: 25, 32 or 33
\echo * No edge has id: 25, 32 or 33
\echo '\`\`\`'
SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path;
\echo '\`\`\`'
\echo The new back end code has the restrictions as follows
\echo '\`\`\`'
SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path;
\echo '\`\`\`'

\echo therefore the shortest path expected are as if there was no restriction involved


-------------------------------------------------------------------------------------------
\echo # The Vertices signature version
------------------
\echo ## (Vertices) No path representation differences
\echo Original function code
\echo  * Sometimes it crasses the server when no path was found
\echo  * Sometimes represents with Error a no path found
\echo  * Forcing the user to use the wrapper or the replacement function
\echo
\echo Calls to the original function of is no longer allowed without restrictions
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15, true, true
);
\echo '\`\`\`'

\echo dijkstra returns EMPTY SET to represent no path found
\echo '\`\`\`'
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15
);
\echo '\`\`\`'

\echo pgr_trsp use the pgr_dijkstra when there are no restrictions
\echo therefore returns EMPTY SET to represent no path found
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15, true, true
);
\echo '\`\`\`'

\echo pgr_trsp use the original code  when there are restrictions
\echo therefore throws Error to represent no path found
\echo '\`\`\`'
\set VERBOSITY terse
SELECT * FROM pgr_trsp(
     $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
     1, 15, true, true,
     $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\set VERBOSITY default
\echo '\`\`\`'


------------------
\echo ## routing from/to same location

\echo using dijkstra to verify (1 to 1)
\echo '\`\`\`'
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 1
);
\echo '\`\`\`'


\echo This call uses the replacement function because there are no restrictions (1 to 1)
\echo therefore is expected to return EMPTY SET to represent no path found
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1,  true, true
);
\echo '\`\`\`'


\echo call forcing the use of the original code (1 to 1)
\echo * not longer allowed without restrictions
\echo
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1,  true, true
);
\echo '\`\`\`'


\echo trsp with restrictions (1 to 1) use the original code
\echo is expected to return Error to represent no path found
\echo '\`\`\`'
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1,
    true,
    true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo '\`\`\`'

\echo trsp calling the original code with restrictions (1 to 1)
\echo is expected to return Error to represent no path found
\echo but "finds" a path when there should be no path.
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    1, 1,
    true
);
\echo '\`\`\`'


------------------
\echo ## (vertices) Undirected graph

\echo using Dijkstra to verify the shortest path from (2 to 3) on undirected graph
\echo '\`\`\`'
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    2, 3, false
);
\echo '\`\`\`'

\echo using the replacement function because there are no restrictions (2 to 3)
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    2, 3,
    false,
    true
);
\echo '\`\`\`'

\echo call forcing the use of the original code
\echo * not longer allowed without restrictions
\echo
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    2, 3,
    false, true
);
\echo '\`\`\`'

\echo trsp with restrictions (2 to 3)
\echo does not find the shortest path
\echo '\`\`\`'
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    2, 3,
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo '\`\`\`'

\echo calling the original code with restrictions (2 to 3)
\echo does not find the shortest path
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    2, 3,
    false
);
\echo '\`\`\`'

-----------------------------------------------------------------------------------------------
\echo # The Edges signature version

------------------HERE
\echo ## (Edges) No path representation differences
\echo Original function code
\echo  * Sometimes it crasses the server when no path was found
\echo  * Sometimes represents with Error a no path found
\echo  * Forcing the user to use the wrapper or the replacement function
\echo
\echo Calls to the original function of is no longer allowed without restrictions
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 17, 0.5, true, true
);
\echo '\`\`\`'
\echo pgr_withPoints returns EMPTY SET to represent no path found
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 1 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 17, 0.5)$$,
    -1, -2
);
\echo '\`\`\`'


------------------
\echo ## Definition of a path
\echo Remember that one characteristic of a path is that for a path of N edges it has N+1 vertices.
\echo
\echo For this example, suppose points, where the pid are different even if the edge and fraction are different.
\echo One point might be on the left side other on the right side, pgr_trsp does not take into account
\echo the side of the point

\echo calls forcing the use of the original code
\echo * not longer allowed without restrictions
\echo
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true
);
\echo '\`\`\`'
\echo * with restrictions
\echo '\`\`\`'
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo '\`\`\`'

\echo Using the *pgr_withPoints* it returns a path of N edge and N+1 vertices
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 1 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 1, 0.5)$$,
    -1, -2
);
\echo '\`\`\`'

\echo The *pgr_withPoints* is used when there are no restrictions
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true
);
\echo '\`\`\`'

\echo The original *_pgr_trsp* code is used when there are restrictions
\echo '\`\`\`'
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo '\`\`\`'

\echo note that pgr_withPoints returns an EMPTY SET when the point is the same
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 1 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 1, 0.5)$$,
    -1, -1
);
\echo '\`\`\`'

------------------
\echo ## Points on the same edge

\echo The *pgr_withPoints* is used when there are no restrictions
\echo * it returns a path of N edges and N+1 vertices
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true, true
);
\echo '\`\`\`'

\echo The original *_pgr_trsp* is used when there are restrictions
\echo * it returns a path of N edges and N vertex instead of N edge and N+1 vertices
\echo '\`\`\`'
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$

);
\echo '\`\`\`'

------------------
\echo ## (Edges) Undirected graph
\echo the shortest path obtained with *pgr_withPoints*
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$(SELECT 1 as pid, 4 as edge_id, 0.5::float as fraction)
    UNION
    (SELECT 2, 2, 0.8)$$,
    -1, -2, directed:=false
);
\echo '\`\`\`'

\echo The original *_pgr_trsp* is used when there are restrictions
\echo
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    4, 0.5, 2, 0.8, false, true
);
\echo '\`\`\`'

\echo when using restrictions the original *_pgr_trsp* is used internally
\echo * it returns a path of N edges and N vertex instead of N edge and N+1 vertices
\echo * it does not return the shortest path.
\echo
\echo '\`\`\`'
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    4, 0.5, 2, 0.8, false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo '\`\`\`'


----------------
\echo ## Using a points of interest table
\echo The points of interest
\echo '\`\`\`'
SELECT * FROM pointsOfInterest;
\echo '\`\`\`'

\echo On *pgr_trsp*, to be able to use the table information:
\echo * Each parameter has to be extracted explicitly from the table
\echo * Regardles of the point pid original value
\echo '  * will always be -1 for the first point'
\echo '  * will always be -2 for the second point'
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 6),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 6),
    true, true
);
\echo '\`\`\`'

\echo On *pgr_withPoints*, to be able to use the table information:
\echo * select statement on the *pointsOfInterest* table
\echo * *Negative* the id to represent the id is a pid
\echo * The points pids in the result are not renumbered
\echo * The points pids in the result are represented with negative sign
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -6
);
\echo '\`\`\`'


----------------
\echo ## routing from/to the same point

\echo using pgr_withPoints to check results
\echo expecting EMPTY SET to represent no path found
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -1
);
\echo '\`\`\`'


\echo This call uses the replacement function because there are no restrictions
\echo * Because the pid is not involved the points are considered different
\echo * it returns a path
\echo
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true
);
\echo '\`\`\`'


\echo pgr_trsp with restrictions use the original code
\echo * it returns a path of N edges and N vertex instead of N edge and N+1 vertices
\echo
\echo '\`\`\`'
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo '\`\`\`'


----------------
\echo ## passing in front of other points

\echo using pgr_withPoints to verify the shortest path from pids (1 to 3)
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -3
);
\echo '\`\`\`'

\echo **pgr_withPoints** can be used to see when the route passes in front of other points
\echo In this example point pid=6 is passed in front of
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -3, details:=true
);
\echo '\`\`\`'

\echo Can not be used to see if other points are passed in front of.
\echo The pointsOfInterest table is not part of the parameter
\echo '\`\`\`'
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 3),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 3),
    true, true
);
\echo '\`\`\`'

----------------
\echo ## (edges) Routing from a vertex to a point

\echo Showing *pgr_withPoints* results of the shortest path from vid 6 to pid 1
\echo '\`\`\`'
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    6, -1
);
\echo '\`\`\`'

\echo * Vertex 6 is on edge 8 at 1 fraction
\echo * Vertex 6 is also edge 11 at 0 fraction
\echo * Undefined behaviour when at least one of the "points" is an actual vertex
\echo
\echo '\`\`\`'
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    8, 1,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true
);
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    11, 0,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true
);
\echo '\`\`\`'

---------------------------------------------------------------

\echo # pgr_trspViaVertices
------------------
\echo ## pgr_trspViaVertices No path representation differences

\echo pgr_trspViaVertices uses:
\echo "* When there are restrictions: `_pgr_trsp(one to one)`"
\echo "* When there are no restrictions: `pgr_dijkstraVia`"
\echo
\echo **PLEASE: Use pgr_dijstraVia when there are no restrictions**
\echo
\echo Representation of **no path found**:
\echo "* Sometimes represents with Error a no path found"
\echo "* Sometimes represents with EMPTY SET when no path found"
\echo "* Forcing the user to use the wrapper or the replacement function"
\echo
\echo Calls to the original function of is no longer allowed without restrictions
\echo '\`\`\`'
\set VERBOSITY terse
SELECT * FROM _pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 15, 2],
    false, true
);
\set VERBOSITY default
\echo '\`\`\`'

\echo Calls to the wrapper function allowed without restrictions
\echo '\`\`\`'
SELECT * FROM pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[2, 3, 2],
    false, true
);
\echo '\`\`\`'

\echo But it uses pgr_dijkstraVia that gives more information on the result
\echo '\`\`\`'
SELECT * FROM pgr_dijkstraVia(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[2, 3, 2],
    false
);
\echo '\`\`\`'


\echo ## when a path does not exist on the route

\echo pgr_TRSPViaVertices gives different results even if restrictions are nt involved on the
\echo shortest path(s) when restrictions are used VS when restrictions are not used:
\echo
\echo Because there is no path from 1 to 1 then there is no complete route 1 to 1 to 2
\echo therefore the expected result is EMPTY SET to represent no route was found
\echo "* without restrictions"
\echo '\`\`\`'
SELECT * FROM pgr_TRSPViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 1, 2],
    false, true
);
\echo '\`\`\`'

\echo "* with restrictions"
\echo Restrictions on the wrapper function, is the last parameter and its the old style:
\echo '\`\`\`'
SELECT * FROM pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 1, 2],
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo '\`\`\`'





---------------------------------------------------------------

\echo # pgr_trspViaEdges
------------------

\echo *pgr_trspViaEdges* will use the original code when
\echo * There are restrictions
\echo * A point is a vertex in disguise (with pcts value of 0)
\echo
\echo What it returns
\echo * Error to represent no route
\echo * the points are renumbered to -1, -2 .. -N
\echo * if a point is part of a path it will **not** show on the path
\echo

\echo Exaple execution
\echo '\`\`\`'
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 2, 1], ARRAY[0.1,0.5,0.5],
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);

\echo '\`\`\`'
\echo A temporay wraper function is used when:
\echo * There are no restrictions
\echo * Before: No point is a vertex in disguise (with pcts value of 0)
\echo * Now: c$Undefined behaviour when a point is a vertex in disguise (with pcts value of 0)
\echo
\echo Internaly:
\echo * builds a new graph and calls pgr_dijkstraVia
\echo
\echo What it returns
\echo * returns EMPTY SET to represent no route
\echo * edge = -1 in the result to represent the end of a intermediate path
\echo * edge = -2 in the result to represent the end of the last path & route
\echo * the points are renumbered to -1, -2 .. -N
\echo * if a point is part of a path it will show on the path
\echo
\echo Note: I do not mention the wrapper name due to the fact that this is not official documentation
\echo Note: I will use *_pgr_withPointsVia* as the wrapper name just for sake of this notes

\echo Example excution
\echo '\`\`\`'
SELECT * FROM _pgr_withPointsVia(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 2, 1], ARRAY[0.1,0.5,0.5],
    directed := false
);
\echo '\`\`\`'


\echo ## pgr_trspViaEdges No path representation differences
----------------

\echo This example no path is found (edge 17 is disconnected) from the big graph.
\echo * There is a vertex in disguise (fraction 0 or 1)
\echo * Undefined behaviour
\echo
\echo '\`\`\`'
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 17, 1], ARRAY[0,0.5,0.5],
    false, true
);
\echo '\`\`\`'


\echo This example no path is found (edge 17 is disconnected) from the big graph.
\echo * Has a restriction
\echo * *pgr_trspViaEdges* original code is used
\echo * throws error to represent no route was not found
\echo
\echo '\`\`\`'
\echo SELECT * FROM pgr_trspViaEdges(
\echo     $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
\echo     ARRAY[1, 17, 1], ARRAY[0.5,0.5,0.5],
\echo     false, true,
\echo     $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
\echo );
\echo server closed the connection unexpectedly
\echo This probably means the server terminated abnormally
\echo before or while processing the request.
\echo The connection to the server was lost. Attempting reset: Failed.
\echo '\`\`\`'

\echo This example no path is found (edge 17 is disconnected) from the big graph.
\echo * *_pgr_withPointsVia* is used
\echo * returns EMPTY SET to represent no route
\echo
\echo '\`\`\`'
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 17, 1], ARRAY[0.1,0.5,0.5],
    false,
    true
);
\echo '\`\`\`'

----------------
\echo ## (pgr_trspViaEdges) Using the pointsOfInterest table

\echo Only routing points
\echo when *_pgr_withPointsVia* is used
\echo * the renumbering still takes place
\echo
\echo '\`\`\`'
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
\echo '\`\`\`'

\echo Only routing points
\echo when originalcode is used (because there is a restriction)
\echo
\echo '\`\`\`'
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
\echo '\`\`\`'

\echo Routing points & vertices
\echo * vertex 6 is on edge 11 with fraction 0
\echo  * Undefined behavior
\echo '\`\`\`'
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
\echo '\`\`\`'
BEGIN;
