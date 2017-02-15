\i setup.sql

SELECT plan(87);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_bdastar',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_bdastar',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'],
    'setof pgr_costresult');


--with reverse cost
SELECT test_integer('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
SELECT test_integer('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
SELECT test_integer('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
SELECT test_float('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
SELECT test_float('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'reverse_cost');
SELECT test_float('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
SELECT test_float('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
SELECT test_float('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
SELECT test_float('pgr_bdastar', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');


--without reverse cost
SELECT test_integer('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
SELECT test_integer('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
SELECT test_integer('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
SELECT test_float('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
SELECT test_float('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
SELECT test_float('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
SELECT test_float('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
SELECT test_float('pgr_bdastar', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');



SELECT finish();
ROLLBACK;
