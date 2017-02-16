\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_dijkstravia',
    ARRAY['text', 'anyarray', 'boolean', 'boolean', 'boolean']);

SELECT function_returns('pgr_dijkstravia',
    ARRAY['text', 'anyarray', 'boolean', 'boolean', 'boolean'],
    'setof record');


-- ONE TO ONE
--with reverse cost
SELECT test_anyinteger('pgr_dijkstravia', ',ARRAY[2,3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyinteger('pgr_dijkstravia', ',ARRAY[2,3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyinteger('pgr_dijkstravia', ',ARRAY[2,3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstravia', ',ARRAY[2,3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_dijkstravia', ',ARRAY[2,3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyinteger('pgr_dijkstravia', ',ARRAY[2,3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyinteger('pgr_dijkstravia', ',ARRAY[2,3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyinteger('pgr_dijkstravia', ',ARRAY[2,3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_dijkstravia', ',ARRAY[2,3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');


SELECT finish();
ROLLBACK;
