\i setup.sql
SELECT plan(8);

-- 0 edge 0 vertex tests
PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');


PREPARE ltdtree_test1 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id < 0',1
);

PREPARE ltdtree_test2 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 0',1
);

PREPARE ltdtree_test3 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18',1
);

SELECT is_empty('ltdtree_test1', 'ltdtree_test1: Graph with 0 edge and 0 vertex');
SELECT is_empty('ltdtree_test2', 'ltdtree_test2: Graph with 0 edge and 0 vertex');
SELECT is_empty('ltdtree_test3', 'ltdtree_test3: Graph with 0 edge and 0 vertex');



--root not present tests
PREPARE ltdtree_test4 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 2',1
);


PREPARE ltdtree_test5 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 2',1
);

SELECT is_empty('ltdtree_test4', 'ltdtree_test4: Root not present in the Graph');
SELECT is_empty('ltdtree_test5', 'ltdtree_test5: Root not present in the Graph');

--vertex not present in the graph tests


--verify results test


--id constrained tests
PREPARE q2 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 0',1
);
SELECT isnt_empty('q2', 'q1: Graph with 0 edge and 0 vertex');

-- Negative root tests
PREPARE q3 AS
SELECT *
FROM pgr_ltdtree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table',-1
);
SELECT throws_ok('q3', 'P0001', 'Negative value found on ''root_vertex''', '3: Negative root throws');

ROLLBACK;
