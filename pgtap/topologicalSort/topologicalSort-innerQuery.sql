\i setup.sql

SELECT plan(2);
SET client_min_messages TO ERROR;

SELECT has_function('pgr_topologicalsort',
    ARRAY['text'] );

SELECT function_returns('pgr_topologicalsort',
    ARRAY['text'],
    'setof record');

SELECT finish();
ROLLBACK;
