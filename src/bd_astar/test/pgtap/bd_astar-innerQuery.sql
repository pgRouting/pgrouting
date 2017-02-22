\i setup.sql

SELECT plan(87);

SELECT has_function('pgr_bdastar',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_bdastar',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'],
    'setof pgr_costresult');

SELECT style_old_astar_with('pgr_bdastar', ', 2, 3, true, true)');
SELECT style_old_astar_no_rev('pgr_bdastar', ', 2, 3, true, false)');


SELECT finish();
ROLLBACK;
