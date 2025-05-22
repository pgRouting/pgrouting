/*PGR-GNU*****************************************************************
File: ksp.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2023 Aniket Agarwal
aniketgarg187 at gmail.com

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


-- one-to-one
--v3.6
CREATE FUNCTION pgr_ksp(
    TEXT,
    BIGINT,
    BIGINT,
    INTEGER,
    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,

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
    FROM _pgr_ksp_v4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4, $5, $6);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- one-to-many
--v3.6
CREATE FUNCTION pgr_ksp(
    TEXT,
    BIGINT,
    ANYARRAY,
    INTEGER,
    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,

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
    FROM  _pgr_ksp_v4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $4, $5, $6);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- many-to-one
--v3.6
CREATE FUNCTION pgr_ksp(
    TEXT,
    ANYARRAY,
    BIGINT,
    INTEGER,
    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,

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
    FROM  _pgr_ksp_v4(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $4, $5, $6);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- many-to-many
--v3.6
CREATE FUNCTION pgr_ksp(
    TEXT,
    ANYARRAY,
    ANYARRAY,
    INTEGER,
    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,

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
    FROM  _pgr_ksp_v4(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], $4, $5, $6);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- combinations
--v3.6
CREATE FUNCTION pgr_ksp(
    TEXT,
    TEXT,
    INTEGER,
    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,

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
    FROM  _pgr_ksp_v4(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4, $5);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_ksp(TEXT, BIGINT, BIGINT, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgr_KSP
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From vertex identifier
    - To vertex identifier
    - K
- Optional Parameters
    - directed := true
    - heap_paths := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_KSP.html
';

COMMENT ON FUNCTION pgr_ksp(TEXT, BIGINT, ANYARRAY, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgr_KSP(One to Many)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From vertex identifier
    - To ARRAY[vertex identifier]
    - K
- Optional Parameters
    - directed := true
    - heap_paths := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_KSP.html
';

COMMENT ON FUNCTION pgr_ksp(TEXT, ANYARRAY, BIGINT, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgr_KSP(Many to One)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[vertex identifier]
    - To vertex identifier
    - K
- Optional Parameters
    - directed := true
    - heap_paths := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_KSP.html
';

COMMENT ON FUNCTION pgr_ksp(TEXT, ANYARRAY, ANYARRAY, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgr_KSP(Many to Many)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[vertex identifier]
    - To ARRAY[vertex identifier]
    - K
- Optional Parameters
    - directed := true
    - heap_paths := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_KSP.html
';

COMMENT ON FUNCTION pgr_ksp(TEXT, TEXT, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgr_KSP(Combinations)
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Combinations SQL with columns: source, target
    - K
- Optional Parameters
    - directed := true
    - heap_paths := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_KSP.html
';
