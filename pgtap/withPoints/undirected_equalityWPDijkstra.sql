
\i setup.sql

SELECT PLAN(12);




---
-- UNDIRECTED GRAPH
--  Because the graph is undirected no matter the driving side
-- the results should be the same

---------------------------
-- No Details
---------------------------

------------------
---- FROM -1 to 7
------------------
SELECT
    seq::BIGINT, node::BIGINT, edge::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test1
FROM
(VALUES
    (   1 ,   -1 ,    1 ,  0.6 ,        0),
    (   2 ,    2 ,    4 ,    1 ,      0.6),
    (   3 ,    5 ,    7 ,    1 ,      1.6),
    (   4 ,    8 ,    6 ,    1 ,      2.6),
    (   5 ,    7 ,   -1 ,    0 ,      3.6)
) AS t (seq, node, edge, cost, agg_cost)
ORDER BY seq;

-------- both driving sides

PREPARE q1 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 7, driving_side := 'b', details := false, directed:=false)
ORDER BY seq;

SELECT set_eq('q1',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test1$$,
    'Should be aggregating individual costs: both driving sides');



-------- right driving side

PREPARE q2 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 7, driving_side := 'r', details := false, directed:=false)
ORDER BY seq;


SELECT set_eq('q2',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test1$$,
    'Should be aggregating individual costs: right driving side');

-------- left driving side

PREPARE q3 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 7, driving_side := 'l', details := false, directed:=false)
ORDER BY seq;


SELECT set_eq('q3',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test1$$,
    'Should be aggregating individual costs: left driving side');



------------------
---- FROM 12 to -1
------------------

-- all results on udirected graph are "allegedly" equal
SELECT
    seq::BIGINT, node::BIGINT, edge::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test2
FROM
(VALUES
    (   1 ,   12 ,   15 , 1    , 0),
    (   2 ,    9 ,   16 , 1    , 1),
    (   3 ,    4 ,    3 , 1    , 2),
    (   4 ,    3 ,    2 , 1    , 3),
    (   5 ,    2 ,    1 , 0.6  , 4),
    (   6 ,   -1 ,   -1 , 0    , 4.6)
) AS t (seq, node, edge, cost, agg_cost)
ORDER BY seq;

-------- both driving sides

PREPARE q4 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    12, -1, driving_side := 'b', details := false, directed:=false)
ORDER BY seq;

SELECT set_eq('q4',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test2$$,
    'Should be aggregating individual costs: both driving sides');



-------- right driving side

PREPARE q5 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    12, -1, driving_side := 'r', details := false, directed:=false)
ORDER BY seq;


SELECT set_eq('q5',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test2$$,
    'Should be aggregating individual costs: right driving side');

-------- left driving side

PREPARE q6 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    12, -1, driving_side := 'l', details := false, directed:=false)
ORDER BY seq;


SELECT set_eq('q6',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test2$$,
    'Should be aggregating individual costs: left driving side');



---------------------------
-- With Details
---------------------------

------------------
---- FROM 1 to 7
------------------
SELECT
    node::BIGINT, edge::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test3
FROM
(VALUES
   (     1 ,    1 , 0.4  , 0),
   (    -1 ,    1 , 0.6  , 0.4),
   (     2 ,    4 , 0.7  , 1),
   (    -6 ,    4 , 0.3  , 1.7),
   (     5 ,    7 , 1    , 2),
   (     8 ,    6 , 0.7  , 3),
   (    -4 ,    6 , 0.3  , 3.7),
   (     7 ,   -1 , 0    , 4)
) AS t (node, edge, cost, agg_cost);

-------- both driving sides

PREPARE q7 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    1, 7, driving_side := 'b', details := true, directed:=false);

SELECT set_eq('q7',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test3$$,
    '1, 7, driving_side := b, details := true, directed:=false');




-------- right driving side

PREPARE q8 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    1, 7, driving_side := 'r', details := true, directed:=false)
ORDER BY seq;


SELECT set_eq('q8',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test3$$,
    '1, 7, driving_side := r, details := true, directed:=false');

-------- left driving side

PREPARE q9 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    1, 7, driving_side := 'l', details := true, directed:=false)
ORDER BY seq;


SELECT set_eq('q9',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test3$$,
    '1, 7, driving_side := l, details := true, directed:=false');


------------------
---- FROM 12 to -1
------------------

-- all results on udirected graph are "allegedly" equal
SELECT
    node::BIGINT, edge::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test4
FROM
(VALUES
    (  12 ,   15 , 0.6  , 0),
    (  -2 ,   15 , 0.4  , 0.6),
    (   9 ,   16 , 1    , 1),
    (   4 ,    3 , 1    , 2),
    (   3 ,    2 , 1    , 3),
    (   2 ,    1 , 0.6  , 4),
    (  -1 ,   -1 , 0    , 4.6)
) AS t (node, edge, cost, agg_cost);

-------- both driving sides

PREPARE q10 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    12, -1, driving_side := 'b', details := true, directed:=false);

SELECT set_eq('q10',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test4$$,
    '12, -1, driving_side := b, details := true, directed:=false');



-------- right driving side

PREPARE q11 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    12, -1, driving_side := 'r', details := true, directed:=false);


SELECT set_eq('q11',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test4$$,
    '12, -1, driving_side := r, details := true, directed:=false');

-------- left driving side

PREPARE q12 AS
SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    12, -1, driving_side := 'l', details := true, directed:=false);


SELECT set_eq('q12',
    $$SELECT  node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM test4$$,
    '12, -1, driving_side := l, details := true, directed:=false');



SELECT * FROM finish();
ROLLBACK;
