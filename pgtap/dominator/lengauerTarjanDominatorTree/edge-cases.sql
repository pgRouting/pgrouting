\i setup.sql
SELECT plan(6);

-- 0 edge 0 vertex tests
PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');


PREPARE LengauerTarjanDominatorTree_test1 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id < 0',1
);

PREPARE LengauerTarjanDominatorTree_test2 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 0',1
);

PREPARE LengauerTarjanDominatorTree_test3 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18',1
);

SELECT is_empty('LengauerTarjanDominatorTree_test1', 'LengauerTarjanDominatorTree_test1: Graph with 0 edge and 0 vertex');
SELECT is_empty('LengauerTarjanDominatorTree_test2', 'LengauerTarjanDominatorTree_test2: Graph with 0 edge and 0 vertex');
SELECT is_empty('LengauerTarjanDominatorTree_test3', 'LengauerTarjanDominatorTree_test3: Graph with 0 edge and 0 vertex');



--root not present tests
/*
PREPARE LengauerTarjanDominatorTree_test4 AS
SELECT *
FROM pgr_LengauerTarjanDominatorTree_tree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 2',1
);


PREPARE LengauerTarjanDominatorTree_test5 AS
SELECT *
FROM pgr_LengauerTarjanDominatorTree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 2',1
);

SELECT is_empty('LengauerTarjanDominatorTree_test4', 'LengauerTarjanDominatorTree_test4: Root not present in the Graph');
SELECT is_empty('LengauerTarjanDominatorTree_test5', 'LengauerTarjanDominatorTree_test5: Root not present in the Graph');
*/
--vertex not present in the graph tests


--verify results test


--id constrained tests
PREPARE q2 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 0',1
);
SELECT isnt_empty('q2', 'q2: Graph with 0 edge and 0 vertex');

-- Negative root tests
PREPARE q3 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'SELECT id, source, target, cost, reverse_cost
FROM edge_table',-1
);
SELECT throws_ok('q3', 'P0001', 'Negative value found on ''root_vertex''', '3: Negative root throws');

ROLLBACK;
