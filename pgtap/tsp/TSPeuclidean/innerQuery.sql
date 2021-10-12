
BEGIN;
\i tsp_pgtap_tests.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(18);

CREATE TEMP TABLE data AS
SELECT id, ST_X(the_geom) AS x, ST_Y(the_geom) AS y
FROM edge_table_vertices_pgr;

SELECT tsp_anyInteger('pgr_tspeuclidean',
    ARRAY['id', 'x', 'y'],
    'id');
SELECT tsp_anyNumerical('pgr_tspeuclidean',
    ARRAY['id', 'x', 'y'],
    'x');
SELECT tsp_anyNumerical('pgr_tspeuclidean',
    ARRAY['id', 'x', 'y'],
    'y');

SELECT finish();
ROLLBACK;
