
BEGIN;
\i tsp_pgtap_tests.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('4.0.0') THEN plan(1) ELSE plan(9) END;

CREATE TEMP TABLE data AS
SELECT * FROM pgr_dijkstraCostMatrix(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE ID < 14),
    directed:= false
);

SELECT annaeling_parameters('pgr_tsp');

SELECT finish();
ROLLBACK;
