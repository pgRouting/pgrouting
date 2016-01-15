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
http://hansolav.net/sql/graphs.html
*/

SET client_min_messages TO WARNING;

DROP TABLE IF EXISTS table1_vertices;
CREATE TABLE table1_vertices (
    vid SERIAL PRIMARY KEY,
    name TEXT
);

DROP TABLE IF EXISTS table1;
CREATE TABLE table1 (
    id SERIAL,
    source INTEGER REFERENCES table1_vertices(vid),
    target INTEGER REFERENCES table1_vertices(vid),
    cost FLOAT
);

INSERT INTO table1_vertices (name) VALUES ('Seatle');
INSERT INTO table1_vertices (name) VALUES ('San Francisco');
INSERT INTO table1_vertices (name) VALUES ('Las Vegas');
INSERT INTO table1_vertices (name) VALUES ('Los Angeles');
INSERT INTO table1_vertices (name) VALUES ('Denver');
INSERT INTO table1_vertices (name) VALUES ('Minneapolis');
INSERT INTO table1_vertices (name) VALUES ('Dallas');
INSERT INTO table1_vertices (name) VALUES ('Chicago');
INSERT INTO table1_vertices (name) VALUES ('Washington D.C.');
INSERT INTO table1_vertices (name) VALUES ('Boston');
INSERT INTO table1_vertices (name) VALUES ('Nueva York');
INSERT INTO table1_vertices (name) VALUES ('Miami');

INSERT INTO table1 (source, target, cost) VALUES ( 1,  2, 1306);
INSERT INTO table1 (source, target, cost) VALUES ( 1,  5, 2161);
INSERT INTO table1 (source, target, cost) VALUES ( 1,  6, 2661);
INSERT INTO table1 (source, target, cost) VALUES ( 2,  3,  919);
INSERT INTO table1 (source, target, cost) VALUES ( 2,  4,  629);
INSERT INTO table1 (source, target, cost) VALUES ( 3,  4,  435);
INSERT INTO table1 (source, target, cost) VALUES ( 3,  5, 1225);
INSERT INTO table1 (source, target, cost) VALUES ( 3,  7, 1983);
INSERT INTO table1 (source, target, cost) VALUES ( 5,  6, 1483);
INSERT INTO table1 (source, target, cost) VALUES ( 5,  7, 1258);
INSERT INTO table1 (source, target, cost) VALUES ( 6,  7, 1532);
INSERT INTO table1 (source, target, cost) VALUES ( 6,  8,  661);
INSERT INTO table1 (source, target, cost) VALUES ( 7,  9, 2113);
INSERT INTO table1 (source, target, cost) VALUES ( 7, 12, 2161);
INSERT INTO table1 (source, target, cost) VALUES ( 8,  9, 1145);
INSERT INTO table1 (source, target, cost) VALUES ( 8, 10, 1613);
INSERT INTO table1 (source, target, cost) VALUES ( 9, 10,  725);
INSERT INTO table1 (source, target, cost) VALUES ( 9, 11,  383);
INSERT INTO table1 (source, target, cost) VALUES ( 9, 12, 1709);
INSERT INTO table1 (source, target, cost) VALUES (10, 11,  338);
INSERT INTO table1 (source, target, cost) VALUES (11, 12, 2145);

SET client_min_messages TO NOTICE;

-- Their output starts with 0 so we substract 1 to the vid
-- pgrouting: no paths or 0 length (aka I am there, so no path) are not included
SELECT  name, cost, agg_cost  FROM pgr_dijkstra(
    'SELECT id, source, target, cost FROM table1',
    1,  6, true)
    JOIN table1_vertices ON (node = vid) ORDER BY seq;

SELECT  name, cost, agg_cost  FROM pgr_dijkstra(
    'SELECT id, source, target, cost FROM table1',
    1,  11, true)
    JOIN table1_vertices ON (node = vid) ORDER BY seq;

select end_vid, array_to_string( array_agg(name ORDER BY seq),','), array_agg(node ORDER BY seq) 
FROM pgr_dijkstra(
    'SELECT id, source, target, cost FROM table1',
    1,  ARRAY[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12], true)
JOIN table1_vertices ON (node = vid) GROUP BY end_vid; 
