/*PGR-GNU*****************************************************************
File: dagShortestPath.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Sourabh Garg
Mail: sourabh.garg.mat at gmail.com

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
--v4.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    BIGINT,   -- from_vid (required)

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
    SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dagShortestPath_v4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], false, true);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- ONE to MANY
--v4.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    ANYARRAY, -- to_vids (required)

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
    SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dagShortestPath_v4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], false, true);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- MANY to ONE
--v4.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT,   -- to_vid (required)

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
    SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dagShortestPath_v4(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], false, true);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- MANY to MANY
--v4.0
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)

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
    SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dagShortestPath_v4(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], false, true);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMBINATIONS
--v3.2
CREATE FUNCTION pgr_dagShortestPath(
    TEXT,     -- edges_sql (required)
    TEXT,     -- combinations_sql (required)

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
    SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dagShortestPath_v4(_pgr_get_statement($1), _pgr_get_statement($2), false);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


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
