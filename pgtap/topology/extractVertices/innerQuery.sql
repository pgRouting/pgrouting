\i setup.sql

SELECT plan(10);

CREATE OR REPLACE FUNCTION style_id_s_t(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with the_geom
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'the_geom'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'the_geom'],
    'target');
END;
$BODY$
LANGUAGE plpgsql;

SELECT style_id_s_t('pgr_extractVertices', ')');

SELECT finish();
ROLLBACK;
