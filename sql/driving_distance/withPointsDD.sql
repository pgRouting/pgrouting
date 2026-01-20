/*PGR-GNU*****************************************************************
File: withPointsDD.sql

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project at pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Yige Huang
Mail: square1ge at gmail.com

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

-- SINGLE
--v3.6
CREATE FUNCTION pgr_withPointsDD(
    TEXT,   --edges_sql (required)
    TEXT,   -- points_sql (required)
    BIGINT, -- from_vid (required)
    FLOAT,  -- distance (required)
    CHAR,   -- driving_side (required)

    directed BOOLEAN DEFAULT true,
    details BOOLEAN DEFAULT false,

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
    FROM _pgr_withPointsDDv4(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4,
      $5, directed, details, false);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST ${COST_HIGH} ROWS ${ROWS_HIGH};

-- MULTIPLE
--v3.6
CREATE FUNCTION pgr_withPointsDD(
    TEXT,     --edges_sql (required)
    TEXT,     -- points_sql (required)
    ANYARRAY, -- from_vid (required)
    FLOAT,    -- distance (required)
    CHAR,     -- driving_side (required)

    directed BOOLEAN DEFAULT true,
    details BOOLEAN DEFAULT false,
    equicost BOOLEAN DEFAULT false,

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
    FROM _pgr_withPointsDDv4(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4,
      $5, directed, details, equicost);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST ${COST_HIGH} ROWS ${ROWS_HIGH};


COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, BIGINT, FLOAT, CHAR, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsDD(Single Vertex)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex identifier
    - Distance
    - Driving side
- Optional Parameters
    - directed := true
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsDD.html
';


COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, ANYARRAY, FLOAT, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsDD(Multiple Vertices)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices identifiers]
    - Distance
    - Driving side
- Optional Parameters
    - directed := true
    - details := false
    - equicost := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsDD.html
';


-- SINGLE
--v4.0
CREATE FUNCTION pgr_withPointsDD(
    TEXT,   --edges_sql (required)
    TEXT,   -- points_sql (required)
    BIGINT, -- from_vid (required)
    FLOAT,  -- distance (required)

    directed BOOLEAN DEFAULT true,
    details BOOLEAN DEFAULT false,

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
    FROM _pgr_withPointsDDv4(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4,
      (CASE WHEN directed THEN 'r' ELSE 'b' END), directed, details, false);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST ${COST_HIGH} ROWS ${ROWS_HIGH};

-- MULTIPLE
--v4.0
CREATE FUNCTION pgr_withPointsDD(
    TEXT,     --edges_sql (required)
    TEXT,     -- points_sql (required)
    ANYARRAY, -- from_vid (required)
    FLOAT,    -- distance (required)

    directed BOOLEAN DEFAULT true,
    details BOOLEAN DEFAULT false,
    equicost BOOLEAN DEFAULT false,

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
    FROM _pgr_withPointsDDv4(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4,
      (CASE WHEN directed THEN 'r' ELSE 'b' END), directed, details, equicost);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST ${COST_HIGH} ROWS ${ROWS_HIGH};


COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, BIGINT, FLOAT, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsDD(Single Vertex)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex identifier
    - Distance
- Optional Parameters
    - directed := true
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsDD.html
';


COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, ANYARRAY, FLOAT, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsDD(Multiple Vertices)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices identifiers]
    - Distance
- Optional Parameters
    - directed := true
    - details := false
    - equicost := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsDD.html
';
