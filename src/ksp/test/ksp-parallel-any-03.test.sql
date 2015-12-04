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
---
--   TESTS FROM ISSUE 285
---


SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost 
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 1, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 2, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 3, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 4, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 100, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 1, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 2, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 3, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 100, true
);

