/*PGR-GNU*****************************************************************
File: bellman_ford.sql

Copyright (c) 2015 pgRouting developers
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

------------------
-- pgr_bellmanFord
------------------


--ONE TO ONE
--v3.0
CREATE FUNCTION pgr_bellmanFord(
    TEXT,   -- edges_sql (required)
    BIGINT, -- from_vid (required)
    BIGINT, -- to_vid (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_bellmanFord(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed, false) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--ONE TO MANY
--v3.0
CREATE FUNCTION pgr_bellmanFord(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,


    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_bellmanFord(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], directed, false) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--MANY TO ONE
--v3.0
CREATE FUNCTION pgr_bellmanFord(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT,   -- to_vid (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.start_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_bellmanFord(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], directed, false) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--MANY TO MANY
--v3.0
CREATE FUNCTION pgr_bellmanFord(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,

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
    SELECT a.seq, a.path_seq, a.start_vid, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_bellmanFord(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], directed, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT;


--COMBINATIONS
--v3.2
CREATE FUNCTION pgr_bellmanFord(
    TEXT,     -- edges_sql (required)
    TEXT,     -- combinations_sql (required)

    directed BOOLEAN DEFAULT true,

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
    SELECT a.seq, a.path_seq, a.start_vid, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_bellmanFord(_pgr_get_statement($1), _pgr_get_statement($2), directed, false ) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_bellmanFord(TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_bellmanFord(One to One)
- EXPERIMENTAL
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To vertex identifier
- Optional Parameters:
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_bellmanFord.html
';


COMMENT ON FUNCTION pgr_bellmanFord(TEXT, BIGINT, ANYARRAY, BOOLEAN)
IS 'pgr_bellmanFord(One to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_bellmanFord.html
';


COMMENT ON FUNCTION pgr_bellmanFord(TEXT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_bellmanFord(Many to One)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To vertex identifier
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_bellmanFord.html
';


COMMENT ON FUNCTION pgr_bellmanFord(TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgr_bellmanFord(Many to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_bellmanFord.html
';


COMMENT ON FUNCTION pgr_bellmanFord(TEXT, TEXT, BOOLEAN)
IS 'pgr_bellmanFord(Combinations)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Combinations SQL with columns: source, target
- Optional Parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_bellmanFord.html
';
