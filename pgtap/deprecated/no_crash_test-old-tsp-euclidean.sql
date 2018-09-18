\i setup.sql

SELECT plan(7);

PREPARE data AS
SELECT id::INTEGER, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr;

SELECT isnt_empty('data', 'Should not be empty true to tests be meaningful');

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY[
    '$$SELECT id::INTEGER, st_X(the_geom) AS x, st_Y(the_geom)AS y  FROM edge_table_vertices_pgr$$',
    '1'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    'NULL::INTEGER'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_tsp', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
