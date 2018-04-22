/*PGR-GNU*****************************************************************

Copyright (c) 2015  ~ pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com

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


----------------------------------------------------------------------------
-- Routing function: pgr_dijkstra
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - from v2.0
--   - up to v2.1 (not including)
--   - moved to legacy on v3.0
--
-- Use the new signatures of pgr_dijkstra instead
----------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);
    sql = edges_sql;
    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    RETURN query SELECT seq-1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM _pgr_dijkstra(sql, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed, false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

