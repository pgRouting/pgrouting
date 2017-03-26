\i setup.sql

SELECT plan(82);


SELECT has_function('pgr_floydwarshall',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_floydwarshall',
    ARRAY['text', 'boolean'],
    'setof record');

SELECT style_dijkstra_no_id('pgr_floydwarshall', ', true)');
SELECT style_dijkstra('pgr_floydwarshall', ', true)');


SELECT finish();
ROLLBACK;
