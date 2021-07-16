
\i setup.sql
\i tsp_pgtap_tests.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('4.0.0') THEN plan(1) ELSE plan(9) END;

CREATE TEMP TABLE data AS
SELECT id, ST_X(the_geom) AS x, ST_Y(the_geom) AS y
FROM edge_table_vertices_pgr;

SELECT annaeling_parameters('pgr_tspeuclidean');

SELECT finish();
ROLLBACK;
