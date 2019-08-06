\i setup.sql

SELECT plan(2);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_transitiveClosure');

SELECT function_returns('pgr_transitiveClosure',ARRAY['text'],'setof record');

SELECT finish();
ROLLBACK;
