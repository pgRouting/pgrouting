\set QUIET 1

SET client_min_messages = WARNING;


------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              SAMPLE DATA                
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

    DROP TABLE IF EXISTS edge_table;
    DROP TABLE IF EXISTS edge_table_vertices_pgr;
    DROP table if exists pointsOfInterest;
    DROP TABLE IF EXISTS restrictions;
    DROP TABLE IF EXISTS vertex_table;
    DROP TABLE IF EXISTS categories;

--EDGE TABLE CREATE
CREATE TABLE edge_table (
    id BIGSERIAL,
    dir character varying,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT,
    category_id INTEGER,
    reverse_category_id INTEGER,
    x1 FLOAT,
    y1 FLOAT,
    x2 FLOAT,
    y2 FLOAT,
    the_geom geometry
);
--EDGE TABLE ADD DATA
INSERT INTO edge_table (
    category_id, reverse_category_id,
    cost, reverse_cost,
    x1, y1,
    x2, y2) VALUES 
(3, 1,    1,  1,  2,   0,    2, 1),
(3, 2,   -1,  1,  2,   1,    3, 1),
(2, 1,   -1,  1,  3,   1,    4, 1),
(2, 4,    1,  1,  2,   1,    2, 2),
(1, 4,    1, -1,  3,   1,    3, 2),
(4, 2,    1,  1,  0,   2,    1, 2),
(4, 1,    1,  1,  1,   2,    2, 2),
(2, 1,    1,  1,  2,   2,    3, 2),
(1, 3,    1,  1,  3,   2,    4, 2),
(1, 4,    1,  1,  2,   2,    2, 3),
(1, 2,    1, -1,  3,   2,    3, 3),
(2, 3,    1, -1,  2,   3,    3, 3),
(2, 4,    1, -1,  3,   3,    4, 3),
(3, 1,    1,  1,  2,   3,    2, 4),
(3, 4,    1,  1,  4,   2,    4, 3),
(3, 3,    1,  1,  4,   1,    4, 2),
(1, 2,    1,  1,  0.5, 3.5,  1.999999999999,3.5),
(4, 1,    1,  1,  3.5, 2.3,  3.5,4);

UPDATE edge_table SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
dir = CASE WHEN (cost>0 AND reverse_cost>0) THEN 'B'   -- both ways
           WHEN (cost>0 AND reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
           WHEN (cost<0 AND reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
           ELSE '' END;                                -- unknown

--EDGE TABLE TOPOLOGY
SELECT pgr_createTopology('edge_table',0.001);

--POINTS CREATE
CREATE TABLE pointsOfInterest(
    pid BIGSERIAL,
    x FLOAT,
    y FLOAT,
    edge_id BIGINT,
    side CHAR,
    fraction FLOAT,
    the_geom geometry,
    newPoint geometry
);

INSERT INTO pointsOfInterest (x, y, edge_id, side, fraction) VALUES
(1.8, 0.4,   1, 'l', 0.4),
(4.2, 2.4,  15, 'r', 0.4),
(2.6, 3.2,  12, 'l', 0.6),
(0.3, 1.8,   6, 'r', 0.3),
(2.9, 1.8,   5, 'l', 0.8),
(2.2, 1.7,   4, 'b', 0.7);
UPDATE pointsOfInterest SET the_geom = st_makePoint(x,y);

UPDATE pointsOfInterest
    SET newPoint = ST_LineInterpolatePoint(e.the_geom, fraction)
    FROM edge_table AS e WHERE edge_id = id;

--RESTRICTIONS CREATE
CREATE TABLE restrictions (
    rid BIGINT NOT NULL,
    to_cost FLOAT,
    target_id BIGINT,
    from_edge BIGINT,
    via_path TEXT
);

INSERT INTO restrictions (rid, to_cost, target_id, from_edge, via_path) VALUES
(1, 100,  7,  4, NULL),
(1, 100, 11,  8, NULL),
(1, 100, 10,  7, NULL),
(2,   4,  8,  3, 5),
(3, 100,  9, 16, NULL);

--RESTRICTIONS END

CREATE TABLE categories (
    category_id INTEGER,
    category text,
    capacity BIGINT
);

INSERT INTO categories VALUES
(1, 'Category 1', 130),
(2, 'Category 2', 100),
(3, 'Category 3',  80),
(4, 'Category 4',  50);

--CATEGORIES END

CREATE TABLE vertex_table (
    id SERIAL,
    x FLOAT,
    y FLOAT
);
INSERT INTO vertex_table VALUES
(1,2,0), (2,2,1), (3,3,1), (4,4,1), (5,0,2), (6,1,2), (7,2,2),
(8,3,2), (9,4,2), (10,2,3), (11,3,3), (12,4,3), (13,2,4);

--VERTEX TABLE END

