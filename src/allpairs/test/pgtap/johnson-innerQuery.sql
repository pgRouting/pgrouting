\i setup.sql

SELECT plan(82);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_johnson',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_johnson',
    ARRAY['text', 'boolean'],
    'setof record');


--with reverse cost
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_johnson', ', true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



--without ID
--with reverse cost
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_johnson', ', true)',
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_johnson', ', true)',
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_johnson', ', true)',
    ARRAY['source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_johnson', ', true)',
    ARRAY['source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
