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
-- all the groups of queries are equivialent (give the same results)

SET client_min_messages TO warning;

\set vertices_cant 17

--\echo directed, only cost
DROP TABLE IF EXISTS query1;

SELECT * INTO TEMP query1
    FROM pgr_johnson(
        'SELECT id, source, target, cost FROM edge_table ORDER BY id',
        TRUE
);

SELECT count(*) = 69 FROM query1 ;
SELECT count(*) = :vertices_cant  FROM query1 WHERE agg_cost = 0;
SELECT count(*) = :vertices_cant  FROM query1 WHERE start_vid = end_vid;


--\echo directed, both costs

DROP TABLE IF EXISTS query2;


SELECT * INTO TEMP query2
    FROM pgr_apspJohnson(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
);

SELECT count(*) = 69 FROM query2 ;
SELECT count(*) = :vertices_cant  FROM query2 WHERE cost = 0;
SELECT count(*) = :vertices_cant  FROM query2 WHERE id1 = id2;

SELECT *  INTO TEMP query3
    FROM pgr_apspJohnson(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
);

SELECT count(*) = 69 FROM query3;
SELECT count(*) = :vertices_cant  FROM query3 WHERE cost = 0;
SELECT count(*) = :vertices_cant  FROM query3 WHERE id1 = id2;



-- errors:

-- no flags
SELECT * FROM pgr_apspJohnson(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    TRUE
);
-- no flags
SELECT * FROM pgr_apspJohnson(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    FALSE, TRUE
);
