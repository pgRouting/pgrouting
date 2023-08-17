/*PGR-GNU*****************************************************************
File: withPointsDD.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project at pgrouting.org

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
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_v4withPointsDD(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4, $5, $6, $7, false);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

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
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_v4withPointsDD(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4, $5, $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, BIGINT, FLOAT, CHAR, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsDD(Single Vertex)
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex identifier
    - Distance
    - Driving_side
- Optional Parameters
    - directed := true
    - details := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsDD.html
';


COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, ANYARRAY, FLOAT, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsDD(Multiple Vertices)
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices identifiers]
    - Distance
    - Driving_side
- Optional Parameters
    - directed := true
    - details := false
    - equicost := false
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_withPointsDD.html
';

/* TODO remove on V4 */
-- SINGLE
--v2.6
CREATE FUNCTION pgr_withPointsDD(
    TEXT,   --edges_sql (required)
    TEXT,   -- points_sql (required)
    BIGINT, -- from_vid (required)
    FLOAT,  -- distance (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b',
    details BOOLEAN DEFAULT false,

    OUT seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RAISE WARNING 'pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean) is been deprecated';
    RETURN QUERY
    SELECT a.seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_withPointsDD(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4, $5, $6, $7, false) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;

-- MULTIPLE
--v2.6
CREATE FUNCTION pgr_withPointsDD(
    TEXT,     --edges_sql (required)
    TEXT,     -- points_sql (required)
    ANYARRAY, -- from_vid (required)
    FLOAT,    -- distance (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b',
    details BOOLEAN DEFAULT false,
    equicost BOOLEAN DEFAULT false,

    OUT seq INTEGER,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
  RAISE WARNING 'pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean) is been deprecated';
  RETURN QUERY
    SELECT *
    FROM _pgr_withPointsDD(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4, $5, $6, $7, $8);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, BIGINT, FLOAT, BOOLEAN, CHAR, BOOLEAN)
IS 'pgRouting deprecated function';


COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, ANYARRAY, FLOAT, BOOLEAN, CHAR, BOOLEAN, BOOLEAN)
IS 'pgRouting deprecated function';
