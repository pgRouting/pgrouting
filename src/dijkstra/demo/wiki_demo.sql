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
https://en.wikipedia.org/wiki/Dijkstra's_algorithm
*/

DROP TABLE IF EXISTS table1;
CREATE TABLE table1 (
    id SERIAL,
    source INTEGER,
    target INTEGER,
    cost FLOAT
);

INSERT INTO table1 (source, target, cost) VALUES (1, 2, 7);
INSERT INTO table1 (source, target, cost) VALUES (1, 3, 9);
INSERT INTO table1 (source, target, cost) VALUES (1, 6, 14);
INSERT INTO table1 (source, target, cost) VALUES (2, 3, 10);
INSERT INTO table1 (source, target, cost) VALUES (2, 4, 15);
INSERT INTO table1 (source, target, cost) VALUES (3, 6, 2);
INSERT INTO table1 (source, target, cost) VALUES (3, 4, 11);
INSERT INTO table1 (source, target, cost) VALUES (4, 5, 6);
INSERT INTO table1 (source, target, cost) VALUES (5, 6, 9);

SELECT * FROM pgr_dijkstra(
    'SELECT id, source, target, cost FROM table1',
    1, 5, false);

