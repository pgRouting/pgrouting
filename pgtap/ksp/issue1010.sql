\i setup.sql


SELECT plan(6);


PREPARE r1 AS
SELECT seq, node::BIGINT, edge::BIGINT, agg_cost FROM ( VALUES
       (1,      1,   4,   0),
       (2,      12,   6,   1),
       (3,      14,   8,   2),
       (4,      0,  -1,   3))
AS t(seq, node, edge, agg_cost);

PREPARE r2 AS
SELECT seq, node::BIGINT, edge::BIGINT, agg_cost FROM ( VALUES
   (5,1,4,0),
   (6,12,5,1),
   (7,13,7,2),
   (8,14,8,3),
   (9,0,-1,4))
AS t(seq, node, edge, agg_cost);

PREPARE r3 AS
SELECT seq, node::BIGINT, edge::BIGINT, agg_cost FROM ( VALUES
  (10,1,1,0),
  (11,2,2,1),
  (12,3,3,2),
  (13,0,-1,101))
AS t(seq, node, edge, agg_cost);

PREPARE q1 AS
SELECT seq, node, edge, agg_cost
FROM pgr_ksp(
     'select 1 as id, 1 as source, 2 as target, 1 as cost union
     select 2 , 2 , 3 , 1  union
     select 3 , 3 , 0 , 99 union
     select 4 , 1 , 12, 1  union
     select 5 , 12, 13, 1  union
     select 6 , 12, 14, 1  union
     select 7 , 13, 14, 1  union
     select 8 , 14, 0 , 1',
     1, 0, 1, directed := TRUE);

SELECT set_eq('q1','r1','q1 should have r1 result');

PREPARE q2 AS
SELECT seq, node, edge, agg_cost
FROM pgr_ksp(
     'select 1 as id, 1 as source, 2 as target, 1 as cost union
     select 2 , 2 , 3 , 1  union
     select 3 , 3 , 0 , 99 union
     select 4 , 1 , 12, 1  union
     select 5 , 12, 13, 1  union
     select 6 , 12, 14, 1  union
     select 7 , 13, 14, 1  union
     select 8 , 14, 0 , 1',
     1, 0, 2, directed := TRUE);

SELECT bag_has('q2','r1','q2 should have r1 result');
SELECT bag_has('q2','r2','q2 should have r2 result');

PREPARE q3 AS
SELECT seq, node, edge, agg_cost
FROM pgr_ksp(
     'select 1 as id, 1 as source, 2 as target, 1 as cost union
     select 2 , 2 , 3 , 1  union
     select 3 , 3 , 0 , 99 union
     select 4 , 1 , 12, 1  union
     select 5 , 12, 13, 1  union
     select 6 , 12, 14, 1  union
     select 7 , 13, 14, 1  union
     select 8 , 14, 0 , 1',
     1, 0, 3, directed := TRUE);

SELECT bag_has('q3','r1','q3 should have r1 result');
SELECT bag_has('q3','r2','q3 should have r2 result');
SELECT bag_has('q3','r3','q3 should have r3 result');


SELECT finish();
ROLLBACK;
