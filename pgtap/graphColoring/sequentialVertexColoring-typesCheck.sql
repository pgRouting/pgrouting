\i setup.sql

SELECT plan(1);

SELECT todo_start('Complete the types check');

SELECT pass( 'A sample test' );

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
