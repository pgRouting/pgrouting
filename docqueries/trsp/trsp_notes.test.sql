ROLLBACK;
SET extra_float_digits=-3;

\set VERBOSITY terse
\echo --place1
SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path;
\echo --place2
SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path;
\echo --place3
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15, true, true
);
\echo --place4
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15
);
\echo --place5
SELECT * FROM pgr_trsp(
     $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
     1, 15, true, true,
     $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);

\echo --place6
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1,  true, true
);
\echo --place7
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    14, 14, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place8
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 1, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place9
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
    2, 3, false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place10
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    2, 3, false, false,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place11
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 17, 0.5, true, true
);
\echo --place12
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 17, 0.5, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);

\echo --place13
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true, true
);
\echo --place14
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place15
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true
);
\echo --place16
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place17
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
    1, 0.5, 1, 0.8, false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place18
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    1, 0.5, 1, 0.8, false, false,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place19

-- using a points of interest table
SELECT * FROM pointsOfInterest;
\echo --place20
SELECT * FROM pgr_TRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 6),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 6),
    true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place21
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -6
);

-- vertex to point
\echo --place22
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    8, 1,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place23
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    11, 0,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
\echo --place24
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    6, -1
);
\echo --place25


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
\set VERBOSITY default
BEGIN;
