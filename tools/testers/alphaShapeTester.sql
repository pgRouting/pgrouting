
CREATE OR REPLACE FUNCTION test_alpha(_tbl REGCLASS, _geom TEXT, alpha FLOAT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
    CREATE TABLE original AS
    SELECT pgr_pointsAsPolygon(format($$
        WITH
        Points AS (SELECT (st_dumppoints(%2$I)).geom FROM %1$I)
        SELECT row_number() over()::INTEGER AS id, ST_X(geom) AS x, ST_Y(geom) AS y FROM Points$$,
        _tbl, _geom), alpha)
    AS geom;

    EXECUTE format($$
    CREATE TABLE newquery AS
    SELECT pgr_alphaShape1((SELECT array_agg(%2$I) FROM %1$I), sqrt(%3$s))
    AS geom$$, _tbl, _geom, alpha);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_IsValid(geom) FROM original$$,
        $$SELECT true$$,
        'Original should be valid with alpha = ' || alpha);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_IsValid(geom) FROM newquery$$,
        $$SELECT true$$,
        'New results should be valid with alpha = ' || alpha);


    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_Area(geom)::TEXT FROM original$$,
        $$SELECT ST_Area(geom)::TEXT FROM newquery$$,
        'Areas are the same with alpha = ' || alpha);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT count(*) FROM (SELECT ST_DumpPoints(geom) FROM original) AS a$$,
        $$SELECT count(*) FROM (SELECT ST_DumpPoints(geom) FROM newquery) AS a$$,
        'Number of points are the same with alpha = ' || alpha);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_NPoints(geom) FROM original$$,
        $$SELECT ST_NPoints(geom) FROM newquery$$,
        'NPoints are the same with alpha = ' || alpha);

    RETURN QUERY
    SELECT set_eq(
        $$SELECT (ST_DumpPoints(geom)).geom FROM original$$,
        $$SELECT (ST_DumpPoints(geom)).geom FROM newquery$$,
        'Points are the same with alpha = ' || alpha);

    DROP TABLE original;
    DROP TABLE newquery;

END
$BODY$
LANGUAGE plpgsql VOLATILE;

