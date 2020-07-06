\i setup.sql

SELECT plan(1);

SELECT todo_start('Complete the edge cases (unit tests)');

SELECT pass( 'A sample test' );

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
