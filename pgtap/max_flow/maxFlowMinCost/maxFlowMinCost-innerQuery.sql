\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(586);


SELECT has_function('pgr_maxflowmincost',
    ARRAY['text', 'bigint', 'bigint']);
SELECT has_function('pgr_maxflowmincost',
    ARRAY['text', 'bigint', 'anyarray']);
SELECT has_function('pgr_maxflowmincost',
    ARRAY['text', 'anyarray', 'bigint']);
SELECT has_function('pgr_maxflowmincost',
    ARRAY['text', 'anyarray', 'anyarray']);
SELECT has_function('pgr_maxflowmincost',
    ARRAY['text', 'text']);

SELECT function_returns('pgr_maxflowmincost',
    ARRAY['text', 'bigint', 'bigint'],
    'setof record');
SELECT function_returns('pgr_maxflowmincost',
    ARRAY['text', 'bigint', 'anyarray'],
    'setof record');
SELECT function_returns('pgr_maxflowmincost',
    ARRAY['text', 'anyarray', 'bigint'],
    'setof record');
SELECT function_returns('pgr_maxflowmincost',
    ARRAY['text', 'anyarray', 'anyarray'],
    'setof record');
SELECT function_returns('pgr_maxflowmincost',
    ARRAY['text', 'text'],
    'setof record');


-- ONLY WORKS ON DIRECTED GRAPH
SELECT style_cost_flow('pgr_maxflowmincost', ', 2, 3)');
SELECT style_cost_flow('pgr_maxflowmincost', ', 2, ARRAY[3])');
SELECT style_cost_flow('pgr_maxflowmincost', ', ARRAY[2], 3)');
SELECT style_cost_flow('pgr_maxflowmincost', ', ARRAY[2], ARRAY[3])');

SELECT finish();
ROLLBACK;
