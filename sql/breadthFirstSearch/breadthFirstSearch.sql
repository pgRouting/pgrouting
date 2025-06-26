/*PGR-GNU*****************************************************************
File: breadthFirstSearch.sql

Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Gudesa Venkata Sai Akhil
Mail: gvs.akhil1997 at gmail.com

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

--ONE TO DEPTH
--v3.0
CREATE FUNCTION pgr_breadthFirstSearch(
    TEXT,   -- edges_sql (required)
    BIGINT, -- root_vid (required)

    directed BOOLEAN DEFAULT true,
    max_depth BIGINT DEFAULT 9223372036854775807,

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, depth, start_vid, pred, node, edge, cost, agg_cost
    FROM _pgr_breadthFirstSearch(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], max_depth, directed);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--MANY TO DEPTH
--v3.0
CREATE FUNCTION pgr_breadthFirstSearch(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- root_vids (required)

    directed BOOLEAN DEFAULT true,
    max_depth BIGINT DEFAULT 9223372036854775807,

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, depth, start_vid, pred, node, edge, cost, agg_cost
    FROM _pgr_breadthFirstSearch(_pgr_get_statement($1), $2::BIGINT[], max_depth, directed);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


COMMENT ON FUNCTION pgr_breadthFirstSearch(TEXT, BIGINT, BOOLEAN, BIGINT)
IS 'pgr_breadthFirstSearch(One to Depth)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From root vertex identifier
- Optional parameters
  - directed := true
  - max_depth := 9223372036854775807
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_breadthFirstSearch.html
';

COMMENT ON FUNCTION pgr_breadthFirstSearch(TEXT, ANYARRAY, BOOLEAN, BIGINT)
IS 'pgr_breadthFirstSearch(Many to Depth)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[root vertices identifiers]
- Optional parameters
   - directed := true
   - max_depth := 9223372036854775807
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_breadthFirstSearch.html
';
