/*PGR-GNU*****************************************************************
File: turnRestrictedPath.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

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

-- ONE to ONE
--v4.0
CREATE FUNCTION pgr_turnRestrictedPath(
  TEXT,   -- edges_sql (required)
  TEXT,   -- restrictions_sql (required)
  BIGINT, -- start_vertex (required)
  BIGINT, -- end_vertex (required)
  INTEGER,-- K cycles (required)

  directed BOOLEAN DEFAULT true,
  heap_paths BOOLEAN DEFAULT false,
  stop_on_first BOOLEAN DEFAULT true,
  strict BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
  SELECT seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_turnRestrictedPath_v4(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    ARRAY[$3]::BIGINT[], ARRAY[$4]::BIGINT[],
    $5, directed, heap_paths, stop_on_first, strict);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

COMMENT ON FUNCTION pgr_turnRestrictedPath(TEXT, TEXT, BIGINT, BIGINT, INTEGER, BOOLEAN, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_turnRestrictedPath
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: id, cost, path
  - From vertex identifier
  - To vertex identifier
  - K
- Optional Parameters
  - directed := true
  - heap_paths := false
  - stop_on_first := true
  - strict := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_turnRestrictedPath.html
';
