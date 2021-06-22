\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(1);

CREATE OR REPLACE FUNCTION issue()
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  IF NOT min_lib_version('3.1.2') THEN
    RETURN QUERY
    SELECT skip(1, 'Issue fixed on 3.1.2');
    RETURN;
  END IF;

PREPARE missing_id_on_matrix AS
SELECT * FROM pgr_pickDeliver(
    $$ SELECT * FROM orders ORDER BY id $$,
    $$ SELECT * FROM vehicles $$,
    $$ SELECT * from pgr_dijkstraCostMatrix(
        ' SELECT * FROM edge_table ', ARRAY[3, 4, 5, 8, 9, 11])
    $$
);

RETURN QUERY
SELECT throws_ok('missing_id_on_matrix', 'XX000', 'Unable to find node on matrix', 'Should throw: matrix is missing node 6');

END
$BODY$
LANGUAGE plpgsql VOLATILE;

SELECT issue();
SELECT finish();
ROLLBACK;
