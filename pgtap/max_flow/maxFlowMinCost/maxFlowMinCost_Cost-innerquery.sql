\i setup.sql

SELECT plan(584);


SELECT has_function('pgr_maxflowmincost_cost',
    ARRAY['text', 'bigint', 'bigint']);
SELECT has_function('pgr_maxflowmincost_cost',
    ARRAY['text', 'bigint', 'anyarray']);
SELECT has_function('pgr_maxflowmincost_cost',
    ARRAY['text', 'anyarray', 'bigint']);
SELECT has_function('pgr_maxflowmincost_cost',
    ARRAY['text', 'anyarray', 'anyarray']);

SELECT function_returns('pgr_maxflowmincost_cost',
    ARRAY['text', 'bigint', 'bigint'],
    'double precision');
SELECT function_returns('pgr_maxflowmincost_cost',
    ARRAY['text', 'bigint', 'anyarray'],
    'double precision');
SELECT function_returns('pgr_maxflowmincost_cost',
    ARRAY['text', 'anyarray', 'bigint'],
    'double precision');
SELECT function_returns('pgr_maxflowmincost_cost',
    ARRAY['text', 'anyarray', 'anyarray'],
    'double precision');


-- ONLY WORKS ON DIRECTED GRAPH
SELECT style_cost_flow('pgr_maxflowmincost_cost', ', 2, 3)');
SELECT style_cost_flow('pgr_maxflowmincost_cost', ', 2, ARRAY[3])');
SELECT style_cost_flow('pgr_maxflowmincost_cost', ', ARRAY[2], 3)');
SELECT style_cost_flow('pgr_maxflowmincost_cost', ', ARRAY[2], ARRAY[3])');

SELECT finish();
ROLLBACK;
