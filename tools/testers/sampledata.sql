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
DROP TABLE IF EXISTS new_restrictions;
DROP TABLE IF EXISTS retrict;
DROP TABLE IF EXISTS vertex_table;
DROP TABLE IF EXISTS categories;
DROP TABLE IF EXISTS vehicles;
DROP TABLE IF EXISTS orders;

--EDGE TABLE CREATE start
CREATE TABLE edge_table (
    id BIGSERIAL,
    dir character varying,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT,
    capacity BIGINT,
    reverse_capacity BIGINT,
    category_id INTEGER,
    reverse_category_id INTEGER,
    x1 FLOAT,
    y1 FLOAT,
    x2 FLOAT,
    y2 FLOAT,
    the_geom geometry
);
--EDGE TABLE CREATE end
--EDGE TABLE ADD DATA start
INSERT INTO edge_table (
    category_id, reverse_category_id,
    cost, reverse_cost,
    capacity, reverse_capacity,
    x1, y1,
    x2, y2) VALUES
(3, 1,    1,  1,  80, 130,   2,   0,    2, 1),
(3, 2,   -1,  1,  -1, 100,   2,   1,    3, 1),
(2, 1,   -1,  1,  -1, 130,   3,   1,    4, 1),
(2, 4,    1,  1, 100,  50,   2,   1,    2, 2),
(1, 4,    1, -1, 130,  -1,   3,   1,    3, 2),
(4, 2,    1,  1,  50, 100,   0,   2,    1, 2),
(4, 1,    1,  1,  50, 130,   1,   2,    2, 2),
(2, 1,    1,  1, 100, 130,   2,   2,    3, 2),
(1, 3,    1,  1, 130,  80,   3,   2,    4, 2),
(1, 4,    1,  1, 130,  50,   2,   2,    2, 3),
(1, 2,    1, -1, 130,  -1,   3,   2,    3, 3),
(2, 3,    1, -1, 100,  -1,   2,   3,    3, 3),
(2, 4,    1, -1, 100,  -1,   3,   3,    4, 3),
(3, 1,    1,  1,  80, 130,   2,   3,    2, 4),
(3, 4,    1,  1,  80,  50,   4,   2,    4, 3),
(3, 3,    1,  1,  80,  80,   4,   1,    4, 2),
(1, 2,    1,  1, 130, 100,   0.5, 3.5,  1.999999999999,3.5),
(4, 1,    1,  1,  50, 130,   3.5, 2.3,  3.5,4);
--EDGE TABLE ADD DATA end

--EDGE TABLE update geometry start

UPDATE edge_table SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
dir = CASE WHEN (cost>0 AND reverse_cost>0) THEN 'B'   -- both ways
           WHEN (cost>0 AND reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
           WHEN (cost<0 AND reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
           ELSE '' END;                                -- unknown

--EDGE TABLE update geometry end

--EDGE TABLE TOPOLOGY start
SELECT pgr_createTopology('edge_table',0.001);
--EDGE TABLE TOPOLOGY end

--POINTS CREATE start
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
--POINTS CREATE end

--RESTRICTIONS CREATE start
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

CREATE TABLE new_restrictions (
    id SERIAL PRIMARY KEY,
    path BIGINT[],
    cost float
);

INSERT INTO new_restrictions (path, cost) VALUES
(ARRAY[4, 7], 100),
(ARRAY[8, 11], 100),
(ARRAY[4, 8], 100),
(ARRAY[5, 9], 100),
(ARRAY[10, 12], 100),
(ARRAY[9, 15], 100),
(ARRAY[3, 5, 8], 100);
--RESTRICTIONS CREATE end


--VEHICLES TABLE START

CREATE TABLE vehicles (
      id BIGSERIAL PRIMARY KEY,
      start_node_id BIGINT,
      start_x FLOAT,
      start_y FLOAT,
      start_open FLOAT,
      start_close FLOAT,
      number integer,
      capacity FLOAT
);

INSERT INTO vehicles
(start_node_id, start_x,  start_y,  start_open,  start_close,  number,  capacity) VALUES
(            6,       3,        2,           0,           50,       2,        50);

--VEHICLES TABLE END



--ORDERS TABLE START
CREATE TABLE orders (
    id BIGSERIAL PRIMARY KEY,
    demand FLOAT,
    -- the pickups
    p_node_id BIGINT,
    p_x FLOAT,
    p_y FLOAT,
    p_open FLOAT,
    p_close FLOAT,
    p_service FLOAT,
    -- the deliveries
    d_node_id BIGINT,
    d_x FLOAT,
    d_y FLOAT,
    d_open FLOAT,
    d_close FLOAT,
    d_service FLOAT
);


INSERT INTO orders
(demand,
    p_node_id,  p_x, p_y,  p_open,  p_close,  p_service,
    d_node_id,  d_x, d_y,  d_open,  d_close,  d_service) VALUES
(10,
            3,    3,   1,      2,         10,          3,
            8,    1,   2,      6,         15,          3),
(20,
            9,    4,   2,      4,         15,          2,
            4,    4,   1,      6,         20,          3),
(30,
            5,    2,   2,      2,         10,          3,
           11,    3,   3,      3,         20,          3);


--ORDERS TABLE END

--COMBINATIONS CREATE start
CREATE TABLE combinations_table (
    source BIGINT,
    target BIGINT
);

INSERT INTO combinations_table (
    source, target) VALUES
(1, 2),
(1, 4),
(2, 1),
(2, 4),
(2, 17);

--COMBINATIONS CREATE end
