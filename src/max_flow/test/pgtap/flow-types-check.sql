--\i setup.sql
BEGIN;
SELECT plan(39);
SET client_min_messages TO WARNING;
--Boykov-Kolmogorov
SELECT has_function('pgr_maxflowboykovkolmogorov');

SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'bigint'
    ]);
SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'bigint'
    ]);
SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'anyarray'
    ]);
SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'anyarray'
    ]);

SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'anyarray'
    ], 'setof record');
SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'anyarray'
    ], 'setof record');

--Edmonds-Karp
SELECT has_function('pgr_maxflowedmondskarp');

SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'bigint'
    ]);
SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'bigint'
    ]);
SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'anyarray'
    ]);
SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'anyarray'
    ]);

SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'anyarray'
    ], 'setof record');
SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'anyarray'
    ], 'setof record');

--Push-Relabel
SELECT has_function('pgr_maxflowpushrelabel');

SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'bigint'
    ]);
SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'bigint'
    ]);
SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'anyarray'
    ]);
SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'anyarray'
    ]);


SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'anyarray'
    ], 'setof record');
SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'anyarray'
    ], 'setof record');

PREPARE parameters_one2one AS
SELECT array[
'edges_sql',
'source_vertex',
'sink_vertex',
'id',
'edge_id',
'source',
'target',
'flow',
'residual_capacity'];

PREPARE parameters_one2many AS
SELECT array[
'edges_sql',
'source_vertex',
'sink_vertices',
'id',
'edge_id',
'source',
'target',
'flow',
'residual_capacity'];

PREPARE parameters_many2one AS
SELECT array[
'edges_sql',
'source_vertices',
'sink_vertex',
'id',
'edge_id',
'source',
'target',
'flow',
'residual_capacity'];

PREPARE parameters_many2many AS
SELECT array[
'edges_sql',
'source_vertices',
'sink_vertices',
'id',
'edge_id',
'source',
'target',
'flow',
'residual_capacity'];

SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowpushrelabel'$$,
    'parameters_one2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowpushrelabel'$$,
    'parameters_one2many');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowpushrelabel'$$,
    'parameters_many2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowpushrelabel'$$,
    'parameters_many2many');

SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowedmondskarp'$$,
    'parameters_one2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowedmondskarp'$$,
    'parameters_one2many');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowedmondskarp'$$,
    'parameters_many2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowedmondskarp'$$,
    'parameters_many2many');

SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowboykovkolmogorov'$$,
    'parameters_one2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowboykovkolmogorov'$$,
    'parameters_one2many');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowboykovkolmogorov'$$,
    'parameters_many2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maxflowboykovkolmogorov'$$,
    'parameters_many2many');

SELECT * FROM finish();
ROLLBACK;