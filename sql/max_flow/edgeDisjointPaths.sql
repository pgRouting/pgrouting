/*PGR-GNU*****************************************************************
File: edgeDisjointPaths.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli at gmail.com

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
CREATE FUNCTION pgr_edgeDisjointPaths(
    TEXT,   --edges_sql (required)
    BIGINT, -- From_vid (required)
    BIGINT, -- to_vid (required)

    directed BOOLEAN DEFAULT true,

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
    FROM _pgr_edgeDisjointPaths(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- ONE to MANY
--v4.0
CREATE FUNCTION pgr_edgeDisjointPaths(
    TEXT,     --edges_sql (required)
    BIGINT,   -- From_vid (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,

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
    FROM _pgr_edgeDisjointPaths(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $4);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- MANY to ONE
--v4.0
CREATE FUNCTION pgr_edgeDisjointPaths(
    TEXT,     --edges_sql (required)
    ANYARRAY, -- From_vids (required)
    BIGINT,   -- to_vid (required)

    directed BOOLEAN DEFAULT true,

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
    SELECT seq, path_id, path_seq, end_vid, start_vid, node, edge, cost, agg_cost
    FROM _pgr_edgeDisjointPaths(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $4);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- MANY to MANY
--v3.0
CREATE FUNCTION pgr_edgeDisjointPaths(
    TEXT,     --edges_sql (required)
    ANYARRAY, -- From_vids (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,

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
    FROM _pgr_edgeDisjointPaths(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], $4);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMBINATIONS
--v3.2
CREATE FUNCTION pgr_edgeDisjointPaths(
    TEXT,     -- edges_sql (required)
    TEXT,     -- combinations_sql (required)

    directed BOOLEAN DEFAULT true,

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
    FROM _pgr_edgeDisjointPaths(_pgr_get_statement($1), _pgr_get_statement($2), $3)
$BODY$
LANGUAGE SQL VOLATILE STRICT;

COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_edgeDisjointPaths(One to One)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To vertex identifier
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_edgeDisjointPaths.html
';


COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, BIGINT, ANYARRAY, BOOLEAN)
IS 'pgr_edgeDisjointPaths(One to Many)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_edgeDisjointPaths.html
';


COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_edgeDisjointPaths(Many to One)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To vertex identifier
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_edgeDisjointPaths.html
';


COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgr_edgeDisjointPaths(Many to Many)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_edgeDisjointPaths.html
';

COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, TEXT, BOOLEAN)
IS 'pgr_edgeDisjointPaths(Combinations)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Combinations SQL with columns: source, target
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_edgeDisjointPaths.html
';
