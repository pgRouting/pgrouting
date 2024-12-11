
/*PGR-GNU*****************************************************************

Copyright (c) 2018  pgRouting developers
Mail: project@pgrouting.org

------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ********************************************************************PGR-GNU*/
BEGIN;

SELECT plan(3);

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
    the_geom geometry
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

UPDATE railroad SET the_geom = ST_makeline(ST_point(x1,y1),ST_point(x2,y2));
UPDATE railroad SET cost = ST_length(the_geom), reverse_cost = ST_length(the_geom);
SELECT pgr_createTopology('railroad',0.001);

INSERT INTO restrictions_railroad (cost, path)
VALUES (100, ARRAY[9,2]),(100, ARRAY[2,9]),(100, ARRAY[7,2]),(100, ARRAY[2,7]);

PREPARE dijkstra AS
SELECT seq, path_seq, node, edge, cost, agg_cost
FROM pgr_dijkstra('SELECT * FROM railroad', 4, 1);

SELECT set_eq('dijkstra', $$VALUES
  (1, 1, 4, 3, 1, 0),
  (2, 2, 3, 2, 3, 1),
  (3, 3, 2, 1, 1, 4),
  (4, 4, 1,-1, 0, 5)
  $$, 'expected dijkstra');

PREPARE between2nodes AS
SELECT seq, path_seq, node, edge, cost, agg_cost
FROM pgr_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM railroad$$,
  $$SELECT * FROM (VALUES (1, 2, 0.75),(2, 8, 0.5)) AS t(pid, edge_id, fraction)$$,
  -1, -2);

SELECT set_eq('between2nodes', $$VALUES
  (1, 1, -1, 2, 0.75,               0),
  (2, 2,  3, 9, 1.4142135623730951, 0.75),
  (3, 3,  8, 8, 0.5,                2.164213562373095),
  (4, 4, -2,-1, 0,                  2.664213562373095)
  $$, 'expected withPoints');

PREPARE between2nodeswithRestriction AS
SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM railroad$$,
  $$SELECT * FROM restrictions_railroad$$,
  $$SELECT * FROM (VALUES (1, 2, 0.75),(2, 8, 0.5)) AS t(pid, edge_id, fraction)$$,
  -1, -2);
SELECT set_eq('between2nodeswithRestriction', $$VALUES
   (1, 1, -1, -2, -1, 2, 0.75,                0),
   (2, 2, -1, -2,  3, 3, 1,                   0.75),
   (3, 3, -1, -2,  4, 4, 5,                   1.75),
   (4, 4, -1, -2,  5, 5, 5,                   6.75),
   (5, 5, -1, -2,  6, 6, 5,                  11.75),
   (6, 6, -1, -2,  1, 1, 1,                  16.75),
   (7, 7, -1, -2,  2, 7, 1.4142135623730958, 17.75),
   (8, 8, -1, -2,  7, 8, 0.5,                19.164213562373096),
   (9, 9, -1, -2, -2,-1, 0,                  19.664213562373096)
  $$, 'expected withPoints');

SELECT finish();
ROLLBACK;


