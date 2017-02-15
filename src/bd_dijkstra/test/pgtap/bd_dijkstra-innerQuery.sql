\i setup.sql

SELECT plan(47);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_bddijkstra',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_bddijkstra',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'],
    'setof pgr_costresult');


--with reverse cost
SELECT test_integer('pgr_bddijkstra', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
SELECT test_integer('pgr_bddijkstra', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
SELECT test_integer('pgr_bddijkstra', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
SELECT test_float('pgr_bddijkstra', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
SELECT test_float('pgr_bddijkstra', ',2,3, true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'reverse_cost');


--without reverse cost
SELECT test_integer('pgr_bddijkstra', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
SELECT test_integer('pgr_bddijkstra', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
SELECT test_integer('pgr_bddijkstra', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
SELECT test_float('pgr_bddijkstra', ',2,3, true, false)',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');



SELECT finish();
ROLLBACK;
