\i setup.sql

SELECT plan(54);

PREPARE edges AS
SELECT the_geom AS geom FROM edge_table;

PREPARE edges1 AS
SELECT id, the_geom AS geom FROM edge_table;

PREPARE edges2 AS
SELECT ST_startPoint(the_geom) AS startpoint, ST_startPoint(the_geom) AS endpoint FROM edge_table;

PREPARE edges3 AS
SELECT id, ST_startPoint(the_geom) AS startpoint, ST_startPoint(the_geom) AS endpoint FROM edge_table;

PREPARE edges4 AS
SELECT source, target FROM edge_table;

PREPARE edges5 AS
SELECT id, source, target FROM edge_table;

SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('edges1', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('edges2', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('edges3', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('edges4', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('edges5', 'Should be not empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- with geometry
    params = ARRAY[
    '$$SELECT the_geom AS geom FROM edge_table$$'
    ]::TEXT[];

    subs = ARRAY[
    'NULL'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    -- with geometry and id
    params = ARRAY[
    '$$SELECT id, the_geom AS geom FROM edge_table$$'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges1$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    -- with startpoint & endpoint
    params = ARRAY[
    '$$SELECT ST_startPoint(the_geom) AS startpoint, ST_startPoint(the_geom) AS endpoint FROM edge_table$$'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges2$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    -- with startpoint & endpoint and id
    params = ARRAY[
    '$$SELECT id, ST_startPoint(the_geom) AS startpoint, ST_startPoint(the_geom) AS endpoint FROM edge_table$$'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges3$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    -- with source & target
    params = ARRAY[
    '$$SELECT source, target FROM edge_table$$'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges3$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    -- with source & target & id
    params = ARRAY[
    '$$SELECT id, source, target FROM edge_table$$'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

    params[1] := '$$edges3$$';
    RETURN query SELECT * FROM no_crash_test('pgr_extractVertices', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;




SELECT * FROM test_function();

ROLLBACK;
