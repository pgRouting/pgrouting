\i setup.sql

SELECT plan(5);

PREPARE prim1 AS
SELECT * FROM pgr_prim(
  'SELECT id, source, target, cost, reverse_cost
    FROM edge_table', 18
);

PREPARE prim2 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost
       FROM edge_table', 20
);

PREPARE prim3 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost
       FROM edge_table ORDER BY id', 4
) WHERE cost < 0;

SELECT is_empty('prim1', '1: No_edge');
SELECT is_empty('prim2', 'Node 18: This node does not exists');
SELECT is_empty('prim3', 'No cost can be negative');

PREPARE prim4 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost
       FROM edge_table where id = 17'
);

PREPARE prim5 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost
       FROM edge_table ', 14
);

PREPARE prim6 AS
SELECT *
FROM pgr_prim(
    'SELECT id, source, target, cost, reverse_cost
       FROM edge_table ', 16
);

SELECT set_eq( 'prim4', 'prim5', '4: Spanning of edge 17');
SELECT set_eq('prim6', 'VALUES (1, 16, 16, -1, 0, 0, 0), (2, 16, 17, 18, 1, 1, 1)' , '5: Compare when node is 16 with actual result');


