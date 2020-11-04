\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);

SELECT plan(4);


PREPARE allgraph AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    $$SELECT * FROM edge_table$$,
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

PREPARE minigraph AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    $$SELECT * FROM edge_table WHERE source IN(1,2) OR target IN(1,2)$$,
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

SELECT lives_ok('allgraph', 'allgraph QUERY 1: Graph with no loop cycle');
SELECT lives_ok('minigraph', 'minigraph QUERY 1: Graph with no loop cycle');

INSERT INTO edge_table (source, target, cost, reverse_cost) VALUES
(1, 1, 1, 1);

SELECT lives_ok('allgraph', 'allgraph QUERY 1: Graph with no loop cycle');
SELECT lives_ok('minigraph', 'minigraph QUERY 1: Graph with no loop cycle');

SELECT finish();
DELETE FROM edge_table WHERE id > 18;
ROLLBACK;
