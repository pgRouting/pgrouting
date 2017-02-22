\i setup.sql

SELECT plan(82);


SELECT has_function('pgr_johnson',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_johnson',
    ARRAY['text', 'boolean'],
    'setof record');


SELECT style_dijkstra_no_id('pgr_johnson', ', true)');
SELECT style_dijkstra('pgr_johnson', ', true)');



SELECT finish();
ROLLBACK;
