/*PGR-GNU*****************************************************************
File: withPointsDD.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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
CREATE OR REPLACE FUNCTION pgr_withPointsDD(
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
    SELECT seq, node, edge, cost, agg_cost
    FROM _pgr_withPointsDD(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4, $5, $6, $7, false);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- MULTIPLE
CREATE OR REPLACE FUNCTION pgr_withPointsDD(
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
    SELECT *
    FROM _pgr_withPointsDD(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4, $5, $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, BIGINT, FLOAT, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_withPointsDD(Single Vertex)
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex identifier
    - Distance
- Optional Parameters
    - directed := true
    - driving_side := b
    - details := false
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_withPointsDD.html
';


COMMENT ON FUNCTION pgr_withPointsDD(TEXT, TEXT, ANYARRAY, FLOAT, BOOLEAN, CHAR, BOOLEAN, BOOLEAN)
IS 'pgr_withPointsDD(Multiple Vertices)
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices identifiers]
    - Distance
- Optional Parameters
    - directed := true
    - driving_side := b
    - details := false
    - equicost := false
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_withPointsDD.html
';
