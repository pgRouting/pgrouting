/*PGR-GNU*****************************************************************
File: trsp_withPoints.sql

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo
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


-- ONE TO ONE
--v3.4
CREATE FUNCTION pgr_trsp_withPoints(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  TEXT, -- Points SQL
  BIGINT, -- start_vid
  BIGINT, -- end_vid

  directed BOOLEAN DEFAULT true,
  driving_side CHAR DEFAULT 'r', -- 'r'/'l'
  details BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT *
FROM _pgr_trsp_withPoints(
  _pgr_get_statement($1),
  _pgr_get_statement($2),
  _pgr_get_statement($3),
  ARRAY[$4]::BIGINT[], ARRAY[$5]::BIGINT[], $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- ONE TO MANY
--v3.4
CREATE FUNCTION pgr_trsp_withPoints(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  TEXT, -- Points SQL
  BIGINT, -- start_vid
  ANYARRAY, -- end_vids

  directed BOOLEAN DEFAULT true,
  driving_side CHAR DEFAULT 'r', -- 'r'/'l'
  details BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT *
FROM _pgr_trsp_withPoints(
  _pgr_get_statement($1),
  _pgr_get_statement($2),
  _pgr_get_statement($3),
  ARRAY[$4]::BIGINT[], $5::BIGINT[], $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY TO ONE
--v3.4
CREATE FUNCTION pgr_trsp_withPoints(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  TEXT, -- Points SQL
  ANYARRAY, -- start_vids
  BIGINT, -- end_vid

  directed BOOLEAN DEFAULT true,
  driving_side CHAR DEFAULT 'r', -- 'r'/'l'
  details BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT *
FROM _pgr_trsp_withPoints(
  _pgr_get_statement($1),
  _pgr_get_statement($2),
  _pgr_get_statement($3),
  $4::BIGINT[], ARRAY[$5]::BIGINT[], $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY TO MANY
--v3.4
CREATE FUNCTION pgr_trsp_withPoints(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  TEXT, -- Points SQL
  ANYARRAY, -- start_vids
  ANYARRAY, -- end_vids

  directed BOOLEAN DEFAULT true,
  driving_side CHAR DEFAULT 'r', -- 'r'/'l'
  details BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT *
FROM _pgr_trsp_withPoints(
  _pgr_get_statement($1),
  _pgr_get_statement($2),
  _pgr_get_statement($3),
  $4::BIGINT[], $5::BIGINT[], $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- Combinations
--v3.4
CREATE FUNCTION pgr_trsp_withPoints(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  TEXT, -- Points SQL
  TEXT, -- combinations SQL

  directed BOOLEAN DEFAULT true,
  driving_side CHAR DEFAULT 'r', -- 'r'/'l'
  details BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT *
FROM _pgr_trsp_withPoints(
  _pgr_get_statement($1),
  _pgr_get_statement($2),
  _pgr_get_statement($3),
  _pgr_get_statement($4),
  $5, $6, $7);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS
COMMENT ON FUNCTION pgr_trsp_withPoints(TEXT, TEXT, TEXT, BIGINT, BIGINT, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_trsp_withPoints (One to One)
- PROPOSED
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: id, cost, path
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - Departure vertex/point identifier
  - Destination vertex/point identifier
- Optional Parameters:
  - directed := ''true''
  - driving_side := ''r''
  - details := ''false''
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp_withPoints.html
';


COMMENT ON FUNCTION pgr_trsp_withPoints(TEXT, TEXT, TEXT, BIGINT, ANYARRAY, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_trsp_withPoints(One to Many)
- PROPOSED
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: id, cost, path
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - Departure vertex/point identifier
  - Destinations ARRAY[vertices/Points identifier]
- Optional Parameters:
  - directed := ''true''
  - driving_side := ''r''
  - details := ''false''
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp_withPoints.html
';

COMMENT ON FUNCTION pgr_trsp_withPoints(TEXT, TEXT, TEXT, ANYARRAY, BIGINT, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_trsp_withPoints(Many to One)
- PROPOSED
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: id, cost, path
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - Departures ARRAY[vertices/Points identifier]
  - Destination vertex/point identifier
- Optional Parameters:
  - directed := ''true''
  - driving_side := ''r''
  - details := ''false''
- Documentation:
- ${PROJECT_DOC_LINK}/pgr_trsp_withPoints.html
';

COMMENT ON FUNCTION pgr_trsp_withPoints(TEXT, TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_trsp_withPoints(Many to Many)
- PROPOSED
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: id, cost, path
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - Departures ARRAY[vertices/Points identifier]
  - Destinations ARRAY[vertices/Points identifier]
- Optional Parameters:
  - directed := ''true''
  - driving_side := ''r''
  - details := ''false''
- Documentation:
- ${PROJECT_DOC_LINK}/pgr_trsp_withPoints.html
';

COMMENT ON FUNCTION pgr_trsp_withPoints(TEXT, TEXT, TEXT, TEXT, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_trsp_withPoints(Combinations)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: id, path, cost
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - Combinations SQL with columns: source, target
- Optional Parameters:
  - directed := ''true''
  - driving_side := ''r''
  - details := ''false''
- Documentation:
- ${PROJECT_DOC_LINK}/pgr_trsp_withPoints.html
';
