\i setup.sql

SELECT plan(94);


SELECT has_function('pgr_drivingdistance',
    ARRAY['text', 'bigint', 'double precision', 'boolean']);
SELECT has_function('pgr_drivingdistance',
    ARRAY['text', 'anyarray', 'double precision', 'boolean', 'boolean']);

SELECT function_returns('pgr_drivingdistance',
    ARRAY['text', 'bigint', 'double precision', 'boolean'],
    'setof record');
SELECT function_returns('pgr_drivingdistance',
    ARRAY['text', 'anyarray', 'double precision', 'boolean', 'boolean'],
    'setof record');


-- ONE SOURCE
--with reverse cost
SELECT test_anyInteger('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_drivingdistance', ',2,3, true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



-- MANY SOURCES
--with reverse cost
SELECT test_anyInteger('pgr_drivingdistance', ',ARRAY[3], 1, true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_drivingdistance', ',ARRAY[3],1,  true)',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');



SELECT finish();
ROLLBACK;
