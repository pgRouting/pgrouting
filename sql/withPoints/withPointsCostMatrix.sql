/*PGR-GNU*****************************************************************
File: withPointsCost.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
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
CREATE FUNCTION pgr_withPointsCostMatrix(
  TEXT,     -- edges
  TEXT,     -- points
  ANYARRAY, -- vids
  CHAR,     -- driving side

  directed BOOLEAN DEFAULT true,

  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
  SELECT start_vid, end_vid, agg_cost
  FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], '{}'::BIGINT[],
    directed, $4, true, true, true, 0, true);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

COMMENT ON FUNCTION pgr_withPointsCostMatrix(TEXT, TEXT, ANYARRAY, CHAR, BOOLEAN)
IS'pgr_withPointsCostMatrix
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - ARRAY [vertex/points identifiers],
  - driving side: directed graph [r,l], undirected graph [b]
- Optional Parameters
  - directed => true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPointsCostMatrix.html
';

/* TODO remove in v5.0 */
---------------------------
-- pgr_withPointsCostMatrix
---------------------------

--v2.6
CREATE FUNCTION pgr_withPointsCostMatrix(
    TEXT,     -- edges_sql (required)
    TEXT,     -- points_sql (required)
    ANYARRAY, -- pids (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.start_pid, a.end_pid, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), _pgr_get_statement($2), $3, $3, $4,  $5, TRUE, TRUE) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_withPointsCostMatrix(TEXT, TEXT, ANYARRAY, BOOLEAN, CHAR)
IS 'pgr_withPointsCostMatrix deprecated signature on v4.0.0';
