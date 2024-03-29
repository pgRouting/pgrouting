/*PGR-GNU*****************************************************************
File: trspVia_withPoints.sql

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo

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

--v3.4
CREATE FUNCTION pgr_trspVia_withPoints(
  TEXT,     -- edges sql
  TEXT,     -- restrictions sql
  TEXT,     -- points
  ANYARRAY, -- via vids

  directed BOOLEAN DEFAULT true,

  -- via parameters
  strict BOOLEAN DEFAULT false,
  U_turn_on_edge BOOLEAN DEFAULT true,

  -- withPoints parameters
  driving_side CHAR DEFAULT 'r', -- 'r'/'l'
  details BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT,
  OUT route_agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost, route_agg_cost
FROM _pgr_trspVia_withPoints(
  _pgr_get_statement($1),
  _pgr_get_statement($2),
  _pgr_get_statement($3),
  $4, $5, $6, $7, $8, $9);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_trspVia_withPoints(TEXT, TEXT, TEXT, ANYARRAY, BOOLEAN, BOOLEAN, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_trspVia_withPoints
- PROPOSED
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: cost, path
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - ARRAY[via vertices identifiers]
- Optional Parameters
  - directed := true
  - strict := false
  - U_turn_on_edge := true
  - driving_side := ''r''
  - details := ''false''
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trspVia_withPoints.html
';
