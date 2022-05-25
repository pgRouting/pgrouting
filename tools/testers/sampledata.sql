------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              SAMPLE DATA
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

DROP TABLE IF EXISTS edge_table;
DROP TABLE IF EXISTS edge_table_vertices_pgr;
DROP table if exists pointsOfInterest;
DROP TABLE IF EXISTS old_restrictions;
DROP TABLE IF EXISTS restrictions;
DROP TABLE IF EXISTS retrict;
DROP TABLE IF EXISTS combinations_table;
DROP TABLE IF EXISTS vertex_table;
DROP TABLE IF EXISTS categories;
DROP TABLE IF EXISTS vehicles;
DROP TABLE IF EXISTS orders;


/* --EDGE TABLE CREATE start */
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
    geom geometry
);
/* --EDGE TABLE CREATE end */
/* --EDGE TABLE ADD DATA start */
INSERT INTO edge_table (
    category_id, reverse_category_id,
    cost, reverse_cost,
    capacity, reverse_capacity, geom) VALUES
(3, 1,    1,  1,  80, 130,   ST_MakeLine(ST_POINT(2, 0), ST_POINT(2, 1))),
(3, 2,   -1,  1,  -1, 100,   ST_MakeLine(ST_POINT(2, 1), ST_POINT(3, 1))),
(2, 1,   -1,  1,  -1, 130,   ST_MakeLine(ST_POINT(3, 1), ST_POINT(4, 1))),
(2, 4,    1,  1, 100,  50,   ST_MakeLine(ST_POINT(2, 1), ST_POINT(2, 2))),
(1, 4,    1, -1, 130,  -1,   ST_MakeLine(ST_POINT(3, 1), ST_POINT(3, 2))),
(4, 2,    1,  1,  50, 100,   ST_MakeLine(ST_POINT(0, 2), ST_POINT(1, 2))),
(4, 1,    1,  1,  50, 130,   ST_MakeLine(ST_POINT(1, 2), ST_POINT(2, 2))),
(2, 1,    1,  1, 100, 130,   ST_MakeLine(ST_POINT(2, 2), ST_POINT(3, 2))),
(1, 3,    1,  1, 130,  80,   ST_MakeLine(ST_POINT(3, 2), ST_POINT(4, 2))),
(1, 4,    1,  1, 130,  50,   ST_MakeLine(ST_POINT(2, 2), ST_POINT(2, 3))),
(1, 2,    1, -1, 130,  -1,   ST_MakeLine(ST_POINT(3, 2), ST_POINT(3, 3))),
(2, 3,    1, -1, 100,  -1,   ST_MakeLine(ST_POINT(2, 3), ST_POINT(3, 3))),
(2, 4,    1, -1, 100,  -1,   ST_MakeLine(ST_POINT(3, 3), ST_POINT(4, 3))),
(3, 1,    1,  1,  80, 130,   ST_MakeLine(ST_POINT(2, 3), ST_POINT(2, 4))),
(3, 4,    1,  1,  80,  50,   ST_MakeLine(ST_POINT(4, 2), ST_POINT(4, 3))),
(3, 3,    1,  1,  80,  80,   ST_MakeLine(ST_POINT(4, 1), ST_POINT(4, 2))),
(1, 2,    1,  1, 130, 100,   ST_MakeLine(ST_POINT(0.5, 3.5), ST_POINT(1.999999999999, 3.5))),
(4, 1,    1,  1,  50, 130,   ST_MakeLine(ST_POINT(3.5, 2.3), ST_POINT(3.5, 4)));
/* --EDGE TABLE ADD DATA end */

/* -- Create the vertices table */
SELECT  * INTO edge_table_vertices_pgr
FROM pgr_extractVertices('SELECT id, geom FROM edge_table ORDER BY id');

/* -- Inspect the vertices table */
SELECT * FROM edge_table_vertices_pgr;

/* -- Create the topology */
/* -- set the source information */
WITH
out_going AS (
  SELECT id AS vid, unnest(out_edges) AS eid, x, y
  FROM edge_table_vertices_pgr
)
UPDATE edge_table
SET source = vid, x1 = x, y1 = y
FROM out_going WHERE id = eid;

/* -- set the target information */
WITH
in_coming AS (
  SELECT id AS vid, unnest(in_edges) AS eid, x, y
  FROM edge_table_vertices_pgr
)
UPDATE edge_table
SET target = vid, x2 = x, y2 = y
FROM in_coming WHERE id = eid;

/* -- inspect the routing topology*/
SELECT id, source, target
FROM edge_table ORDER BY id;

/* --EDGE TABLE update geometry start */

/*
UPDATE edge_table SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
dir = CASE WHEN (cost>0 AND reverse_cost>0) THEN 'B'   -- both ways
           WHEN (cost>0 AND reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
           WHEN (cost<0 AND reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
           ELSE '' END;                                -- unknown
*/
/* --EDGE TABLE update geometry end */

/* --EDGE TABLE TOPOLOGY start */
/*
SELECT pgr_createTopology('edge_table',0.001);
*/
/* --EDGE TABLE TOPOLOGY end */

/* --POINTS CREATE start */
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
    SET newPoint = ST_LineInterpolatePoint(e.geom, fraction)
    FROM edge_table AS e WHERE edge_id = id;
/* --POINTS CREATE end */

/* --RESTRICTIONS OLD CREATE start */
CREATE TABLE old_restrictions (
    rid BIGINT NOT NULL,
    to_cost FLOAT,
    target_id BIGINT,
    via_path TEXT
);

INSERT INTO old_restrictions (rid, to_cost, target_id, via_path) VALUES
(1, 100,  7,  '4'),
(1, 100, 11,  '8'),
(1, 100, 10,  '7'),
(2,   4,  9,  '5, 3'),
(3, 100,  9, '16');
/* --RESTRICTIONS OLD CREATE end */

/* --RESTRICTIONS CREATE start */
CREATE TABLE restrictions (
    id SERIAL PRIMARY KEY,
    path BIGINT[],
    cost FLOAT
);

INSERT INTO restrictions (path, cost) VALUES
(ARRAY[4, 7], 100),
(ARRAY[8, 11], 100),
(ARRAY[7, 10], 100),
(ARRAY[3, 5, 9], 4),
(ARRAY[9, 16], 100);
/* --RESTRICTIONS CREATE end */


/* --VEHICLES TABLE START */

CREATE TABLE vehicles (
  id BIGSERIAL PRIMARY KEY,
  capacity FLOAT,
  start_node_id BIGINT,
  start_x FLOAT,
  start_y FLOAT,
  start_open FLOAT,
  start_close FLOAT
);

INSERT INTO vehicles
(start_node_id, start_x,  start_y,  start_open,  start_close, capacity) VALUES
(            11,       3,        2,           0,           50,       50),
(            11,       3,        2,           0,           50,       50);

/* --VEHICLES TABLE END */



/* --ORDERS TABLE START */
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
            10,    3,   1,      2,         10,          3,
            3,    1,   2,      6,         15,          3),
(20,
            16,    4,   2,      4,         15,          2,
            15,    4,   1,      6,         20,          3),
(30,
            7,    2,   2,      2,         10,          3,
           12,    3,   3,      3,         20,          3);


/* --ORDERS TABLE END */

/* --COMBINATIONS CREATE start */
CREATE TABLE combinations_table (
    source BIGINT,
    target BIGINT
);

INSERT INTO combinations_table (
    source, target) VALUES
(5, 6),
(5, 10),
(6, 5),
(6, 15),
(6, 14);

/* --COMBINATIONS CREATE end */
