\i setup.sql
SET client_min_messages TO WARNING;

BEGIN;
SELECT plan(17);

SELECT has_function('pgr_edgedisjointpaths');

SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'bigint', 'boolean'
    ]);
SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'anyarray', 'boolean'
    ]);
SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'bigint', 'boolean'
    ]);
SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'anyarray', 'boolean'
    ]);

SELECT has_function('pgr_maximumcardinalitymatching');

SELECT has_function('pgr_maximumcardinalitymatching', ARRAY[
    'text', 'boolean'
    ]);

SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'bigint', 'boolean'
    ], 'setof record');
SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'anyarray', 'boolean'
    ], 'setof record');
SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'bigint', 'boolean'
    ], 'setof record');
SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'anyarray', 'boolean'
    ], 'setof record');

SELECT function_returns('pgr_maximumcardinalitymatching', ARRAY[
    'text', 'boolean'
    ], 'setof record');

PREPARE parameters_one2one AS
SELECT array[
'edges_sql',
'source_vertex',
'destination_vertex',
'directed',
'seq',
'path_seq',
'node',
'edge'];

PREPARE parameters_one2many AS
SELECT array[
'edges_sql',
'source_vertex',
'destination_vertices',
'directed',
'seq',
'path_seq',
'end_vid',
'node',
'edge'];

PREPARE parameters_many2one AS
SELECT array[
'edges_sql',
'source_vertices',
'destination_vertex',
'directed',
'seq',
'path_seq',
'start_vid',
'node',
'edge'];

PREPARE parameters_many2many AS
SELECT array[
'edges_sql',
'source_vertices',
'destination_vertices',
'directed',
'seq',
'path_seq',
'start_vid',
'end_vid',
'node',
'edge'];

PREPARE parameters_maxcardinalitymatching AS
SELECT array[
'edges_sql',
'directed',
'seq',
'edge_id',
'source',
'target'];


SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_edgedisjointpaths'$$,
    'parameters_one2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_edgedisjointpaths'$$,
    'parameters_one2many');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_edgedisjointpaths'$$,
    'parameters_many2one');
SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_edgedisjointpaths'$$,
    'parameters_many2many');

SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_maximumcardinalitymatching'$$,
    'parameters_maxcardinalitymatching');




SELECT * FROM finish();
ROLLBACK;