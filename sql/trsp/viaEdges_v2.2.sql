/*PGR-GNU*****************************************************************

Copyright (c) 2016 pgRouting developers
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

/* trspViaEdges

 - when contradiction (has rcost but does not want it):
   - the call is made to trsp only with cost column

 - When there are no restrictions:
   a call to _pgr_withPointsVia
 - Else
   a call to _pgr_trspViaEdges
     - which calls repetedly the original C/C++ code
*/


/*
------------------------------
-- pgr_trspViaEdges Wrapper
------------------------------
*/
CREATE OR REPLACE FUNCTION pgr_trspViaEdges(
    sql TEXT,
    eids INTEGER[],
    pcts float[],
    directed BOOLEAN,
    has_rcost BOOLEAN,
    turn_restrict_sql text DEFAULT NULL::TEXT)
    RETURNS SETOF pgr_costresult3 AS
$BODY$
DECLARE
    i INTEGER;
    rr pgr_costresult3;
    lrr pgr_costresult3;
    first BOOLEAN := true;
    seq INTEGER := 0;
    seq2 INTEGER :=0;
    has_reverse BOOLEAN;
    edges_sql TEXT;

BEGIN
    has_reverse =_pgr_parameter_check('dijkstra', sql, false);
    edges_sql := sql;
    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            edges_sql = 'SELECT id, source, target, cost FROM (' || sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    IF (turn_restrict_sql IS NULL OR length(turn_restrict_sql) = 0) THEN
        -- no restrictions then its a _pgr_withPointsVia
        RETURN query SELECT seq-1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost
        FROM _pgr_withPointsVia(edges_sql, eids, pcts, directed)
        RETURN;
    END IF;


    -- make the call to the original code
    RETURN query SELECT * FROM _pgr_trspViaEdges(edges_sql, eids, pcts, directed, has_rcost, turn_restrict_sql);

END;
$BODY$
LANGUAGE plpgsql VOLATILE
cost 100
rows 1000;
