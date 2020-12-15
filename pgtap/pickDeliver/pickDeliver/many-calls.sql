\i setup.sql

SELECT plan(15);
SET client_min_messages TO NOTICE;


PREPARE No_problem_query AS
SELECT * FROM pgr_pickDeliver(
    $$ SELECT * FROM orders ORDER BY id $$,
    $$ SELECT * FROM vehicles ORDER BY id$$,
    $$ SELECT * from pgr_dijkstraCostMatrix(
        'SELECT * FROM edge_table ',
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a))
    $$
);

SELECT lives_ok('No_problem_query', 'Should live: '|| 1);
SELECT lives_ok('No_problem_query', 'Should live: '|| 2);
SELECT lives_ok('No_problem_query', 'Should live: '|| 3);
SELECT lives_ok('No_problem_query', 'Should live: '|| 4);
SELECT lives_ok('No_problem_query', 'Should live: '|| 5);
SELECT lives_ok('No_problem_query', 'Should live: '|| 6);
SELECT lives_ok('No_problem_query', 'Should live: '|| 7);
SELECT lives_ok('No_problem_query', 'Should live: '|| 8);
SELECT lives_ok('No_problem_query', 'Should live: '|| 9);
SELECT lives_ok('No_problem_query', 'Should live: '|| 10);
SELECT lives_ok('No_problem_query', 'Should live: '|| 11);
SELECT lives_ok('No_problem_query', 'Should live: '|| 12);
SELECT lives_ok('No_problem_query', 'Should live: '|| 13);
SELECT lives_ok('No_problem_query', 'Should live: '|| 14);
SELECT lives_ok('No_problem_query', 'Should live: '|| 15);


SELECT finish();
