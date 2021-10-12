BEGIN;

\i trsp_tests.sql

SELECT plan(612);

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

SELECT compare_trsp_dijkstra(18, false);

SELECT finish();
ROLLBACK;

