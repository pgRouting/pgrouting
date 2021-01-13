/*PGR-GNU*****************************************************************
File: maxFlowMinCost.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

------------------------
-- pgr_maxFlowMinCost
------------------------

--    ONE TO ONE
--v3.0
CREATE FUNCTION pgr_maxFlowMinCost(
    TEXT,   -- edges_sql (required)
    BIGINT,   -- source (required)
    BIGINT,   -- target (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--    ONE TO MANY
--v3.0
CREATE FUNCTION pgr_maxFlowMinCost(
    TEXT,   -- edges_sql (required)
    BIGINT, -- sources (required)
    ANYARRAY,   -- target (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--    MANY TO ONE
--v3.0
CREATE FUNCTION pgr_maxFlowMinCost(
    TEXT,   -- edges_sql (required)
    ANYARRAY,   -- source (required)
    BIGINT, -- targets (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

--    MANY TO MANY
--v3.0
CREATE FUNCTION pgr_maxFlowMinCost(
    TEXT,   -- edges_sql (required)
    ANYARRAY, -- sources (required)
    ANYARRAY, -- targets (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMBINATIONS
--v3.0
CREATE FUNCTION pgr_maxFlowMinCost(
    TEXT,   -- edges_sql (required)
    TEXT,   -- combinations_sql (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), _pgr_get_statement($2), only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMMENTS

COMMENT ON FUNCTION pgr_maxFlowMinCost(TEXT, BIGINT, BIGINT)
IS 'pgr_maxFlowMinCost(One to One)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To vertex identifier
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_maxFlowMinCost.html
';

COMMENT ON FUNCTION pgr_maxFlowMinCost(TEXT, BIGINT, ANYARRAY)
IS 'pgr_maxFlowMinCost(One to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_maxFlowMinCost.html
';

COMMENT ON FUNCTION pgr_maxFlowMinCost(TEXT, ANYARRAY, BIGINT)
IS 'pgr_maxFlowMinCost(Many to One)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]vertex identifier
  - To vertex identifier
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_maxFlowMinCost.html
';

COMMENT ON FUNCTION pgr_maxFlowMinCost(TEXT, ANYARRAY, ANYARRAY)
IS 'EXPERIMENTAL pgr_maxFlowMinCost(Many to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_maxFlowMinCost.html
';

COMMENT ON FUNCTION pgr_maxFlowMinCost(TEXT, TEXT)
IS 'EXPERIMENTAL pgr_maxFlowMinCost(Combinations)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Combinations SQL with columns: source, target
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_maxFlowMinCost.html
';
