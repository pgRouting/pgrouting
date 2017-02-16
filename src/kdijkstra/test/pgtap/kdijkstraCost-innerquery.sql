\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_kdijkstracost',
    ARRAY['text', 'integer', 'integer[]', 'boolean', 'boolean']);

SELECT function_returns('pgr_kdijkstracost',
    ARRAY['text', 'integer', 'integer[]', 'boolean', 'boolean'],
    'setof pgr_costresult');


-- ONE TO ONE
--with reverse cost
SELECT test_integer('pgr_kdijkstracost', ',2,ARRAY[3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_integer('pgr_kdijkstracost', ',2,ARRAY[3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_integer('pgr_kdijkstracost', ',2,ARRAY[3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_float('pgr_kdijkstracost', ',2,ARRAY[3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_float('pgr_kdijkstracost', ',2,ARRAY[3], true, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_integer('pgr_kdijkstracost', ',2,ARRAY[3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_integer('pgr_kdijkstracost', ',2,ARRAY[3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_integer('pgr_kdijkstracost', ',2,ARRAY[3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_float('pgr_kdijkstracost', ',2,ARRAY[3], true, false)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');




SELECT finish();
ROLLBACK;
