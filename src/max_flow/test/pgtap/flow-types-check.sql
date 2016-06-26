\i setup.sql
SELECT plan(128);
SET client_min_messages TO WARNING;
SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'integer', 'bigint', 'bigint'
    ]);

SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'bigint', 'bigint', 'bigint',
    'bigint', 'bigint'
    ], 'setof record');



PREPARE parameters AS
SELECT array[
'edges_sql',
'source_vertex',
'sink_vertex',
'id',
'source',
'target',
'capacity',
'reverse_capacity'];

SELECT * FROM finish();
ROLLBACK;