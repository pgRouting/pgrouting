\i setup.sql

SELECT plan(87);


SELECT has_function('pgr_astar',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_astar',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'],
    'setof pgr_costresult');



-- ONE TO ONE
--with reverse cost
SELECT style_old_astar_with('pgr_astar', ',2,3, true, true)');
SELECT style_old_astar_no_rev('pgr_astar', ',2,3, true, false)');


SELECT finish();
ROLLBACK;
