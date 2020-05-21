\i setup.sql

SELECT plan(1);

SELECT todo_start('Must add all edge cases');

SELECT is_empty(' SELECT id, source, target, cost > 0, reverse_cost > 0  from edge_table where id>18 ','1');

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
