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
* return meaningful names of columns(DOING)
* return types should be bigint
* return more columns (like dijkstra)
* directed should be optional (DEFAULT true)
* Function should be STRICT (aka no NULLS allowed)
*/
--v3.0
CREATE FUNCTION pgr_trsp(
    TEXT, -- edges SQL (required)
    TEXT, -- restrictions sql (required)
    BIGINT, -- from_vid (required)
    BIGINT, -- to_vid (required)
    BOOLEAN, -- directed (required)

    OUT seq INTEGER,
    OUT node INTEGER,
    OUT edge INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF record AS
$BODY$
DECLARE
    edges_sql ALIAS FOR $1;
    restrictions_sql ALIAS FOR $2;
    start_vid ALIAS FOR $3;
    end_vid   ALIAS FOR $4;
    directed  ALIAS FOR $5;

    restrictions_query TEXT;
    trsp_sql TEXT;

BEGIN
    /*
    TODO This should be handled by C code
    */
    IF (restrictions_sql IS NULL OR length(restrictions_sql) = 0) THEN
        -- no restrictions then its a dijkstra
        -- RAISE WARNING 'Executing pgr_dijkstra';
        RETURN query SELECT a.seq - 1 AS seq, a.node::INTEGER, a.edge::INTEGER, a.cost
        FROM pgr_dijkstra(edges_sql, start_vid, end_vid, directed) a;
        RETURN;
    END IF;


    /*
    TODO Should use the new restrictions
    */
    restrictions_query = $$
        WITH old_restrictions AS ( $$ ||
            restrictions_sql || $$
        )
        SELECT ROW_NUMBER() OVER() AS id,
            _pgr_array_reverse(array_prepend(target_id, string_to_array(via_path::text, ',')::INTEGER[])) AS path,
            to_cost AS cost
        FROM old_restrictions;
    $$;



    RETURN query
        SELECT (a.seq - 1)::INTEGER, a.node::INTEGER, a.edge::INTEGER, a.cost
        FROM _pgr_trsp(edges_sql, restrictions_query, start_vid, end_vid, directed) AS a;
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Error computing path: Path Not Found';
    END IF;

END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, BIGINT, BIGINT, BOOLEAN)
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
