\i setup.sql

SELECT plan(52);


SELECT has_function('pgr_drivingdistance',
    ARRAY['text', 'bigint', 'double precision', 'boolean', 'boolean']);

SELECT function_returns('pgr_drivingdistance',
    ARRAY['text', 'bigint', 'double precision', 'boolean', 'boolean'],
    'setof pgr_costresult');


-- ONE SOURCE
SELECT style_old_dijkstra_with('pgr_drivingdistance', ',2,3, true, true)');
SELECT style_old_dijkstra_with('pgr_drivingdistance', ',2,3, true, false)');



SELECT finish();
ROLLBACK;
