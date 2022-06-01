------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              SAMPLE DATA
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

DROP TABLE IF EXISTS edges;
DROP TABLE IF EXISTS vertices;
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
CREATE TABLE edges (
    id BIGSERIAL,
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
INSERT INTO edges (
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

/* -- q1 */
SELECT  * INTO vertices
FROM pgr_extractVertices('SELECT id, geom FROM edges ORDER BY id');
/* -- q2 */
SELECT * FROM vertices;
/* -- q3 */
/* -- set the source information */
WITH
out_going AS (
  SELECT id AS vid, unnest(out_edges) AS eid, x, y
  FROM vertices
)
UPDATE edges
SET source = vid, x1 = x, y1 = y
FROM out_going WHERE id = eid;

/* -- set the target information */
WITH
in_coming AS (
  SELECT id AS vid, unnest(in_edges) AS eid, x, y
  FROM vertices
)
UPDATE edges
SET target = vid, x2 = x, y2 = y
FROM in_coming WHERE id = eid;
/* -- q4 */
SELECT id, source, target
FROM edges ORDER BY id;
/* -- q5 */

/* --POINTS CREATE start */

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
/* -- p4 */
UPDATE pointsOfInterest
    SET newPoint = ST_LineInterpolatePoint(e.geom, fraction)
    FROM edges AS e WHERE edge_id = id;
/* -- p5 */
SELECT pid, x, y, edge_id, side, fraction
FROM pointsOfInterest;
/* -- p6 */
/* --POINTS CREATE end */

/* --COMBINATIONS CREATE start */
/* -- c1 */
CREATE TABLE combinations_table (
    source BIGINT,
    target BIGINT
);
/* -- c2 */
INSERT INTO combinations_table (
    source, target) VALUES
(5, 6),
(5, 10),
(6, 5),
(6, 15),
(6, 14);
/* -- c3 */
SELECT * FROM combinations_table;
/* -- c4 */
/* --COMBINATIONS CREATE end */

/* --RESTRICTIONS CREATE start */
/* -- r1 */
CREATE TABLE restrictions (
    id SERIAL PRIMARY KEY,
    path BIGINT[],
    cost FLOAT
);
/* -- r2 */
INSERT INTO restrictions (path, cost) VALUES
(ARRAY[4, 7], 100),
(ARRAY[8, 11], 100),
(ARRAY[7, 10], 100),
(ARRAY[3, 5, 9], 4),
(ARRAY[9, 16], 100);
/* -- r3 */
SELECT * FROM restrictions;
/* -- r4 */
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

