-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_lineGraph(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges WHERE id IN (2,4,5,8)',
  false);
/* -- q2 */
CREATE TABLE edges_shared (
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT,
    geom geometry
);

INSERT INTO edges_shared (id, source, target, cost, reverse_cost, geom) VALUES
  (102, 1, 2, 1, -1, ST_MakeLine(ST_POINT(0,  2), ST_POINT(2,  2))),
  (104, 1, 4, 1, -1, ST_MakeLine(ST_POINT(0,  2), ST_POINT(0,  0))),
  (301, 3, 1, 1, -1, ST_MakeLine(ST_POINT(2,  0), ST_POINT(0,  2))),
  (203, 2, 3, 1,  1, ST_MakeLine(ST_POINT(2,  2), ST_POINT(2,  0))),
  (304, 3, 4, 1,  1, ST_MakeLine(ST_POINT(0,  0), ST_POINT(2,  0)));

SELECT seq, source, target, cost, reverse_cost
FROM pgr_lineGraph(
  'SELECT id, source, target, cost, reverse_cost FROM edges_shared',
  true);
/* -- q3 */
CREATE TABLE edges_unique (
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    geom geometry
);

INSERT INTO edges_unique (id, source, target, cost, geom) VALUES
  (102, 1, 2, 1, ST_MakeLine(ST_POINT(0,  2), ST_POINT(2,  2))),
  (104, 1, 4, 1, ST_MakeLine(ST_POINT(0,  2), ST_POINT(0,  0))),
  (301, 3, 1, 1, ST_MakeLine(ST_POINT(2,  0), ST_POINT(0,  2))),
  (203, 2, 3, 1, ST_MakeLine(ST_POINT(2,  2), ST_POINT(2,  0))),
  (304, 3, 4, 1, ST_MakeLine(ST_POINT(2,  0), ST_POINT(0,  0))),
  (302, 3, 2, 1, ST_MakeLine(ST_POINT(2,  0), ST_POINT(2,  2))),
  (403, 4, 3, 1, ST_MakeLine(ST_POINT(0,  0), ST_POINT(2,  0)));

SELECT seq, source, target, cost, reverse_cost
FROM pgr_lineGraph(
  'SELECT id, source, target, cost FROM edges_unique',
  true);
/* -- q4 */
