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

--v4.0
CREATE FUNCTION pgr_withPointsCost(
  TEXT,   -- edges
  TEXT,   -- points
  BIGINT, -- starts
  BIGINT, -- ends
  CHAR,   -- driving side

  directed BOOLEAN DEFAULT true,

  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT start_vid, end_vid, agg_cost
FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], ARRAY[$4]::BIGINT[],
  directed, $5, true, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


--v4.0
CREATE FUNCTION pgr_withPointsCost(
  TEXT,     -- edges
  TEXT,     -- points
  BIGINT,   -- starts
  ANYARRAY, -- ends
  CHAR,    -- driving side

  directed BOOLEAN DEFAULT true,

  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT start_vid, end_vid, agg_cost
FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4::BIGINT[],
  directed, $5, true, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


--v4.0
CREATE FUNCTION pgr_withPointsCost(
  TEXT,     -- edges
  TEXT,     -- points
  ANYARRAY, -- starts
  BIGINT,   -- ends
  CHAR,     -- driving side

  directed BOOLEAN DEFAULT true,

  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT start_vid, end_vid, agg_cost
FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], ARRAY[$4]::BIGINT[],
  directed, $5, true, true, false, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


--v4.0
CREATE FUNCTION pgr_withPointsCost(
  TEXT,     -- edges
  TEXT,     -- points
  ANYARRAY, -- starts
  ANYARRAY, -- ends
  CHAR,     -- driving side

  directed BOOLEAN DEFAULT true,

  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
SELECT start_vid, end_vid, agg_cost
FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], $4::BIGINT[],
  directed, $5, true, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


--v4.0
CREATE FUNCTION pgr_withPointsCost(
  TEXT, -- edges
  TEXT, -- points
  TEXT, -- combinations
  CHAR, -- driving side

  directed BOOLEAN DEFAULT true,

  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT start_vid, end_vid, agg_cost
FROM _pgr_withPoints_v4(_pgr_get_statement($1), _pgr_get_statement($2), _pgr_get_statement($3),
  directed, $4, true, true, 0, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, BIGINT, BIGINT, CHAR, BOOLEAN)
IS 'pgr_withPointsCost (One to One)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - From vertices/point identifier
  - To vertex/point identifier
  - Driving side: directed graph [r,l], undirected graph [b]
- Optional Parameters
  - directed => true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPointsCost.html
';

COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, BIGINT, ANYARRAY, CHAR, BOOLEAN)
IS 'pgr_withPointsCost (One to Many)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - From vertices/point identifier
  - To ARRAY[vertices/points identifiers]
  - Driving side: directed graph [r,l], undirected graph [b]
- Optional Parameters
  - directed => true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPointsCost.html
';

COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, ANYARRAY, BIGINT, CHAR, BOOLEAN)
IS 'pgr_withPointsCost (Many to One)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - From ARRAY[vertices/points identifiers]
  - To vertex/point identifier
  - Driving side: directed graph [r,l], undirected graph [b]
- Optional Parameters
  - directed => true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPointsCost.html
';


COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, ANYARRAY, ANYARRAY, CHAR, BOOLEAN)
IS 'pgr_withPointsCost (Many to Many)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - From ARRAY[vertices/points identifiers]
  - To ARRAY[vertices/points identifiers]
  - driving side: directed graph [r,l], undirected graph [b]
- Optional Parameters:
  - directed => true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPointsCost.html
';

COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, TEXT, CHAR, BOOLEAN)
IS 'pgr_withPointsCost(Combinations)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - Combinations SQL with columns: source, target
  - driving side: directed graph [r,l], undirected graph [b]
- Optional Parameters:
  - directed => true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPointsCost.html
';
