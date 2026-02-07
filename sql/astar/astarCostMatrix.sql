/*PGR-GNU*****************************************************************
File: astarCostMatrix.sql

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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

-----------------------------
-- pgr_aStarCostMatrix
-----------------------------


--v3.0
CREATE FUNCTION pgr_aStarCostMatrix(
    TEXT,     -- edges sql (required)
    ANYARRAY, -- vids (required)

    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor FLOAT DEFAULT 1.0,
    epsilon FLOAT DEFAULT 1.0,

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.start_vid, a.end_vid, a.agg_cost
    FROM _pgr_astar(_pgr_get_statement($1), $2::BIGINT[], '{}'::BIGINT[], $3, $4, $5::FLOAT, $6::FLOAT, true) a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST ${COST_HIGH} ROWS ${ROWS_HIGH};

-- COMMENT

COMMENT ON FUNCTION pgr_aStarCostMatrix(TEXT, ANYARRAY, BOOLEAN, INTEGER, FLOAT, FLOAT)
IS 'pgr_aStarCostMatrix
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost], x1, y1, x2, y2
    - ARRAY [vertices identifiers]
- Optional Parameters:
    - directed := true
    - heuristic := 5
    - factor := 1
    - epsilon := 1
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_aStarCostMatrix.html
';
