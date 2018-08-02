\i setup.sql

SELECT plan(2);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_dagshortestpath',
    ARRAY['text', 'bigint', 'bigint']);

SELECT function_returns('pgr_dagshortestpath',
    ARRAY['text', 'bigint', 'bigint'],
    'setof record');

SELECT finish();
ROLLBACK;
