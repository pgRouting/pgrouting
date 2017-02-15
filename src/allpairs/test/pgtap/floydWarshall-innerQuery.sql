\i setup.sql

SELECT plan(87);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_floydwarshall',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_floydwarshall',
    ARRAY['text', 'boolean'],
    'setof record');


--with reverse cost
SELECT test_anyInteger('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'x1');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'y1');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'x2');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'y2');


--without reverse cost
SELECT test_anyInteger('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'x1');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'y1');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'x2');
SELECT test_anyNumerical('pgr_floydwarshall', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'y2');



SELECT finish();
ROLLBACK;
