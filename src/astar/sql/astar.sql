/*PGR-GNU*****************************************************************

-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
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


-----------------------------------------------------------------------
-- Core function for shortest_path_astar computation
-- Simillar to shortest_path in usage but uses the A* algorithm
-- instead of Dijkstra's.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_astar(sql text, source_id integer, target_id integer, directed boolean, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'shortest_path_astar'
    LANGUAGE c IMMUTABLE STRICT; 

/*    
-- V2 signature
CREATE OR REPLACE FUNCTION pgr_astar(edges_sql TEXT, source_id INTEGER, target_id INTEGER, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated signature of function pgr_astar';
    has_reverse =_pgr_parameter_check('astar', edges_sql, false);
    sql = edges_sql;
    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            sql = 'SELECT id, source, target, cost, x1,y1, x2, y2 FROM (' || edges_sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    RETURN query SELECT * 
    FROM _pgr_astar(sql, source_id, target_id, directed, has_rcost);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
*/
