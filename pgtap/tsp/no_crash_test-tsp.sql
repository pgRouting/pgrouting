\i setup.sql

SELECT plan(5);

PREPARE data AS
SELECT * FROM pgr_dijkstraCostMatrix(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE id < 14),
    directed := false
);

SELECT isnt_empty('data', 'Should not be empty true to tests be meaningful');

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY[
    '$fn$SELECT * FROM pgr_dijkstraCostMatrix(
        $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
        (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE id < 14),
        directed := false
    )$fn$'
    ]::TEXT[];
    subs = ARRAY[
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_TSP', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
