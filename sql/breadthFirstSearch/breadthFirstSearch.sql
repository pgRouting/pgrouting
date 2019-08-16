/*PGR-GNU*****************************************************************
File: breadthFirstSearch.sql

Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Gudesa Venkata Sai Akhil
Mail: gvs.akhil1997@gmail.com

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

------------------
-- pgr_breadthFirstSearch
------------------


--ONE TO DEPTH
CREATE OR REPLACE FUNCTION pgr_breadthFirstSearch(
    TEXT,   -- edges_sql (required)
    BIGINT, -- from_vid (required)

    max_depth BIGINT DEFAULT 9223372036854775807,
    directed BOOLEAN DEFAULT true,

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
    IF $3 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''max_depth'''
        USING HINT = format('Value found: %s', $3);
    END IF;


    RETURN QUERY
    SELECT *
    FROM _pgr_breadthFirstSearch(_pgr_get_statement($1),  ARRAY[$2]::BIGINT[], max_depth, directed) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


--MANY TO DEPTH
CREATE OR REPLACE FUNCTION pgr_breadthFirstSearch(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)

    max_depth BIGINT DEFAULT 9223372036854775807,
    directed BOOLEAN DEFAULT true,

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
    IF $3 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''max_depth'''
        USING HINT = format('Value found: %s', $3);
    END IF;


    RETURN QUERY
    SELECT *
    FROM _pgr_breadthFirstSearch(_pgr_get_statement($1), $2::BIGINT[], max_depth, directed) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS

COMMENT ON FUNCTION pgr_breadthFirstSearch(TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_breadthFirstSearch(One to Depth)
- EXPERIMENTAL
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
- Optional Parameters: 
  - Maximum Depth := 9223372036854775807
  - directed := true
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_breadthFirstSearch.html
';

COMMENT ON FUNCTION pgr_breadthFirstSearch(TEXT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_breadthFirstSearch(Many to Depth)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
- Optional Parameters
  - Maximum Depth := 9223372036854775807
  - directed := true
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_breadthFirstSearch.html
';