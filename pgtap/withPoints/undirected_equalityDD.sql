
\i setup.sql

SELECT PLAN(3);


-- because the graph is undirected, It will not matter the side of the point

--- UNDIRECTED GRAPH
---


-------- both driving sides
PREPARE q1 AS
SELECT seq, node, edge, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 4.8, driving_side := 'b', details := true, directed:=false)
ORDER BY seq;

-------- right driving side

PREPARE q2 AS
SELECT seq, node, edge, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 4.8, driving_side := 'r', details := true, directed:=false)
ORDER BY seq;


-------- left driving side

PREPARE q3 AS
SELECT seq, node, edge, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 4.8, driving_side := 'l', details := true, directed:=false)
ORDER BY seq;


SELECT set_eq('q1', 'q2',
    'Should be equal: both driving sides with right driving side');

SELECT set_eq('q1', 'q3',
    'Should be equal: both driving sides with left driving side');

SELECT set_eq('q2', 'q3',
    'Should be equal: right driving sides with left driving side');

SELECT * FROM finish();
ROLLBACK;
