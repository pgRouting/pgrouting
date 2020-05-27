/*PGR-GNU*****************************************************************
File: withPointsCost.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
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


----------------------
-- pgr_withPointsCost
----------------------


-- ONE TO ONE
CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    TEXT, -- edges_sql (required)
    TEXT, -- points_sql (required)
    BIGINT, -- end_pid (required)
    BIGINT, -- end_pid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT $3, $4, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, ARRAY[$3]::BIGINT[], ARRAY[$4]::BIGINT[], $5, $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- ONE TO MANY
CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    TEXT, -- edges_sql (required)
    TEXT, -- points_sql (required)
    BIGINT, -- end_pid (required)
    ANYARRAY, -- end_pid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT $3, a.end_pid, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, ARRAY[$3]::BIGINT[], $4::BIGINT[], $5, $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- MANY TO ONE
CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    TEXT, -- edges_sql (required)
    TEXT, -- points_sql (required)
    ANYARRAY, -- end_pid (required)
    BIGINT, -- end_pid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.start_pid, $4, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, $3::BIGINT[], ARRAY[$4]::BIGINT[], $5, $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- MANY TO MANY
CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    TEXT, -- edges_sql (required)
    TEXT, -- points_sql (required)
    ANYARRAY, -- end_pid (required)
    ANYARRAY, -- end_pid (required)

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.start_pid, a.end_pid, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, $3::BIGINT[], $4::BIGINT[], $5,  $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS


COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, BIGINT, BIGINT, BOOLEAN, CHAR)
IS 'pgr_withPointsCost (One to One)
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex/point identifier
    - To vertex/point identifier
- Optional Parameters
    - directed := ''true''
    - driving_side := ''b''
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_withPointsCost.html
';


COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, BIGINT, ANYARRAY, BOOLEAN, CHAR)
IS 'pgr_withPointsCost (One to Many)
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From vertex/point identifier
    - To ARRAY[vertices/points identifiers]
- Optional Parameters
    - directed := ''true''
    - driving_side := ''b''
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_withPointsCost.html
';


COMMENT ON FUNCTION pgr_withPointsCost(TEXT, TEXT, ANYARRAY, BIGINT, BOOLEAN, CHAR)
IS 'pgr_withPointsCost (Many to One)
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - Points SQL with columns: [pid], edge_id, fraction[,side]
    - From ARRAY[vertices/points identifiers]
    - To vertex/point identifier
- Optional Parameters
    - directed := ''true''
    - driving_side := ''b''
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_withPointsCost.html
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
  - ${PGROUTING_DOC_LINK}/pgr_withPointsCost.html
';
