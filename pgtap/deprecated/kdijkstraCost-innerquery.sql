\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_kdijkstracost',
    ARRAY['text', 'integer', 'integer[]', 'boolean', 'boolean']);

SELECT function_returns('pgr_kdijkstracost',
    ARRAY['text', 'integer', 'integer[]', 'boolean', 'boolean'],
    'setof pgr_costresult');



-- ONE TO MANY
SELECT style_old_dijkstra_with('pgr_kdijkstracost', ',2,ARRAY[3], true, true)');
SELECT style_old_dijkstra_no_rev('pgr_kdijkstracost', ',2,ARRAY[3], true, false)');


SELECT finish();
ROLLBACK;
