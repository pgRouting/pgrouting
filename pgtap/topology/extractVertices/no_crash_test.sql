\i setup.sql

SELECT plan(17);

PREPARE edges AS
SELECT source, target, the_geom  FROM edge_table;

PREPARE edges1 AS
SELECT source, target  FROM edge_table;

SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- with geometry
    params = ARRAY[
    '$$SELECT source, target, the_geom  FROM edge_table$$'
    ]::TEXT[];

    subs = ARRAY[
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    -- no geometry
    params = ARRAY[
    '$$SELECT source, target  FROM edge_table$$'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges1$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
