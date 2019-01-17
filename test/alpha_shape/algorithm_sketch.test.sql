
CREATE TABLE vertices(
    gid SERIAL,
    x FLOAT,
    y FLOAT,
    geom geometry
);
INSERT INTO vertices (x,y) VALUES
(162, 332), (182, 299), (141, 292), (158, 264), (141, 408), (160, 400),
(177, 430), (151, 442), (155, 425), (134, 430), (126, 447), (139, 466),
(160, 471), (167, 447), (182, 466), (192, 442), (187, 413), (173, 403),
(168, 425), (153, 413), (179, 275), (163, 292), (134, 270), (143, 315),
(177, 320), (163, 311), (162, 281), (182, 255), (141, 226), (156, 235),
(173, 207), (187, 230), (204, 194), (165, 189), (145, 201), (158, 167),
(190, 165), (206, 145), (179, 153), (204, 114), (221, 138),
(243, 112), (248, 139), (177, 122), (179, 99), (196, 82),
(219, 90), (240, 75), (218, 61), (228, 53), (211, 34),
(197, 51), (179, 65), (155, 70), (165, 85), (134, 80),
(124, 58), (153, 44), (173, 34), (192, 27), (156, 19),
(119, 32), (128, 17), (138, 36), (100, 58), (112, 73),
(100, 92), (78, 100), (83, 78), (61, 63), (80, 44),
(100, 26), (60, 39), (43, 71), (34, 54), (32, 90),
(53, 104), (60, 82), (66, 99), (247, 94), (187, 180),
(221, 168);

UPDATE vertices
SET geom = ST_makePoint(x,y);

SELECT a.path[1], a.geom
INTO delauny
FROM (SELECT (ST_dump(ST_DelaunayTriangles(ST_union(geom), 0.5, 0))).*
    FROM vertices) AS a;

WITH
calculations AS (
    SELECT  DISTINCT path
    FROM (SELECT path, ST_Boundary(geom) as geom from delauny) AS t
    CROSS JOIN LATERAL generate_series(1, ST_NPoints(geom) - 1)
    AS gs(x)
    CROSS JOIN LATERAL ST_MakeLine(
        ST_PointN(geom, x),
        ST_PointN(geom, x+1)
    ) AS line(geom) where ST_length(line.geom) > 50
)
DELETE FROM delauny
WHERE path IN (SELECT path from calculations);

-- creating an edge table
WITH
calculations AS (
    SELECT row_number() over() AS gid, NULL::BIGINT AS source, NULL::BIGINT AS target, 1 as cost, line.geom, ST_AsText(line.geom) AS line, ST_length(line.geom) AS lenght, path
    FROM (SELECT path, ST_Boundary(geom) AS geom
        FROM delauny) AS t
    CROSS JOIN LATERAL generate_series(1, ST_NPoints(geom) - 1)
    AS gs(x)
    CROSS JOIN LATERAL ST_MakeLine(
        ST_PointN(geom, x),
        ST_PointN(geom, x+1)
    ) AS line(geom)
)
SELECT *
INTO edges FROM calculations;

-- creating the topology for pgrouting
SELECT * from pgr_createTopology('edges', 0.5, the_geom := 'geom', id := 'gid');

WITH
centralEdges AS (
    SELECT a.gid
    FROM edges AS a
    LEFT JOIN edges AS b
    ON(a.source = b.target AND a.target = b.source)
    WHERE b.gid IS NOT NULL)
-- internal edges are kept
DELETE FROM edges
WHERE gid IN (SELECT gid FROM centralEdges);

-- connected components
WITH
connected AS (
    SELECT * FROM pgr_connectedComponents('SELECT gid as id, source, target, cost FROM edges')
),
the_geom AS (
    SELECT component, geom FROM edges
    JOIN connected ON (source = node)

    UNION

    SELECT component, geom FROM edges
    JOIN connected ON (target = node)
)
SELECT component, st_linemerge(ST_collect(geom)) FROM the_geom GROUP BY component;

