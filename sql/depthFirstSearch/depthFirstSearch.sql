/*PGR-GNU*****************************************************************
File: depthFirstSearch.sql

Generated with Template by:
Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Ashish Kumar
Mail: ashishkr23438@gmail.com

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

--------------------------
-- pgr_depthFirstSearch
--------------------------


-- SINGLE VERTEX
CREATE OR REPLACE FUNCTION pgr_depthFirstSearch(
    TEXT,   -- edges_sql (required)
    BIGINT, -- root_vid (required)

    directed BOOLEAN DEFAULT true,
    max_depth BIGINT DEFAULT 9223372036854775807,

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    IF $4 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''max_depth'''
        USING HINT = format('Value found: %s', $4);
    END IF;


    RETURN QUERY
    SELECT *
    FROM _pgr_depthFirstSearch(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], directed, max_depth);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- MULTIPLE VERTICES
CREATE OR REPLACE FUNCTION pgr_depthFirstSearch(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- root_vids (required)

    directed BOOLEAN DEFAULT true,
    max_depth BIGINT DEFAULT 9223372036854775807,

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    IF $4 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''max_depth'''
        USING HINT = format('Value found: %s', $4);
    END IF;


    RETURN QUERY
    SELECT *
    FROM _pgr_depthFirstSearch(_pgr_get_statement($1), $2, directed, max_depth);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_depthFirstSearch(TEXT, BIGINT, BOOLEAN, BIGINT)
IS 'pgr_depthFirstSearch(Single Vertex)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From root vertex identifier
- Optional parameters
    - directed := true
    - max_depth := 9223372036854775807
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_depthFirstSearch.html
';

COMMENT ON FUNCTION pgr_depthFirstSearch(TEXT, ANYARRAY, BOOLEAN, BIGINT)
IS 'pgr_depthFirstSearch(Multiple Vertices)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[root vertices identifiers]
- Optional parameters
    - directed := true
    - max_depth := 9223372036854775807
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_depthFirstSearch.html
';
