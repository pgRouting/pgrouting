\i setup.sql

SELECT plan(5);

PREPARE q1 AS
SELECT * FROM pgr_prim(
  'SELECT id, source, target, cost, reverse_cost
    FROM edge_table ', 18
);

PREPARE q2 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost 
       FROM edge_table', 20
);

PREPARE q3 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table ORDER BY id', 4
) WHERE cost < 0;


SELECT is_empty('q1', 'Node 18: This node does not exists');
SELECT is_empty('q2', 'Node 18: This node does not exists');
SELECT is_empty('q3', 'No cost can be negative');


PREPARE q4 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table where id = 17'
);

PREPARE q5 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table ', 14 
);

PREPARE q6 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table ', 16 
);


SELECT set_eq( 'q4', 'q5', '4: Spanning of edge 17');
SELECT set_eq('q6', 'VALUES (1, 1, 16, -1, -1, 0, 0), (2, 1, 16, 17, 18, 1, 1)' , '5: Compare when node is 16 with actual result');


SELECT * FROM finish();
ROLLBACK;
