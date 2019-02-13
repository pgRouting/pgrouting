\i setup.sql

/*
Test for old code VS new code results
Data from
https://github.com/plotly/documentation/blob/source-design-merge/_posts/python/scientific/alpha-shapes/data-ex-2d.txt
*/
SELECT plan(39);


CREATE TABLE e_test(geom geometry);
INSERT INTO e_test(geom) VALUES (ST_GeomFromText('MULTIPOINT(
(0.072213 0.616682), (0.115641 0.256523), (0.709330 0.077903),
(0.887964 0.372909), (0.796091 0.624687), (0.780431 0.069521),
(0.222984 0.266937), (0.773456 0.151515), (0.681843 0.500971),
(0.238492 0.623246), (0.465003 0.922338), (0.162313 0.481267),
(0.950678 0.447069), (0.649028 0.803023), (0.128857 0.013585),
(0.594825 0.671393), (0.465626 0.745341), (0.431693 0.989688),
(0.045125 0.219672), (0.540694 0.005390), (0.691728 0.573342),
(0.717810 0.898306), (0.174244 0.180820), (0.270756 0.310889),
(0.410399 0.182079), (0.139572 0.704512), (0.838489 0.642405),
(0.348582 0.240018), (0.305693 0.803172), (0.397964 0.863419))
') ) ;

SELECT results_eq('SELECT count(*) FROM (SELECT ST_DumpPoints(geom) FROM e_test) AS a', 'SELECT 30::BIGINT');
SELECT results_eq('SELECT count(*) FROM (SELECT (ST_DumpPoints(geom)).geom FROM e_test) a;', 'SELECT 30::BIGINT');

CREATE OR REPLACE FUNCTION test_alpha(alpha FLOAT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
    CREATE TABLE original AS
    SELECT pgr_pointsAsPolygon($$
        WITH
        Points AS (SELECT (st_dumppoints(geom)).geom FROM e_test)
        SELECT row_number() over()::INTEGER AS id, ST_X(geom) AS x, ST_Y(geom) AS y FROM Points$$, alpha)
    AS geom;

    CREATE TABLE newquery AS
    SELECT pgr_alphaShape1((SELECT array_agg(geom) FROM e_test), sqrt(alpha))
    AS geom;

    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_IsValid(geom) FROM original$$,
        $$SELECT true$$);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_IsValid(geom) FROM newquery$$,
        $$SELECT true$$);


    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_Area(geom)::TEXT FROM original$$,
        $$SELECT ST_Area(geom)::TEXT FROM newquery$$);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT count(*) FROM (SELECT ST_DumpPoints(geom) FROM original) AS a$$,
        $$SELECT count(*) FROM (SELECT ST_DumpPoints(geom) FROM newquery) AS a$$);

    RETURN QUERY
    SELECT results_eq(
        $$SELECT ST_NPoints(geom) FROM original$$,
        $$SELECT ST_NPoints(geom) FROM newquery$$);

    RETURN QUERY
    SELECT set_eq(
        $$SELECT (ST_DumpPoints(geom)).geom FROM original$$,
        $$SELECT (ST_DumpPoints(geom)).geom FROM newquery$$);

    DROP TABLE original;
    DROP TABLE newquery;

END
$BODY$
LANGUAGE plpgsql VOLATILE;

-- less than 0.33 points as polygon does not give a result
PREPARE q1 AS
SELECT count(*)
FROM (SELECT pgr_pointsAsPolygon($$
        WITH
        Points AS (SELECT (st_dumppoints(geom)).geom FROM e_test)
        SELECT row_number() over()::INTEGER AS id, ST_X(geom) AS x, ST_Y(geom) AS y FROM Points$$, 0.032) AS geom) a
WHERE geom IS NOT NULL;

SELECT results_eq(
        'q1',
        $$SELECT 0::BIGINT$$);

SELECT test_alpha(0.33);
SELECT test_alpha(1);
SELECT test_alpha(5);
SELECT test_alpha(9);
SELECT test_alpha(10);
SELECT test_alpha(11);

