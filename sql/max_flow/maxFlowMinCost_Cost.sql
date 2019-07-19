/*PGR-GNU*****************************************************************
File: pgr_maxFlowMinCost_Cost.sql

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

----------------------------
-- pgr_maxFlowMinCost_Cost
----------------------------

--    ONE TO ONE
CREATE OR REPLACE FUNCTION pgr_maxFlowMinCost_Cost(
    TEXT,   -- edges_sql (required)
    BIGINT,   -- source (required)
    BIGINT)   -- target (required)

RETURNS FLOAT AS
$BODY$
    SELECT cost
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := true);
$BODY$
LANGUAGE SQL VOLATILE;


--    ONE TO MANY
CREATE OR REPLACE FUNCTION pgr_maxFlowMinCost_Cost(
    TEXT,   -- edges_sql (required)
    BIGINT,   -- source (required)
    ANYARRAY) -- targets (required)

RETURNS FLOAT AS
$BODY$
    SELECT cost
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], only_cost := true);
$BODY$
LANGUAGE SQL VOLATILE;


--    MANY TO ONE
CREATE OR REPLACE FUNCTION pgr_maxFlowMinCost_Cost(
    TEXT,   -- edges_sql (required)
    ANYARRAY, -- sources (required)
    BIGINT)   -- target (required)

RETURNS FLOAT AS
$BODY$
    SELECT cost
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := true);
$BODY$
LANGUAGE SQL VOLATILE;


--    MANY TO MANY
CREATE OR REPLACE FUNCTION pgr_maxFlowMinCost_Cost(
    TEXT,   -- edges_sql (required)
    ANYARRAY, -- sources (required)
    ANYARRAY) -- targets (required)

RETURNS FLOAT AS
$BODY$
    SELECT cost
    FROM _pgr_maxFlowMinCost(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], only_cost := true);
$BODY$
LANGUAGE SQL VOLATILE;


-- COMMENTS

COMMENT ON FUNCTION pgr_maxFlowMinCost_Cost(TEXT, BIGINT, BIGINT)
IS 'pgr_maxFlowMinCost_Cost (One to One)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To vertex identifier
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_maxFlowMinCost_Cost.html
';

COMMENT ON FUNCTION pgr_maxFlowMinCost_Cost(TEXT, BIGINT, ANYARRAY)
IS 'pgr_maxFlowMinCost_Cost(One to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_maxFlowMinCost_Cost.html
  ';

COMMENT ON FUNCTION pgr_maxFlowMinCost_Cost(TEXT, ANYARRAY, BIGINT)
IS 'pgr_maxFlowMinCost_Cost (Many to One)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To vertex identifier
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_maxFlowMinCost_Cost.html
';

COMMENT ON FUNCTION pgr_maxFlowMinCost_Cost(TEXT, ANYARRAY, ANYARRAY)
IS 'EXPERIMENTAL pgr_maxFlowMinCost_Cost (Many to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_maxFlowMinCost_Cost.html
';
