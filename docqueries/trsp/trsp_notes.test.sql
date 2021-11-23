ROLLBACK;
set LOG_ERROR_VERBOSITY TO terse;
SET extra_float_digits=-3;

/* --place1 */
SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path;
/* --place2 */
SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path;
/* --place3 */
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    1, 15, true
);

/* --place4 */
SELECT * FROM pgr_dijkstra(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 15
);
/* --place5 */
SELECT * FROM pgr_trsp(
     $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
     $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
     1, 15, true
);

/* --place6 */
SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    1, 1,  true
  );
  /* --place7 */
  SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    14, 14, true
  );
  /* --place8 */
  SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    1, 1, true
  );
  /* --place9 */
  SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    2, 3, false
  );
  /* --place10 */
  SELECT * FROM pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
    $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
    2, 3, false
  );
  /* --place11 */
  SELECT * FROM pgr_withpointstrsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 17, 0.5, true
  );

/* --place12 */
/* OLD CODE is not reachable any more
SELECT * FROM _pgr_trsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 17, 0.5, true, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
*/
/* --place13 */
SELECT * FROM pgr_withpointsTRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true
);
/* --place14 */
SELECT * FROM pgr_withpointsTRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.8, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
/* --place15 */
SELECT * FROM pgr_withpointsTRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true
);
/* --place16 */
SELECT * FROM pgr_withpointsTRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    1, 0.5, 1, 0.5, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
/* --place17 */
SELECT * FROM pgr_withpointstrsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
    1, 0.5, 1, 0.8, false,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
/* --place18 */
SELECT * FROM pgr_withpointstrsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
    1, 0.5, 1, 0.8, false,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
/* --place19 */

-- using a points of interest table
SELECT * FROM pointsOfInterest;
/* --place20 */
SELECT * FROM pgr_withpointsTRSP(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 6),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 6),
    true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
/* --place21 */
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    -1, -6
);

-- vertex to point
/* --place22 */
SELECT * FROM pgr_withpointstrsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    8, 1,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
/* --place23 */
SELECT * FROM pgr_withpointstrsp(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    11, 0,
    (SELECT edge_id::INTEGER  FROM pointsOfInterest WHERE pid = 1),
    (SELECT fraction  FROM pointsOfInterest WHERE pid = 1),
    true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
/* --place24 */
SELECT * FROM pgr_withPoints(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost  FROM edge_table$$,
    $$SELECT pid, edge_id, fraction FROM pointsOfInterest$$,
    6, -1
);
/* --place25 */


-- # pgr_trspViaVertices
------------------
-- ## pgr_trspViaVertices No path representation differences

-- pgr_trspViaVertices uses:
-- "* When there are restrictions: `_pgr_trsp(one to one)`"
-- "* When there are no restrictions: `pgr_dijkstraVia`"
-- **PLEASE: Use pgr_dijstraVia when there are no restrictions**
-- Representation of **no path found**:
-- "* Sometimes represents with Error a no path found"
-- "* Sometimes represents with EMPTY SET when no path found"
-- "* Forcing the user to use the wrapper or the replacement function"
-- Calls to the original function of is no longer allowed without restrictions
-- '\`\`\`'
SET LOG_ERROR_VERBOSITY TO terse;
SELECT * FROM _pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 15, 2],
    false, true
);
SET LOG_ERROR_VERBOSITY TO terse;
-- '\`\`\`'

-- Calls to the wrapper function allowed without restrictions
-- '\`\`\`'
SELECT * FROM pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[2, 3, 2],
    false, true
);
-- '\`\`\`'

-- But it uses pgr_dijkstraVia that gives more information on the result
-- '\`\`\`'
SELECT * FROM pgr_dijkstraVia(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[2, 3, 2],
    false
);
-- '\`\`\`'


-- ## when a path does not exist on the route

-- pgr_TRSPViaVertices gives different results even if restrictions are nt involved on the
-- shortest path(s) when restrictions are used VS when restrictions are not used:
-- Because there is no path from 1 to 1 then there is no complete route 1 to 1 to 2
-- therefore the expected result is EMPTY SET to represent no route was found
-- "* without restrictions"
-- '\`\`\`'
SELECT * FROM pgr_TRSPViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 1, 2],
    false, true
);
-- '\`\`\`'

-- "* with restrictions"
-- Restrictions on the wrapper function, is the last parameter and its the old style:
-- '\`\`\`'
SELECT * FROM pgr_trspViaVertices(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 1, 2],
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);
-- '\`\`\`'





---------------------------------------------------------------

-- # pgr_trspViaEdges
------------------

-- *pgr_trspViaEdges* will use the original code when
-- * There are restrictions
-- * A point is a vertex in disguise (with pcts value of 0)
-- What it returns
-- * Error to represent no route
-- * the points are renumbered to -1, -2 .. -N
-- * if a point is part of a path it will **not** show on the path

-- Exaple execution
-- '\`\`\`'
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 2, 1], ARRAY[0.1,0.5,0.5],
    false, true,
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
);

-- '\`\`\`'
-- A temporay wraper function is used when:
-- * There are no restrictions
-- * Before: No point is a vertex in disguise (with pcts value of 0)
-- * Now: c$Undefined behaviour when a point is a vertex in disguise (with pcts value of 0)
-- Internaly:
-- * builds a new graph and calls pgr_dijkstraVia
-- What it returns
-- * returns EMPTY SET to represent no route
-- * edge = -1 in the result to represent the end of a intermediate path
-- * edge = -2 in the result to represent the end of the last path & route
-- * the points are renumbered to -1, -2 .. -N
-- * if a point is part of a path it will show on the path
-- Note: I do not mention the wrapper name due to the fact that this is not official documentation
-- Note: I will use *_pgr_withPointsVia* as the wrapper name just for sake of this notes

-- Example excution
-- '\`\`\`'
SELECT * FROM _pgr_withPointsVia(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 2, 1], ARRAY[0.1,0.5,0.5],
    directed := false
);
-- '\`\`\`'


-- ## pgr_trspViaEdges No path representation differences
----------------

-- This example no path is found (edge 17 is disconnected) from the big graph.
-- * There is a vertex in disguise (fraction 0 or 1)
-- * Undefined behaviour
-- '\`\`\`'
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 17, 1], ARRAY[0,0.5,0.5],
    false, true
);
-- '\`\`\`'


-- This example no path is found (edge 17 is disconnected) from the big graph.
-- * Has a restriction
-- * *pgr_trspViaEdges* original code is used
-- * throws error to represent no route was not found

-- '\`\`\`'
-- SELECT * FROM pgr_trspViaEdges(
--     $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
--     ARRAY[1, 17, 1], ARRAY[0.5,0.5,0.5],
--     false, true,
--     $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
-- );
-- server closed the connection unexpectedly
-- This probably means the server terminated abnormally
-- before or while processing the request.
-- The connection to the server was lost. Attempting reset: Failed.
-- '\`\`\`'

-- This example no path is found (edge 17 is disconnected) from the big graph.
-- * *_pgr_withPointsVia* is used
-- * returns EMPTY SET to represent no route

-- '\`\`\`'
SELECT * FROM pgr_trspViaEdges(
    $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$$,
    ARRAY[1, 17, 1], ARRAY[0.1,0.5,0.5],
    false,
    true
);
-- '\`\`\`'

----------------
-- ## (pgr_trspViaEdges) Using the pointsOfInterest table

-- Only routing points
-- when *_pgr_withPointsVia* is used
-- * the renumbering still takes place

-- '\`\`\`'
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
-- '\`\`\`'

-- Only routing points
-- when originalcode is used (because there is a restriction)

-- '\`\`\`'
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
-- '\`\`\`'

-- Routing points & vertices
-- * vertex 6 is on edge 11 with fraction 0
--  * Undefined behavior
-- '\`\`\`'
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
-- '\`\`\`'
set LOG_ERROR_VERBOSITY TO default;
BEGIN;
