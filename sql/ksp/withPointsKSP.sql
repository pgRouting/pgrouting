/*PGR-GNU*****************************************************************
File: withPointsKSP.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara AT erosion.dev

Copyright (c) 2023 Abhinav Jain
this.abhinav AT gmail.com

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
--v3.6
CREATE FUNCTION pgr_withPointsKSP(
    TEXT,    -- edges_sql (required)
    TEXT,    -- points_sql (required)
    BIGINT,  -- from_vid (required)
    BIGINT,  -- to_vid (required)
    INTEGER, -- K (required)
    CHAR,    -- driving_side (required)

    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,
    details BOOLEAN DEFAULT false,

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
    SELECT *
    FROM _pgr_v4withPointsKSP(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], ARRAY[$4]::BIGINT[], $5, $6, $7, $8, $9);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- ONE to MANY
--v3.6
CREATE FUNCTION pgr_withPointsKSP(
    TEXT,    -- edges_sql (required)
    TEXT,    -- points_sql (required)
    BIGINT,  -- from_vid (required)
    ANYARRAY,-- to_vids (required)
    INTEGER, -- K (required)
    CHAR,    -- driving_side (required)

    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,
    details BOOLEAN DEFAULT false,

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
    SELECT *
    FROM _pgr_v4withPointsKSP(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4::BIGINT[], $5, $6, $7, $8, $9);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to ONE
--v3.6
CREATE FUNCTION pgr_withPointsKSP(
    TEXT,    -- edges_sql (required)
    TEXT,    -- points_sql (required)
    ANYARRAY,-- from_vid (required)
    BIGINT,  -- to_vids (required)
    INTEGER, -- K (required)
    CHAR,    -- driving_side (required)

    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,
    details BOOLEAN DEFAULT false,

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
    SELECT *
    FROM _pgr_v4withPointsKSP(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], ARRAY[$4]::BIGINT[], $5, $6, $7, $8, $9);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to MANY
--v3.6
CREATE FUNCTION pgr_withPointsKSP(
    TEXT,    -- edges_sql (required)
    TEXT,    -- points_sql (required)
    ANYARRAY,-- from_vid (required)
    ANYARRAY,-- to_vids (required)
    INTEGER, -- K (required)
    CHAR,    -- driving_side (required)

    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,
    details BOOLEAN DEFAULT false,

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
    SELECT *
    FROM _pgr_v4withPointsKSP(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], $4::BIGINT[], $5, $6, $7, $8, $9);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- Combinations SQL signature
--v3.6
CREATE FUNCTION pgr_withPointsKSP(
    TEXT,    -- edges_sql (required)
    TEXT,    -- points_sql (required)
    TEXT,    -- combinations_sql(required)
    INTEGER, -- K (required)
    CHAR,    -- driving_side (required)

    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,
    details BOOLEAN DEFAULT false,

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
    SELECT *
    FROM _pgr_v4withPointsKSP(_pgr_get_statement($1), _pgr_get_statement($2), _pgr_get_statement($3), $4, $5, $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_withPointsKSP(TEXT, TEXT, BIGINT, BIGINT, INTEGER, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsKSP
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex identifier
    - To vertex identifier
    - K
    - driving side
- Optional Parameters
    - directed := true
    - heap paths := false
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsKSP.html';

COMMENT ON FUNCTION pgr_withPointsKSP(TEXT, TEXT, BIGINT, ANYARRAY, INTEGER, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsKSP
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex identifier
    - To ARRAY[vertices identifiers]
    - K
    - driving side
- Optional Parameters
    - directed := true
    - heap paths := false
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsKSP.html';

COMMENT ON FUNCTION pgr_withPointsKSP(TEXT, TEXT, ANYARRAY, BIGINT, INTEGER, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsKSP
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices identifier]
    - To vertex identifier
    - K
    - driving side
- Optional Parameters
    - directed := true
    - heap paths := false
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsKSP.html';

COMMENT ON FUNCTION pgr_withPointsKSP(TEXT, TEXT, ANYARRAY, ANYARRAY, INTEGER, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsKSP
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices identifier]
    - To ARRAY[vertices identifiers]
    - K
    - driving side
- Optional Parameters
    - directed := true
    - heap paths := false
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsKSP.html';

COMMENT ON FUNCTION pgr_withPointsKSP(TEXT, TEXT, TEXT, INTEGER, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsKSP
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - Combinations SQL with columns: source, target
    - K
    - driving side
- Optional Parameters
    - directed := true
    - heap paths := false
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsKSP.html';

--v2.6
CREATE FUNCTION pgr_withPointsKSP(
    TEXT,    -- edges_sql (required)
    TEXT,    -- points_sql (required)
    BIGINT,  -- from_vid (required)
    BIGINT,  -- to_vid (required)
    INTEGER, -- K (required)

    directed BOOLEAN DEFAULT true,
    heap_paths BOOLEAN DEFAULT false,
    driving_side CHAR DEFAULT 'b',
    details BOOLEAN DEFAULT false,

    OUT seq INTEGER, OUT path_id INTEGER, OUT path_seq INTEGER,
    OUT node BIGINT, OUT edge BIGINT,
    OUT cost FLOAT, OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RAISE WARNING 'pgr_withPointsKSP(text,text,bigint,bigint,integer,boolean,boolean,char,boolean) deprecated on v3.6.0';
    RETURN QUERY
    SELECT * FROM _pgr_withPointsKSP(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4, $5, $6, $7, $8, $9);
END
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_withPointsKSP(TEXT, TEXT, BIGINT, BIGINT, INTEGER, BOOLEAN, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_withPointsKSP
- Deprecated signature in v3.6.0
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex identifier
    - To vertex identifier
    - K
- Optional Parameters
    - directed := true
    - heap paths := false
    - driving side := b
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsKSP.html
';
