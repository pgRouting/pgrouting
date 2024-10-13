/*PGR-GNU*****************************************************************
File: trspViaVertices_deprecated.sql

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

/* pgr_trspVia Vertices
 - if size of restrictions_sql  is Zero or no restrictions_sql are given
     then call to pgr_dijkstra is made

 - because it reads the data wrong, when there is a reverse_cost column:
   - put all data costs in one cost column and
   - a call is made to original code in _pgr_trspViaVertices without only the positive values
*/
--v3.0
CREATE FUNCTION pgr_trspViaVertices(
    TEXT, -- edges SQL (required)
    ANYARRAY,  -- via vids (required)
    BOOLEAN, -- directed (required)
    BOOLEAN, -- has_rcost (required)

    restrictions_sql TEXT DEFAULT NULL,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT id3 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF RECORD AS

$BODY$
DECLARE
    edges_sql TEXT     := $1;
    via_vids INTEGER[] := $2;
    directed BOOLEAN   := $3;
    has_rcost BOOLEAN  := $4;

has_reverse BOOLEAN;
new_sql TEXT;
BEGIN
  RAISE WARNING 'pgr_trspViaVertices(text,anyarray,boolean,boolean,text) deprecated function on v3.4.0';

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
            (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, a.cost
            FROM pgr_dijkstraVia(new_sql, via_vids, directed, strict:=true) AS a WHERE edge != -1;
        RETURN;
    END IF;


    -- make the call without contradiction from part of the user
    RETURN query SELECT a.seq, a.id1, a.id2, a.id3, a.cost FROM _pgr_trspViaVertices(new_sql, via_vids::INTEGER[], directed, has_rcost, restrictions_sql) AS a;
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Error computing path: Path Not Found';
    END IF;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_trspViaVertices(TEXT, ANYARRAY, BOOLEAN, BOOLEAN, TEXT)
IS 'pgr_trspViaVertices deprecated function on v3.4.0
- Documentation: ${PROJECT_DOC_LINK}/pgr_trspVia.html';
