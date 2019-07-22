\i setup.sql

SELECT plan(224);


SELECT has_function('pgr_maxflow',
    ARRAY['text', 'bigint', 'bigint']);
SELECT has_function('pgr_maxflow',
    ARRAY['text', 'bigint', 'anyarray']);
SELECT has_function('pgr_maxflow',
    ARRAY['text', 'anyarray', 'bigint']);
SELECT has_function('pgr_maxflow',
    ARRAY['text', 'anyarray', 'anyarray']);

SELECT function_returns('pgr_maxflow',
    ARRAY['text', 'bigint', 'bigint'],
    'bigint');
SELECT function_returns('pgr_maxflow',
    ARRAY['text', 'bigint', 'anyarray'],
    'bigint');
SELECT function_returns('pgr_maxflow',
    ARRAY['text', 'anyarray', 'bigint'],
    'bigint');
SELECT function_returns('pgr_maxflow',
    ARRAY['text', 'anyarray', 'anyarray'],
    'bigint');


-- ONLY WORKS ON DIRECTED GRAPH
SELECT style_max_flow('pgr_maxflow', ', 2, 3)');
SELECT style_max_flow('pgr_maxflow', ', 2, ARRAY[3])');
SELECT style_max_flow('pgr_maxflow', ', ARRAY[2], 3)');
SELECT style_max_flow('pgr_maxflow', ', ARRAY[2], ARRAY[3])');

SELECT finish();
ROLLBACK;
