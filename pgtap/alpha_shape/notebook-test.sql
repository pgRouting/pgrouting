\i setup.sql

/*
Data from
Benchmark tests with data from:
https://github.com/plotly/documentation/blob/source-design-merge/_posts/python/scientific/alpha-shapes/data-ex-2d.txt
https://plot.ly/python/alpha-shapes/
*/
SELECT plan(147);


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


-- several alpha values that give the concave hull
SELECT alphaShape_tester('e_test', 'geom', 100, false, 0.66339515423, 18);
SELECT alphaShape_tester('e_test', 'geom', 4,   false, 0.66339515423, 18);
SELECT alphaShape_tester('e_test', 'geom', 3.2, false, 0.66339515423, 18);
SELECT alphaShape_tester('e_test', 'geom', 3.16, false, 0.66339515423, 18);
SELECT alphaShape_tester('e_test', 'geom', 3.151, false, 0.66339515423, 18);
SELECT alphaShape_tester('e_test', 'geom', 3.1504, false, 0.66339515423, 18);
SELECT alphaShape_tester('e_test', 'geom', 3.15036, false, 0.66339515423, 18);
SELECT alphaShape_tester('e_test', 'geom', 3.15035, false, 0.66339515423, 18);
-- end of concave hull
SELECT alphaShape_tester('e_test', 'geom', 3.15034, false, 0.6608269921575, 19);
SELECT alphaShape_tester('e_test', 'geom', 3.1503, false, 0.6608269921575, 19);
SELECT alphaShape_tester('e_test', 'geom', 3.150, false, 0.6608269921575, 19);
SELECT alphaShape_tester('e_test', 'geom', 3.15, false, 0.6608269921575, 19);
SELECT alphaShape_tester('e_test', 'geom', 3.1, false, 0.6608269921575, 19);
-- several alpha values
SELECT alphaShape_tester('e_test', 'geom', 3,   false, 0.6608269921575, 19);
SELECT alphaShape_tester('e_test', 'geom', 2,   false, 0.6608269921575, 19);
SELECT alphaShape_tester('e_test', 'geom', 1,   false, 0.6608269921575, 19);
SELECT alphaShape_tester('e_test', 'geom', 0.5, false, 0.6553009782815, 20);
SELECT alphaShape_tester('e_test', 'geom', 0.4, false, 0.6460244460635, 21);
SELECT alphaShape_tester('e_test', 'geom', 0.3, false, 0.6405578500125, 22);
-- near best alpha
SELECT alphaShape_tester('e_test', 'geom', 0.25, false, 0.6405578500125, 22);
SELECT alphaShape_tester('e_test', 'geom', 0.24, false, 0.5375455501905, 25);
SELECT alphaShape_tester('e_test', 'geom', 0.23, false, 0.5178261889305, 26);

-- several alpha values near best alpha
SELECT alphaShape_tester('e_test', 'geom', 0.22969, false, 0.5178261889305, 26);


-- best alpha
SELECT set_eq(
    $$SELECT round(st_area(pgr_alphaShape)::numeric, 12) FROM pgr_alphaShape((SELECT ST_Collect(geom) FROM e_test))$$,
    $$SELECT round(st_area(pgr_alphaShape)::numeric, 12) FROM pgr_alphaShape((SELECT ST_Collect(geom) FROM e_test), 0)$$,
    'SHOULD BE: best alpha obtined with spoon radius 0'
);

-- best alpha range
SELECT alphaShape_tester('e_test', 'geom', 0.22968, false, 0.495862454676, 27);
SELECT alphaShape_tester('e_test', 'geom', 0.22214, false, 0.495862454676, 27);

-- several alpha values near best alpha
SELECT alphaShape_tester('e_test', 'geom', 0.2221, false, 0.433826261046, 31);
SELECT alphaShape_tester('e_test', 'geom', 0.222,  false, 0.433826261046, 31);
SELECT alphaShape_tester('e_test', 'geom', 0.22,   false, 0.433826261046, 31);

SELECT alphaShape_tester('e_test', 'geom', 0.20, false, 0.3639712121535, 31);
SELECT alphaShape_tester('e_test', 'geom', 0.1, false, 0.0662883570555, 26);

SELECT alphaShape_tester('e_test', 'geom', 0.05, false, 0.002885695472, 4);
SELECT alphaShape_tester('e_test', 'geom', 0.04983, false, 0.002885695472, 4);
-- no geometry
SELECT alphaShape_tester('e_test', 'geom', 0.04982, true, 0, 0);
SELECT alphaShape_tester('e_test', 'geom', 0.04981, true, 0, 0);
SELECT alphaShape_tester('e_test', 'geom', 0.0498, true, 0, 0);
SELECT alphaShape_tester('e_test', 'geom', 0.01, true, 0, 0);

SELECT finish();

