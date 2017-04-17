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

/* trspViaVertices

 - when contradiction (has rcost but does not want it):
   - the call is made to trsp only with cost column

 - When there are no restrictions:
   a call to pgr_dijkstraVia
 - Else
   a call to _pgr_trspViaVertices
     - which calls repetedly the original C/C++ code

------------------------------
-- pgr_trspViaVertices Wrapper
------------------------------
*/
CREATE OR REPLACE FUNCTION pgr_trspViaVertices(
    edges_sql TEXT,
    via_vids ANYARRAY,
    directed BOOLEAN,
    has_rcost BOOLEAN,
    restrictions_sql TEXT DEFAULT NULL)
RETURNS SETOF pgr_costResult3 AS
$BODY$
DECLARE
has_reverse BOOLEAN;
new_sql TEXT;
BEGIN

    has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);

    new_sql := edges_sql;
    IF (has_reverse != has_rcost) THEN  -- user contradiction
        IF (has_reverse) THEN  -- it has reverse_cost but user don't want it.
            new_sql :=
               'WITH old_sql AS (' || edges_sql || ')' ||
                '   SELECT id, source, target, cost FROM old_sql';
        ELSE -- it does not have reverse_cost but user wants it
            RAISE EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE := 'XX000';
        END IF;
    END IF;

    IF (restrictions_sql IS NULL OR length(restrictions_sql) = 0) THEN
        RETURN query SELECT (row_number() over())::INTEGER, path_id:: INTEGER, node::INTEGER,
            (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost
            FROM pgr_dijkstraVia(new_sql, via_vids, directed, strict:=true) WHERE edge != -1;
        RETURN;
    END IF;


    -- make the call without contradiction from part of the user
    RETURN query SELECT * FROM _pgr_trspViaVertices(new_sql, via_vids::INTEGER[], directed, has_rcost, restrictions_sql);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


