\i setup.sql

/*
data from
http://www.bostongis.com/postgis_concavehull.snippet
*/
SELECT plan(167);

CREATE TABLE s_test(geom geometry);

INSERT INTO s_test(geom) VALUES(ST_GeomFromText('MULTIPOINT(
(120 -224), (185 -219), (190 -234), (200 -246), (212 -256), (227 -261),
(242 -264), (257 -265), (272 -264), (287 -263),(302 -258), (315 -250),
(323 -237), (321 -222), (308 -213), (293 -208), (278 -205), (263 -202),
(248 -199), (233 -196), (218 -193), (203 -190), (188 -185), (173 -180),
(160 -172), (148 -162), (138 -150), (133 -135), (132 -120), (136 -105),
(146  -92), (158  -82), (171  -74), (186  -69), (201  -65), (216  -62),
(231  -60), (246  -60), (261  -60), (276  -60), (291  -61), (306  -64),
(321  -67), (336  -72), (349  -80), (362  -89), (372 -101), (379 -115),
(382 -130), (314 -134), (309 -119), (298 -108), (284 -102), (269 -100),
(254  -99), (239 -100), (224 -102), (209 -107), (197 -117), (200 -132),
(213 -140), (228 -145), (243 -148), (258 -151), (273 -154), (288 -156),
(303 -159), (318 -163), (333 -167), (347 -173), (361 -179), (373 -189),
(383 -201), (389 -215), (391 -230), (390 -245), (384 -259), (374 -271),
(363 -282), (349 -289), (335 -295), (320 -299), (305 -302), (290 -304),
(275 -305), (259 -305), (243 -305), (228 -304), (213 -302), (198 -300),
(183 -295), (169 -289), (155 -282), (143 -272), (133 -260), (126 -246),
(136 -223), (152 -222), (168 -221), (365 -131), (348 -132), (331 -133),
(251 -177), (183 -157), (342  -98), (247  -75), (274 -174), (360 -223),
(192  -85), (330 -273), (210 -283), (326  -97), (177 -103), (315 -188),
(175 -139), (366 -250), (321 -204), (344 -232), (331 -113), (162 -129),
(272  -77), (292 -192), (144 -244), (196 -272), (212  -89), (166 -236),
(238 -167), (289 -282), (333 -187), (341 -249), (164 -113), (238 -283),
(344 -265), (176 -248), (312 -273), (299  -85), (154 -261), (265 -287),
(359 -111), (160 -150), (212 -169), (351 -199), (160  -98), (228  -77),
(376 -224), (148 -120), (174 -272), (194 -100), (292 -173), (341 -212),
(369 -209), (189 -258), (198 -159), (275 -190), (322  -82))') ) ;

SELECT results_eq('SELECT count(*) FROM (SELECT ST_DumpPoints(geom) FROM s_test) AS a', 'SELECT 155::BIGINT');
SELECT results_eq('SELECT count(*) FROM (SELECT (ST_DumpPoints(geom)).geom FROM s_test) a;', 'SELECT 155::BIGINT');

-- convex hull
SELECT alphaShape_tester('s_test', 'geom', 100000, false, 56849, 45);
SELECT alphaShape_tester('s_test', 'geom', 1000, false, 56849, 45);
SELECT alphaShape_tester('s_test', 'geom', 997, false, 56849, 45);
SELECT alphaShape_tester('s_test', 'geom', 996.9, false, 56849, 45);
SELECT alphaShape_tester('s_test', 'geom', 996.82, false, 56849, 45);
SELECT alphaShape_tester('s_test', 'geom', 996.812, false, 56849, 45);
-- end of convex hull
SELECT alphaShape_tester('s_test', 'geom', 996.811, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 996.81, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 996.8, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 996, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 990, false, 56816.5, 46);
-- several values for alpha
SELECT alphaShape_tester('s_test', 'geom', 900, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 800, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 700, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 600, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 500, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 300, false, 56816.5, 46);
SELECT alphaShape_tester('s_test', 'geom', 200, false, 56674.5, 47);
SELECT alphaShape_tester('s_test', 'geom', 100, false, 56468.5, 48);
SELECT alphaShape_tester('s_test', 'geom', 50, false, 56148.5, 49);
SELECT alphaShape_tester('s_test', 'geom', 43, false, 55799.5, 50);

-- several values of alpha for best alpha
SELECT alphaShape_tester('s_test', 'geom', 42, false, 55321.5, 51);
SELECT alphaShape_tester('s_test', 'geom', 41, false, 55321.5, 51);

SELECT todo_start('With postgres 9.4 sometimes gives another result');
-- best alpha
SELECT set_eq(
    $$SELECT st_area(pgr_alphaShape)::TEXT FROM pgr_alphaShape((SELECT ST_Collect(geom) FROM s_test))$$,
    $$SELECT st_area(pgr_alphaShape)::TEXT FROM pgr_alphaShape((SELECT ST_Collect(geom) FROM s_test), 0)$$
);
SELECT todo_end();

SELECT alphaShape_tester('s_test', 'geom', 40, false, 55321.5, 51);
SELECT alphaShape_tester('s_test', 'geom', 39, false, 55321.5, 51);
SELECT alphaShape_tester('s_test', 'geom', 38, false, 55321.5, 51);
SELECT alphaShape_tester('s_test', 'geom', 37, false, 55321.5, 51);
SELECT alphaShape_tester('s_test', 'geom', 36, false, 55321.5, 51);

-- several values of alpha near best alpha
SELECT alphaShape_tester('s_test', 'geom', 35, false, 54839.5, 52);
SELECT alphaShape_tester('s_test', 'geom', 30, false, 49736.5, 66);
SELECT alphaShape_tester('s_test', 'geom', 20, false, 39136.5, 100);

-- several values for alpha
SELECT alphaShape_tester('s_test', 'geom', 10, false, 1505, 52);
SELECT alphaShape_tester('s_test', 'geom', 9.1, false, 212.5, 8);
SELECT alphaShape_tester('s_test', 'geom', 9.05, false, 106, 4);
SELECT alphaShape_tester('s_test', 'geom', 9.049, false, 106, 4);
SELECT alphaShape_tester('s_test', 'geom', 9.0482, false, 106, 4);

-- no geometry
SELECT alphaShape_tester('s_test', 'geom', 9.0481, true, 0, 0);
SELECT alphaShape_tester('s_test', 'geom', 9.048, true, 0, 0);
SELECT alphaShape_tester('s_test', 'geom', 9.04, true, 0, 0);
SELECT alphaShape_tester('s_test', 'geom', 9.0, true, 0, 0);
SELECT alphaShape_tester('s_test', 'geom', 1, true, 0, 0);




ROLLBACK;
