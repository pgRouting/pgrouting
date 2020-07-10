\i setup.sql

SELECT plan(1);


-- Check whether the same set of rows are returned always

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY id;

PREPARE q2 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
ORDER BY id DESC;


PREPARE sequentialVertexColoring1 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q1'
);

PREPARE sequentialVertexColoring2 AS
SELECT *
FROM pgr_sequentialVertexColoring(
    'q2'
);

SELECT set_eq('sequentialVertexColoring1', 'sequentialVertexColoring2', '1: Should return same set of rows');


SELECT * FROM finish();
ROLLBACK;
