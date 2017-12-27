
\i setup.sql

SELECT PLAN(6);


---
--- DIRECTED GRAPH
---
-------- both driving sides

PREPARE q1 AS
SELECT node, edge, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 6.8, driving_side := 'b', details := false)
ORDER BY seq;


SELECT
    node::BIGINT, edge::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test1
FROM
(VALUES
    (  -1 ,   -1 ,    0 ,        0),
    (   1 ,    1 ,  0.4 ,      0.4),
    (   2 ,    1 ,  0.6 ,      0.6),
    (   5 ,    4 ,    1 ,      1.6),
    (   6 ,    8 ,    1 ,      2.6),
    (   8 ,    7 ,    1 ,      2.6),
    (  10 ,   10 ,    1 ,      2.6),
    (   7 ,    6 ,    1 ,      3.6),
    (   9 ,    9 ,    1 ,      3.6),
    (  11 ,   11 ,    1 ,      3.6),
    (  13 ,   14 ,    1 ,      3.6),
    (   4 ,   16 ,    1 ,      4.6),
    (  12 ,   13 ,    1 ,      4.6),
    (   3 ,    3 ,    1 ,      5.6)
) AS t (node, edge, cost, agg_cost);


SELECT set_eq('q1',
    $$SELECT node, edge, cost::TEXT, agg_cost::TEXT FROM test1$$,
    'Should be aggregating individual costs: both driving sides, DIR');



-------- right driving side

PREPARE q2 AS
SELECT node, edge, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 6.8, driving_side := 'r', details := false);


SELECT
    node::BIGINT, edge::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test2
FROM
(VALUES
    (  -1 ,   -1 ,    0 ,        0),
    (   1 ,    1 ,  0.4 ,      0.4),
    (   2 ,    1 ,    1 ,      1.4),
    (   5 ,    4 ,    1 ,      2.4),
    (   6 ,    8 ,    1 ,      3.4),
    (   8 ,    7 ,    1 ,      3.4),
    (  10 ,   10 ,    1 ,      3.4),
    (   7 ,    6 ,    1 ,      4.4),
    (   9 ,    9 ,    1 ,      4.4),
    (  11 ,   11 ,    1 ,      4.4),
    (  13 ,   14 ,    1 ,      4.4),
    (   4 ,   16 ,    1 ,      5.4),
    (  12 ,   13 ,    1 ,      5.4),
    (   3 ,    3 ,    1 ,      6.4)
) AS t (node, edge, cost, agg_cost);


SELECT set_eq('q2',
    $$SELECT node, edge, cost::TEXT, agg_cost::TEXT FROM test2$$,
    'Should be aggregating individual costs: right driving side, DIR');

-------- left driving side

PREPARE q3 AS
SELECT node, edge, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 6.8, driving_side := 'l', details := false)
ORDER BY seq;


SELECT
    node::BIGINT, edge::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test3
FROM
(VALUES
   (-1 ,   -1 ,    0 ,        0),
   ( 2 ,    1 ,  0.6 ,      0.6),
   ( 5 ,    4 ,    1 ,      1.6),
   ( 1 ,    1 ,    1 ,      1.6),
   ( 6 ,    8 ,    1 ,      2.6),
   ( 8 ,    7 ,    1 ,      2.6),
   (10 ,   10 ,    1 ,      2.6),
   ( 7 ,    6 ,    1 ,      3.6),
   ( 9 ,    9 ,    1 ,      3.6),
  ( 11 ,   11 ,    1 ,      3.6),
  ( 13 ,   14 ,    1 ,      3.6),
  (  4 ,   16 ,    1 ,      4.6),
  ( 12 ,   15 ,    1 ,      4.6),
  (  3 ,    3 ,    1 ,      5.6)
) AS t (node, edge, cost, agg_cost);


SELECT set_eq('q3',
    $$SELECT node, edge, cost::TEXT, agg_cost::TEXT FROM test3$$,
    'Should be aggregating individual costs: left driving side, DIR');

---
--- UNDIRECTED GRAPH
---

-- all results on udirected graph are "allegedly" equal
SELECT
    node::BIGINT, cost::FLOAT, agg_cost::FLOAT
INTO test4
FROM
(VALUES
    (  -1 ,    0 ,        0),
    (   1 ,  0.4 ,      0.4),
    (   2 ,  0.6 ,      0.6),
    (   5 ,    1 ,      1.6),
    (   3 ,    1 ,      1.6),
    (   6 ,    1 ,      2.6),
    (   8 ,    1 ,      2.6),
    (  10 ,    1 ,      2.6),
    (   4 ,    1 ,      2.6),
    (   7 ,    1 ,      3.6),
    (   9 ,    1 ,      3.6),
    (  11 ,    1 ,      3.6),
    (  13 ,    1 ,      3.6),
    (  12 ,    1 ,      4.6)
) AS t (node, cost, agg_cost);

-------- both driving sides

PREPARE q4 AS
SELECT node, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 6.8, driving_side := 'b', details := false, directed:=false);

SELECT set_eq('q4',
    $$SELECT node, cost::TEXT, agg_cost::TEXT FROM test4$$,
    'Should be aggregating individual costs: both driving sides, UNDI');



-------- right driving side

PREPARE q5 AS
SELECT node, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 6.8, driving_side := 'r', details := false, directed:=false);


SELECT set_eq('q5',
    $$SELECT node, cost::TEXT, agg_cost::TEXT FROM test4$$,
    'Should be aggregating individual costs: right driving side, UNDI');

-------- left driving side

PREPARE q6 AS
SELECT node, cost::TEXT, agg_cost::TEXT FROM pgr_withPointsDD(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, 6.8, driving_side := 'l', details := false, directed:=false);


SELECT set_eq('q6',
    $$SELECT node, cost::TEXT, agg_cost::TEXT FROM test4$$,
    'Should be aggregating individual costs: left driving side, UNDI');

SELECT * FROM finish();
ROLLBACK;
