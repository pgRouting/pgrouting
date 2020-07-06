\i setup.sql

SELECT plan(1);

SELECT todo_start('Complete the no crash tests');

SELECT pass( 'A sample test' );

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
