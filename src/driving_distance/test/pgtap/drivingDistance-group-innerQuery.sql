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
SELECT style_dijkstra('pgr_drivingdistance', ',2, 1, true)');

-- MANY SOURCES
SELECT style_dijkstra('pgr_drivingdistance', ',ARRAY[2,3], 1, true)');



SELECT finish();
ROLLBACK;
