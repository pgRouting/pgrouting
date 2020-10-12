\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(164);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_linegraph',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_linegraph',
    ARRAY['text', 'boolean'],
    'setof record');

SELECT style_lineGraph('pgr_linegraph', ')');
SELECT style_lineGraph('pgr_linegraph', ', true)');
SELECT style_lineGraph('pgr_linegraph', ', false)');


SELECT finish();
ROLLBACK;
