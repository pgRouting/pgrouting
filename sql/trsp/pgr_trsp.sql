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


/* TODO
* The restrictions sql should be the second parameter
* the vids should be BIGINT
* return meaningful names of columns
* return more columns (like dijkstra)
* directed should be optional (DEFAULT true)
* Function should be STRICT (aka no NULLS allowed)
*/
--v3.0
CREATE FUNCTION pgr_trsp(
    TEXT, -- edges SQL (required)
    INTEGER, -- from_vid (required)
    INTEGER, -- to_vid (required)
    BOOLEAN, -- directed (required)
    restrictions_sql TEXT DEFAULT NULL,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF record AS
$BODY$
DECLARE
    edges_sql TEXT    := $1;
    start_vid INTEGER := $2;
    end_vid INTEGER   := $3;
    directed BOOLEAN  := $4;

has_reverse BOOLEAN;
new_sql TEXT;
restrictions_query TEXT;
trsp_sql TEXT;
BEGIN
    /*
    TODO This should be handled by C code
    */
    IF (restrictions_sql IS NULL OR length(restrictions_sql) = 0) THEN
        -- no restrictions then its a dijkstra
        -- RAISE WARNING 'Executing pgr_dijkstra';
        RETURN query SELECT a.seq - 1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, a.cost
        FROM pgr_dijkstra($1, start_vid, end_vid, directed) a;
        RETURN;
    END IF;


    /*
    TODO Should use the new restrictions
    */
    restrictions_query = $$
        WITH old_restrictions AS ( $$ ||
            $5 || $$
        )
        SELECT ROW_NUMBER() OVER() AS id,
            _pgr_array_reverse(array_prepend(target_id, string_to_array(via_path::text, ',')::INTEGER[])) AS path,
            to_cost AS cost
        FROM old_restrictions;
    $$;



    RETURN query
        SELECT (a.seq - 1)::INTEGER, a.node::INTEGER, a.edge::INTEGER, a.cost
        FROM _pgr_trsp($1, restrictions_query, start_vid, end_vid, directed) AS a;
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Error computing path: Path Not Found';
    END IF;

END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_trsp(TEXT, INTEGER, INTEGER, BOOLEAN, TEXT)
IS 'pgr_trsp
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - from vertex identifier
    - to vertex identifier
    - directed
    - has reverse cost
- Optional parameters
    - restrictions_sql := NULL
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trsp.html
';
