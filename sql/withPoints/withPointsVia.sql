/*PGR-GNU*****************************************************************
File: withPointsVia.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

--v4.0
CREATE FUNCTION pgr_withPointsVia(
  TEXT,     -- edges SQL
  TEXT,     -- points SQL
  ANYARRAY, -- via vids
  CHAR,     -- driving side

  directed BOOLEAN DEFAULT true,

  -- via parameters
  strict BOOLEAN DEFAULT false,
  U_turn_on_edge BOOLEAN DEFAULT true,

  -- withPoints parameters
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
  FROM _pgr_withPointsVia_v4(
    _pgr_get_statement($1), _pgr_get_statement($2), $3,
    directed, strict, u_turn_on_edge, $4, details);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

COMMENT ON FUNCTION pgr_withPointsVia(TEXT, TEXT, ANYARRAY, CHAR, BOOLEAN, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsVia
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - ARRAY[via vertices identifiers]
  - Driving side: directed graph [r,l], undirected graph [b]
- Optional Parameters
  - directed => true
  - strict => false
  - U_turn_on_edge => true
  - details => false
- Documentation:
  -{PROJECT_DOC_LINK}/pgr_withPointsVia.html
';
