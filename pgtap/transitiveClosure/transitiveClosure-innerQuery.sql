\i setup.sql

SELECT plan(2);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_transitiveclosure');

SELECT function_returns('pgr_transitiveclosure',ARRAY['text'],'setof record');

SELECT finish();
ROLLBACK;
