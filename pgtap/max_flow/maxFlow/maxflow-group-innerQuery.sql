\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(226);


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

-- new signature on 3.2
SELECT CASE
WHEN NOT min_version('3.2.0') THEN
  skip(2, 'Combinations functiontionality new on 3.2.0')
ELSE
  collect_tap(
    has_function('pgr_maxflow', ARRAY['text', 'text']),
    function_returns('pgr_maxflow', ARRAY['text', 'text'], 'bigint')
  )
END;


-- ONLY WORKS ON DIRECTED GRAPH
SELECT style_max_flow('pgr_maxflow', ', 2, 3)');
SELECT style_max_flow('pgr_maxflow', ', 2, ARRAY[3])');
SELECT style_max_flow('pgr_maxflow', ', ARRAY[2], 3)');
SELECT style_max_flow('pgr_maxflow', ', ARRAY[2], ARRAY[3])');

SELECT finish();
ROLLBACK;
