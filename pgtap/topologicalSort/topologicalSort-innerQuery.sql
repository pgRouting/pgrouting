\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(2);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_topologicalsort');

SELECT function_returns('pgr_topologicalsort',ARRAY['text'],'setof record');

SELECT finish();
ROLLBACK;
