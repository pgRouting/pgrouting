\i setup.sql

SELECT plan(137);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_turnpenaltygraph',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_turnpenaltygraph',
    ARRAY['text', 'boolean'],
    'setof record');

SELECT style_lineGraph('pgr_turnpenaltygraph', ')');
SELECT style_lineGraph('pgr_turnpenaltygraph', ', true)');
SELECT style_lineGraph('pgr_turnpenaltygraph', ', false)');


SELECT finish();
ROLLBACK;
