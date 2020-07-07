\i setup.sql

SELECT plan(3);

SELECT todo_start('Complete the types check');

SELECT has_function('pgr_sequentialvertexcoloring');

SELECT has_function('pgr_sequentialvertexcoloring', ARRAY['text']);
SELECT function_returns('pgr_sequentialvertexcoloring', ARRAY['text'],  'setof record');

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
