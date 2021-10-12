BEGIN;

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

CREATE OR REPLACE FUNCTION repeat_lives_ok(query_to_test TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE i SMALLINT;
BEGIN
  FOR i IN 1..15 LOOP
    RETURN QUERY SELECT lives_ok(query_to_test, 'Should live: '|| i);
  END LOOP;
END;
$BODY$
LANGUAGE plpgsql;

SELECT * FROM repeat_lives_ok('No_problem_query');

SELECT finish();
ROLLBACK;
