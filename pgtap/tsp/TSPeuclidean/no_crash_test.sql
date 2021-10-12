BEGIN;
\i tsp_pgtap_tests.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(9);

CREATE TEMP TABLE  data AS
SELECT id, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr;

SELECT isnt_empty('SELECT * FROM data', 'Should not be empty true to tests be meaningful');
SELECT tspeuclidean_no_crash();

SELECT finish();
ROLLBACK;

