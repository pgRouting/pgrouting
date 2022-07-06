------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              SAMPLE DATA
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

DROP TABLE IF EXISTS edges;
DROP TABLE IF EXISTS vertices;
DROP table if exists pointsOfInterest;
DROP TABLE IF EXISTS new_restrictions;
DROP TABLE IF EXISTS restrictions;
DROP TABLE IF EXISTS combinations;
DROP TABLE IF EXISTS vehicles;
DROP TABLE IF EXISTS orders;


/* --EDGE TABLE CREATE start */
CREATE TABLE edges (
    id BIGSERIAL PRIMARY KEY,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT,
    capacity BIGINT,
    reverse_capacity BIGINT,
    x1 FLOAT,
    y1 FLOAT,
    x2 FLOAT,
    y2 FLOAT,
    geom geometry
);
/* --EDGE TABLE CREATE end */
/* --EDGE TABLE ADD DATA start */
INSERT INTO edges (
    cost, reverse_cost,
    capacity, reverse_capacity, geom) VALUES
( 1,  1,  80, 130,   ST_MakeLine(ST_POINT(2, 0), ST_POINT(2, 1))),
(-1,  1,  -1, 100,   ST_MakeLine(ST_POINT(2, 1), ST_POINT(3, 1))),
(-1,  1,  -1, 130,   ST_MakeLine(ST_POINT(3, 1), ST_POINT(4, 1))),
( 1,  1, 100,  50,   ST_MakeLine(ST_POINT(2, 1), ST_POINT(2, 2))),
( 1, -1, 130,  -1,   ST_MakeLine(ST_POINT(3, 1), ST_POINT(3, 2))),
( 1,  1,  50, 100,   ST_MakeLine(ST_POINT(0, 2), ST_POINT(1, 2))),
( 1,  1,  50, 130,   ST_MakeLine(ST_POINT(1, 2), ST_POINT(2, 2))),
( 1,  1, 100, 130,   ST_MakeLine(ST_POINT(2, 2), ST_POINT(3, 2))),
( 1,  1, 130,  80,   ST_MakeLine(ST_POINT(3, 2), ST_POINT(4, 2))),
( 1,  1, 130,  50,   ST_MakeLine(ST_POINT(2, 2), ST_POINT(2, 3))),
( 1, -1, 130,  -1,   ST_MakeLine(ST_POINT(3, 2), ST_POINT(3, 3))),
( 1, -1, 100,  -1,   ST_MakeLine(ST_POINT(2, 3), ST_POINT(3, 3))),
( 1, -1, 100,  -1,   ST_MakeLine(ST_POINT(3, 3), ST_POINT(4, 3))),
( 1,  1,  80, 130,   ST_MakeLine(ST_POINT(2, 3), ST_POINT(2, 4))),
( 1,  1,  80,  50,   ST_MakeLine(ST_POINT(4, 2), ST_POINT(4, 3))),
( 1,  1,  80,  80,   ST_MakeLine(ST_POINT(4, 1), ST_POINT(4, 2))),
( 1,  1, 130, 100,   ST_MakeLine(ST_POINT(0.5, 3.5), ST_POINT(1.999999999999, 3.5))),
( 1,  1,  50, 130,   ST_MakeLine(ST_POINT(3.5, 2.3), ST_POINT(3.5, 4)));
/* --EDGE TABLE ADD DATA end */

/* -- q1 */
SELECT * INTO vertices
FROM pgr_extractVertices('SELECT id, geom FROM edges ORDER BY id');
/* -- q1-1 */
CREATE SEQUENCE vertices_id_seq;
ALTER TABLE vertices ALTER COLUMN id SET DEFAULT nextval('vertices_id_seq');
ALTER SEQUENCE vertices_id_seq OWNED BY vertices.id;
SELECT setval('vertices_id_seq', (SELECT coalesce(max(id)) FROM vertices));
/* -- q1-2 */
\d vertices
/* -- q2 */
SELECT * FROM vertices;
/* -- q3 */
/* -- set the source information */
UPDATE edges AS e
SET source = v.id, x1 = x, y1 = y
FROM vertices AS v
WHERE ST_StartPoint(e.geom) = v.geom;

/* -- set the target information */
UPDATE edges AS e
SET target = v.id, x2 = x, y2 = y
FROM vertices AS v
WHERE ST_EndPoint(e.geom) = v.geom;
/* -- q4 */
SELECT id, source, target
FROM edges ORDER BY id;
/* -- q5 */

/* --POINTS CREATE start */

--POINTS CREATE end

/* -- p1 */
CREATE TABLE pointsOfInterest(
    pid BIGSERIAL,
    x FLOAT,
    y FLOAT,
    edge_id BIGINT,
    side CHAR,
    fraction FLOAT,
    geom geometry,
    newPoint geometry
);
/* -- p2 */
INSERT INTO pointsOfInterest (x, y, edge_id, side, fraction) VALUES
(1.8, 0.4,   1, 'l', 0.4),
(4.2, 2.4,  15, 'r', 0.4),
(2.6, 3.2,  12, 'l', 0.6),
(0.3, 1.8,   6, 'r', 0.3),
(2.9, 1.8,   5, 'l', 0.8),
(2.2, 1.7,   4, 'b', 0.7);
/* -- p3 */
UPDATE pointsOfInterest SET geom = st_makePoint(x,y);

UPDATE pointsOfInterest
    SET newPoint = ST_LineInterpolatePoint(e.geom, fraction)
    FROM edges AS e WHERE edge_id = id;
/* -- p4 */
SELECT pid, edge_id, side, fraction,
       ST_AsText(geom), ST_AsText(newPoint)
FROM pointsOfInterest
ORDER BY pid;
/* -- p5 */
/* --POINTS CREATE end */

/* --COMBINATIONS CREATE start */
/* -- c1 */
CREATE TABLE combinations (
    source BIGINT,
    target BIGINT
);
/* -- c2 */
INSERT INTO combinations (
    source, target) VALUES
(5, 6),
(5, 10),
(6, 5),
(6, 15),
(6, 14);
/* -- c3 */
SELECT * FROM combinations;
/* -- c4 */
/* --COMBINATIONS CREATE end */

/* --RESTRICTIONS CREATE start */
/* -- r1 */
CREATE TABLE restrictions (
    rid BIGINT NOT NULL,
    to_cost FLOAT,
    target_id BIGINT,
    from_edge BIGINT,
    via_path TEXT
);
/* -- r2 */

INSERT INTO restrictions (rid, to_cost, target_id, from_edge, via_path) VALUES
(1, 100,  7,  4, NULL),
(1, 100, 11,  8, NULL),
(1, 100, 10,  7, NULL),
(2,   4,  8,  3, 5),
(3, 100,  9, 16, NULL);
/* -- r3 */

CREATE TABLE new_restrictions (
    id SERIAL PRIMARY KEY,
    path BIGINT[],
    cost float
);

INSERT INTO new_restrictions (path, cost) VALUES
(ARRAY[4, 7], 100),
(ARRAY[8, 11], 100),
(ARRAY[7, 10], 100),
(ARRAY[3, 5, 9], 4),
(ARRAY[9, 16], 100);
/* -- r4 */
--RESTRICTIONS CREATE end


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


