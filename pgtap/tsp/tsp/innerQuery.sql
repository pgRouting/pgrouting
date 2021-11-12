BEGIN;
SET client_min_messages TO WARNING;

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(18);

CREATE TEMP TABLE data AS
SELECT * FROM pgr_dijkstraCostMatrix(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE ID < 14),
    directed:= false
);

SELECT tsp_anyInteger('pgr_tsp',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'start_vid');

SELECT tsp_anyInteger('pgr_tsp',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'end_vid');
SELECT tsp_anyNumerical('pgr_tsp',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'agg_cost');

SELECT finish();
ROLLBACK;
