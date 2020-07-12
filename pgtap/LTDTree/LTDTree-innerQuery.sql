\i setup.sql

SELECT plan(56);


SELECT has_function('pgr_lengauer_tarjan_dominator_tree',
    ARRAY['text', 'bigint']);

SELECT function_returns('pgr_lengauer_tarjan_dominator_tree',
    ARRAY['text', 'bigint'],
    'setof record');

-- ONE TO ONE
SELECT style_dijkstra('pgr_lengauer_tarjan_dominator_tree', ',1)');

SELECT finish();
ROLLBACK;
