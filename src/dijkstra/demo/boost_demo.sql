/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
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
/*
DEMO from:
 http://www.boost.org/doc/libs/1_59_0/libs/graph/example/dijkstra-example.cpp
 const int num_nodes = 5;
  enum nodes { A, B, C, D, E }; = 1
  char name[] = "ABCDE";
  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
    Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
  };
  int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
*/

DROP TABLE IF EXISTS table1;
CREATE TABLE table1 (
    id SERIAL,
    source INTEGER,
    target INTEGER,
    source_name TEXT,
    target_name TEXT,
    cost FLOAT
);
DROP TABLE IF EXISTS table1_vertices;
CREATE TABLE table1_vertices (
    vid SERIAL,
    name TEXT
);

INSERT INTO table1_vertices (name) VALUES ('A');
INSERT INTO table1_vertices (name) VALUES ('B');
INSERT INTO table1_vertices (name) VALUES ('C');
INSERT INTO table1_vertices (name) VALUES ('D');
INSERT INTO table1_vertices (name) VALUES ('E');

INSERT INTO table1 (source_name, target_name, cost) VALUES ('A', 'C', 1);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('B', 'B', 2);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('B', 'D', 1);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('B', 'E', 2);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('C', 'B', 7);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('C', 'D', 3);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('D', 'E', 1);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('E', 'A', 1);
INSERT INTO table1 (source_name, target_name, cost) VALUES ('E', 'B', 1);

UPDATE table1 
  SET source = table1_vertices.vid
FROM table1_vertices
WHERE source_name = name;

UPDATE table1 SET target = table1_vertices.vid
FROM table1_vertices
WHERE target_name = name;

-- Their output starts with 0 so we subtract 1 to the vid
-- pgrouting: no paths or 0 length (aka I am there, so no path) are not included
SELECT end_vid-1, agg_cost FROM pgr_dijkstra(
    'SELECT id, source, target, cost FROM table1',
    1, ARRAY[1, 2, 3, 4, 5], true) where edge < 0 order by end_vid;

