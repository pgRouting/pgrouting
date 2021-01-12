
CREATE OR REPLACE FUNCTION alphaShape_tester(
    _tbl REGCLASS,
    _geom TEXT,
    alpha FLOAT,
    isEmpty BOOLEAN,
    area FLOAT,
    Npoints BIGINT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
    IF _pgr_versionless((SELECT boost from pgr_full_version()), '1.54.0') THEN
        RETURN QUERY SELECT * FROM  skip('pgr_alphaSahpe not supported when compiled with Boost version < 1.54.0', 4);
        RETURN;
    END IF;
    EXECUTE format($$
    CREATE TABLE newquery AS
    SELECT pgr_alphaShape((SELECT ST_collect(%2$I) FROM %1$I), %3$s)
    AS geom$$, _tbl, _geom, alpha);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_IsValid(geom) FROM newquery$$,
        $$SELECT true$$,
        'SHOULD BE: valid with spoon radius = ' || alpha);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT round(ST_Area(geom)::numeric,12)::TEXT FROM newquery$$,
        $$SELECT $$ || round(area::numeric,12) || '::TEXT',
        'SHOULD BE: ' || area || ' with spoon radius = ' || alpha);


    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_IsEmpty(geom) FROM newquery$$,
        $$SELECT $$ || isEmpty,
        'SHOULD BE: ' || isEmpty || ' with spoon radius = ' || alpha);


    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_Npoints(geom) FROM newquery$$,
        $$SELECT $$ || Npoints,
        'SHOULD BE: ' || Npoints || ' points with spoon radius = ' || alpha);

    DROP TABLE newquery;

END
$BODY$
LANGUAGE plpgsql VOLATILE;
