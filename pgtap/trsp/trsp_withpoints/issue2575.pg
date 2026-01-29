/* :file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2024-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0 */

BEGIN;

SELECT CASE WHEN min_version('3.4.0') THEN plan(3) ELSE plan(1) END;


-- Create network table
CREATE TABLE railroad (
    id serial,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision,
    x1 double precision,
    y1 double precision,
    x2 double precision,
    y2 double precision,
    geom geometry
);

-- Create restrictions table
CREATE TABLE restrictions_railroad (
    id serial,
    cost FLOAT,
    path BIGINT[]
);

-- Populate network table
INSERT INTO railroad (x1,y1,x2,y2) VALUES
  (0,0,1,0),(1,0,4,0),(4,0,5,0),(5,0,5,5),(5,5,0,5),(0,5,0,0),
  (1,0,2,1),(2,1,3,1),(3,1,4,0)
;

UPDATE railroad SET geom = ST_makeline(ST_point(x1,y1),ST_point(x2,y2));
UPDATE railroad SET cost = ST_length(geom), reverse_cost = ST_length(geom);

SELECT * INTO rvertices
FROM pgr_extractVertices('SELECT id, geom FROM railroad ORDER BY id');

/* -- set the source information */
UPDATE railroad AS e
SET source = v.id
FROM rvertices AS v
WHERE ST_StartPoint(e.geom) = v.geom;
/* -- set the target information */
UPDATE railroad AS e
SET target = v.id
FROM rvertices AS v
WHERE ST_EndPoint(e.geom) = v.geom;

INSERT INTO restrictions_railroad (cost, path)
VALUES (100, ARRAY[9,2]),(100, ARRAY[2,9]),(100, ARRAY[7,2]),(100, ARRAY[2,7]);

CREATE OR REPLACE FUNCTION issue2575()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.4.0') THEN
  RETURN QUERY SELECT skip(1, 'pgr_trsp_withPoints: signatures on 3.4.0');
  RETURN;
END IF;

PREPARE dijkstra AS
SELECT seq, path_seq, node, edge, cost, agg_cost
FROM pgr_dijkstra('SELECT * FROM railroad', 7, 1);

RETURN QUERY
SELECT set_eq('dijkstra', $$VALUES
  (1, 1, 7, 3, 1, 0),
  (2, 2, 6, 2, 3, 1),
  (3, 3, 3, 1, 1, 4),
  (4, 4, 1,-1, 0, 5)
  $$, 'expected dijkstra');

PREPARE between2nodes AS
SELECT seq, path_seq, node, edge, cost, agg_cost
FROM pgr_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM railroad$$,
  $$SELECT * FROM (VALUES (1, 2, 0.75),(2, 8, 0.5)) AS t(pid, edge_id, fraction)$$,
  -1, -2);

RETURN QUERY
SELECT set_eq('between2nodes', $$VALUES
  (1, 1, -1, 2, 0.75,               0),
  (2, 2,  6, 9, 1.4142135623730951, 0.75),
  (3, 3,  5, 8, 0.5,                2.164213562373095),
  (4, 4, -2,-1, 0,                  2.664213562373095)
  $$, 'expected withPoints');

PREPARE between2nodeswithRestriction AS
SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM railroad$$,
  $$SELECT * FROM restrictions_railroad$$,
  $$SELECT * FROM (VALUES (1, 2, 0.75),(2, 8, 0.5)) AS t(pid, edge_id, fraction)$$,
  -1, -2);
RETURN QUERY
SELECT set_eq('between2nodeswithRestriction', $$VALUES
   (1, 1, -1, -2, -1, 2, 0.75,                0),
   (2, 2, -1, -2,  6, 3, 1,                   0.75),
   (3, 3, -1, -2,  7, 4, 5,                   1.75),
   (4, 4, -1, -2,  8, 5, 5,                   6.75),
   (5, 5, -1, -2,  2, 6, 5,                  11.75),
   (6, 6, -1, -2,  1, 1, 1,                  16.75),
   (7, 7, -1, -2,  3, 7, 1.4142135623730958, 17.75),
   (8, 8, -1, -2,  4, 8, 0.5,                19.164213562373096),
   (9, 9, -1, -2, -2,-1, 0,                  19.664213562373096)
  $$, 'expected withPoints');

END;
$BODY$
LANGUAGE plpgsql;

SELECT issue2575();
SELECT finish();
ROLLBACK;
