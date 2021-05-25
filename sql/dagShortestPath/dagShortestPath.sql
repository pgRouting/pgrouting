/*PGR-GNU*****************************************************************
File: dagShortestPath.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Sourabh Garg
Mail: sourabh.garg.mat@gmail.com

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
-- pgr_dagShortestPath
----------------------

-- ONE to ONE
--v3.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    BIGINT,   -- from_vid (required)

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- ONE to MANY
--v3.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    ANYARRAY, -- to_vids (required)

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- MANY to ONE
--v3.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT,   -- to_vid (required)

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- MANY to MANY
--v3.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMBINATIONS
--v3.2
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    TEXT,     -- combinations_sql (required)

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), _pgr_get_statement($2), true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_dagShortestPath(TEXT, BIGINT, BIGINT)
IS 'pgr_dagShortestPath(One to One)
- EXPERIMENTAL
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From vertex identifier
    - To vertex identifier
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_dagShortestPath.html
';

COMMENT ON FUNCTION pgr_dagShortestPath(TEXT, BIGINT, ANYARRAY)
IS 'pgr_dagShortestPath(One to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_dagShortestPath.html
';

COMMENT ON FUNCTION pgr_dagShortestPath(TEXT, ANYARRAY, BIGINT)
IS 'pgr_dagShortestPath(Many to One)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To vertex identifier
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_dagShortestPath.html
';

COMMENT ON FUNCTION pgr_dagShortestPath(TEXT, ANYARRAY, ANYARRAY)
IS 'pgr_dagShortestPath(Many to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_dagShortestPath.html
';

COMMENT ON FUNCTION pgr_dagShortestPath(TEXT, TEXT)
IS 'pgr_dagShortestPath(Combinations)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Combinations SQL with columns: source, target
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_dagShortestPath.html
';
