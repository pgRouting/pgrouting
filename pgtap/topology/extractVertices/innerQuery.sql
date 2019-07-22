\i setup.sql

SELECT plan(84);

CREATE OR REPLACE FUNCTION style_extract_normal(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with id
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'the_geom AS geom'],
    'id');

RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'ST_StartPoint(the_geom) AS startpoint, ST_ENDPoint(the_geom) AS endpoint'],
    'id');

RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target'],
    'target');

--with out id
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target'],
    'target');
END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION style_extract_dryrun(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with id
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target'],
    'id');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'the_geom AS geom'],
    'id');

RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'ST_StartPoint(the_geom) AS startpoint, ST_ENDPoint(the_geom) AS endpoint'],
    'id');

RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target'],
    'source');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target'],
    'target');

--with out id
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['source', 'target'],
    'source');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['source', 'target'],
    'target');
END;
$BODY$
LANGUAGE plpgsql;


SELECT style_extract_normal('pgr_extractVertices', ')');
SELECT style_extract_dryrun('pgr_extractVertices', ', true)');

SELECT finish();
ROLLBACK;
