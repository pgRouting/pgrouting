\i setup.sql

SELECT plan(17);


SELECT has_function('pgr_apspjohnson',
    ARRAY['text']);

SELECT function_returns('pgr_apspjohnson',
    ARRAY['text'],
    'setof pgr_costresult');


-- only works with source, target, cost
-- no reverse_cost and no id
-- when reverse_cost or id are added they are ignored???

SELECT style_old_dijkstra_no_id_no_reverse('pgr_apspjohnson', ')');

SELECT finish();
ROLLBACK;
