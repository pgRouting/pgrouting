\i setup.sql

SELECT plan(23);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--
PREPARE query_1 AS
SELECT * FROM pgr_extractVertices(
    'SELECT source
    FROM edge_table'
);

PREPARE query_2 AS
SELECT *
FROM pgr_extractVertices(
    'SELECT target
     FROM edge_table'
);

SELECT throws_ok('query_1', 'P0001', 'Missing column', 'Incomlete data -> throws');
SELECT throws_ok('query_2', 'P0001', 'Missing column', 'Incomlete data -> throws');

--
PREPARE query_3 AS
SELECT *
FROM pgr_extractVertices(
    'SELECT ST_StartPoint(the_geom) AS startpoint
     FROM edge_table'
);

PREPARE query_4 AS
SELECT *
FROM pgr_extractVertices(
    'SELECT ST_EndPoint(the_geom) AS endpoint
     FROM edge_table'
);

SELECT throws_ok('query_3', 'P0001', 'Missing column', 'Incomlete data -> throws');
SELECT throws_ok('query_4', 'P0001', 'Missing column', 'Incomlete data -> throws');

--

PREPARE query_5 AS
SELECT * FROM pgr_extractVertices(
    'SELECT source, the_geom AS geom
    FROM edge_table'
);

PREPARE query_6 AS
SELECT *
FROM pgr_extractVertices(
    'SELECT target, the_geom AS geom
     FROM edge_table'
);

SELECT lives_ok('query_5', 'geom column makes data complete');
SELECT lives_ok('query_6', 'geom column makes data complete');

--
PREPARE query_7 AS
SELECT *
FROM pgr_extractVertices(
    'SELECT ST_StartPoint(the_geom) AS startpoint, the_geom AS geom
     FROM edge_table'
);

PREPARE query_8 AS
SELECT *
FROM pgr_extractVertices(
    'SELECT ST_EndPoint(the_geom) AS endpoint, the_geom AS geom
     FROM edge_table'
);

SELECT lives_ok('query_7', 'geom column makes data complete');
SELECT lives_ok('query_8', 'geom column makes data complete');

--

SELECT set_eq(
    $$SELECT count(*) FROM pgr_extractVertices( 'SELECT the_geom AS geom FROM edge_table')$$,
    $$VALUES (17)$$,
    '17: Number of vertices extracted');
SELECT set_eq(
    $$SELECT count(*) FROM pgr_extractVertices( 'SELECT ST_StartPoint(the_geom) AS startpoint, ST_EndPoint(the_geom) AS endpoint FROM edge_table')$$,
    $$VALUES (17)$$,
    '17: Number of vertices extracted');
SELECT set_eq(
    $$SELECT count(*) FROM pgr_extractVertices( 'SELECT source, target FROM edge_table')$$,
    $$VALUES (17)$$,
    '17: Number of vertices extracted');

SELECT set_eq(
    $$SELECT count(*) FROM pgr_extractVertices( 'SELECT id, the_geom AS geom FROM edge_table')$$,
    $$VALUES (17)$$,
    '17: Number of vertices extracted');
SELECT set_eq(
    $$SELECT count(*) FROM pgr_extractVertices( 'SELECT id, ST_StartPoint(the_geom) AS startpoint, ST_EndPoint(the_geom) AS endpoint FROM edge_table')$$,
    $$VALUES (17)$$,
    '17: Number of vertices extracted');
SELECT set_eq(
    $$SELECT count(*) FROM pgr_extractVertices( 'SELECT id, source, target FROM edge_table')$$,
    $$VALUES (17)$$,
    '17: Number of vertices extracted');

-- The results with id

SELECT * INTO result_table
FROM (VALUES
  (1 , NULL::BIGINT[] , '{1}'::BIGINT[] , 2::FLOAT, 0::FLOAT, ST_GeomFromText('POINT(2 0)')),

  (2 , '{1}'    , '{4,2}'  , 2   , 1   , ST_GeomFromText('POINT(2 1)')),
  (3 , '{2}'    , '{5,3}'  , 3   , 1   , ST_GeomFromText('POINT(3 1)')),
  (4 , '{3}'    , '{16}'   , 4   , 1   , ST_GeomFromText('POINT(4 1)')),
  (5 , '{7,4}'  , '{8,10}' , 2   , 2   , ST_GeomFromText('POINT(2 2)')),
  (6 , '{5,8}'  , '{9,11}' , 3   , 2   , ST_GeomFromText('POINT(3 2)')),
  (7 , NULL     , '{6}'    , 0   , 2   , ST_GeomFromText('POINT(0 2)')),
  (8 , '{6}'    , '{7}'    , 1   , 2   , ST_GeomFromText('POINT(1 2)')),
  (9 , '{9,16}' , '{15}'   , 4   , 2   , ST_GeomFromText('POINT(4 2)')),
 (10 , '{10}'   , '{12,14}', 2   , 3   , ST_GeomFromText('POINT(2 3)')),
 (11 , '{11,12}', '{13}'   , 3   , 3   , ST_GeomFromText('POINT(3 3)')),
 (12 , '{13,15}', NULL     , 4   , 3   , ST_GeomFromText('POINT(4 3)')),
 (13 , '{14}'   , NULL     , 2   , 4   , ST_GeomFromText('POINT(2 4)')),
 (14 , NULL     , '{17}'   , 0.5 , 3.5 , ST_GeomFromText('POINT(0.5 3.5)')),
 (15 , '{17}'   , NULL     , 1.999999999999 , 3.5 , ST_GeomFromText('POINT(1.999999999999 3.5)')),
 (16 , NULL     , '{18}'   , 3.5 , 2.3 , ST_GeomFromText('POINT(3.5 2.3)')),
 (17 , '{18}'   , NULL     , 3.5 , 4   , ST_GeomFromText('POINT(3.5 4)'))
) AS t(id, in_edges, out_edges, x, y, geom);

--
SELECT set_eq(
    $$
    SELECT x, y, geom
    FROM pgr_extractVertices($g$SELECT the_geom AS geom FROM edge_table$g$)
    $$,
    $$SELECT x, y, geom FROM result_table$$);

SELECT set_eq(
    $$
    SELECT unnest(out_edges), x, y, geom
    FROM pgr_extractVertices($g$SELECT id, the_geom AS geom FROM edge_table$g$)
    $$,
    $$SELECT unnest(out_edges), x, y, geom FROM result_table$$);

SELECT set_eq(
    $$
    SELECT unnest(in_edges), x, y, geom
    FROM pgr_extractVertices($g$SELECT id, the_geom AS geom FROM edge_table$g$)
    $$,
    $$SELECT unnest(in_edges), x, y, geom FROM result_table$$);

--
SELECT set_eq(
    $$
    SELECT x, y, geom
    FROM pgr_extractVertices($g$SELECT  ST_StartPoint(the_geom) AS startpoint, ST_EndPoint(the_geom) AS endpoint FROM edge_table$g$)
    $$,
    $$SELECT x, y, geom FROM result_table$$);

SELECT set_eq(
    $$
    SELECT unnest(out_edges), x, y, geom
    FROM pgr_extractVertices($g$SELECT id,  ST_StartPoint(the_geom) AS startpoint, ST_EndPoint(the_geom) AS endpoint FROM edge_table$g$)
    $$,
    $$SELECT unnest(out_edges), x, y, geom FROM result_table$$);

SELECT set_eq(
    $$
    SELECT unnest(in_edges), x, y, geom
    FROM pgr_extractVertices($g$SELECT id,  ST_StartPoint(the_geom) AS startpoint, ST_EndPoint(the_geom) AS endpoint FROM edge_table$g$)
    $$,
    $$SELECT unnest(in_edges), x, y, geom FROM result_table$$);


--
SELECT set_eq(
    $$
    SELECT id
    FROM pgr_extractVertices($g$SELECT  source, target FROM edge_table$g$)
    $$,
    $$SELECT id FROM result_table$$);

SELECT set_eq(
    $$
    SELECT id, unnest(out_edges)
    FROM pgr_extractVertices($g$SELECT id, source, target FROM edge_table$g$)
    $$,
    $$SELECT id, unnest(out_edges) FROM result_table$$);

SELECT set_eq(
    $$
    SELECT id, unnest(in_edges)
    FROM pgr_extractVertices($g$SELECT id, source, target FROM edge_table$g$)
    $$,
    $$SELECT id, unnest(in_edges) FROM result_table$$);


SELECT * FROM finish();
ROLLBACK;
