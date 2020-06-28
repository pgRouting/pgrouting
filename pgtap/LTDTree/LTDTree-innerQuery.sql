\i setup.sql

SELECT plan(56);


SELECT has_function('pgr_ltdtree',
    ARRAY['text', 'bigint']);

SELECT function_returns('pgr_ltdtree',
    ARRAY['text', 'bigint'],
    'setof record');

-- ONE TO ONE
SELECT style_dijkstra('pgr_ltdtree', ',1)');

SELECT finish();
ROLLBACK;
