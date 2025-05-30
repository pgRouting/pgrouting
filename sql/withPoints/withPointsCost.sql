/*PGR-GNU*****************************************************************
File: withPointsCost.sql

Generated with Template by:
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

-- ONE TO ONE
--v3.0
CREATE FUNCTION pgr_withPointsCost(
    TEXT,   -- edges_sql (required)
    TEXT,   -- points_sql (required)
    BIGINT, -- start_vid (required)
    BIGINT, -- end_vid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], ARRAY[$4]::BIGINT[],
        $5, $6, true, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- ONE TO MANY
--v3.0
CREATE FUNCTION pgr_withPointsCost(
    TEXT,     -- edges_sql (required)
    TEXT,     -- points_sql (required)
    BIGINT,   -- start_vid (required)
    ANYARRAY, -- end_vid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4::BIGINT[],
      $5, $6, true, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY TO ONE
--v3.0
CREATE FUNCTION pgr_withPointsCost(
    TEXT,     -- edges_sql (required)
    TEXT,     -- points_sql (required)
    ANYARRAY, -- start_vid (required)
    BIGINT,   -- end_vid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], ARRAY[$4]::BIGINT[],
      $5, $6, true, true, false, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY TO MANY
--v3.0
CREATE FUNCTION pgr_withPointsCost(
    TEXT,     -- edges_sql (required)
    TEXT,     -- points_sql (required)
    ANYARRAY, -- start_vid (required)
    ANYARRAY, -- end_vid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], $4::BIGINT[],
      $5, $6, true, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- Combinations
--v3.2
CREATE FUNCTION pgr_withPointsCost(
    TEXT, -- edges_sql (required)
    TEXT, -- points_sql (required)
    TEXT, -- combinations_sql (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), _pgr_get_statement($3),
      directed, driving_side, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;




COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, BIGINT, BIGINT, BOOLEAN, CHAR)
IS 'pgr_withPointsCost (One to One)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - Points SQL with columns: [pid], edge_id, fraction[,side]
   - From vertex identifier/point identifier
   - To vertex identifier/point identifier
- Optional Parameters
   - directed := ''true''
   - driving_side := ''b''
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_withPoints.html
';

COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, BIGINT, ANYARRAY, BOOLEAN, CHAR)
IS 'pgr_withPointsCost (One to Many)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - Points SQL with columns: [pid], edge_id, fraction[,side]
   - From vertex identifier/point identifier
   - To ARRAY[vertices/points identifier]
- Optional Parameters
   - directed := ''true''
   - driving_side := ''b''
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_withPoints.html
';

COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, ANYARRAY, BIGINT, BOOLEAN, CHAR)
IS 'pgr_withPointsCost (Many to One)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - Points SQL with columns: [pid], edge_id, fraction[,side]
   - From  ARRAY[vertices/points identifiers]
   - To vertex identifier/point identifier
- Optional Parameters
   - directed := ''true''
   - driving_side := ''b''
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_withPoints.html
';


COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN, CHAR)
IS 'pgr_withPointsCost (Many to Many)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices/points identifiers]
    - To ARRAY[vertices/points identifiers]
- Optional Parameters
    - directed := ''true''
    - driving_side := ''b''
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPoints.html
';

COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, TEXT, BOOLEAN, CHAR)
IS 'pgr_withPointsCost(Combinations)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - Points SQL with columns: [pid], edge_id, fraction [,side]
   - Combinations SQL with columns: source, target
- Optional Parameters
    - directed := ''true''
    - driving_side := ''b''
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_withPoints.html
';
