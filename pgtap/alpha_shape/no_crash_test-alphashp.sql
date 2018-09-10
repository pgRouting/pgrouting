\i setup.sql

SELECT plan(5);

PREPARE alpha_sql AS
SELECT id::INTEGER, ST_X(the_geom) AS x, ST_Y(the_geom) AS y FROM edge_table_vertices_pgr;

SELECT isnt_empty('alpha_sql', 'Should not be empty true to tests be meaningful');





CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY['$$SELECT id::INTEGER, ST_X(the_geom) AS x, ST_Y(the_geom) AS y FROM edge_table_vertices_pgr$$']::TEXT[];
    subs = ARRAY[
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_alphashape', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
