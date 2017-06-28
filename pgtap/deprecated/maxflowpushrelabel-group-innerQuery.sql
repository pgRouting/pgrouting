\i setup.sql

SELECT plan(188);


SELECT has_function('pgr_maxflowpushrelabel',
    ARRAY['text', 'bigint', 'bigint']);
SELECT has_function('pgr_maxflowpushrelabel',
    ARRAY['text', 'bigint', 'anyarray']);
SELECT has_function('pgr_maxflowpushrelabel',
    ARRAY['text', 'anyarray', 'bigint']);
SELECT has_function('pgr_maxflowpushrelabel',
    ARRAY['text', 'anyarray', 'anyarray']);

SELECT function_returns('pgr_maxflowpushrelabel',
    ARRAY['text', 'bigint', 'bigint'],
    'setof record');
SELECT function_returns('pgr_maxflowpushrelabel',
    ARRAY['text', 'bigint', 'anyarray'],
    'setof record');
SELECT function_returns('pgr_maxflowpushrelabel',
    ARRAY['text', 'anyarray', 'bigint'],
    'setof record');
SELECT function_returns('pgr_maxflowpushrelabel',
    ARRAY['text', 'anyarray', 'anyarray'],
    'setof record');


-- ONLY WORKS ON DIRECTED GRAPH
SELECT style_max_flow('pgr_maxflowpushrelabel', ', 2, 3)');
SELECT style_max_flow('pgr_maxflowpushrelabel', ', 2, ARRAY[3])');
SELECT style_max_flow('pgr_maxflowpushrelabel', ', ARRAY[2], 3)');
SELECT style_max_flow('pgr_maxflowpushrelabel', ', ARRAY[2], ARRAY[3])');

SELECT finish();
ROLLBACK;
