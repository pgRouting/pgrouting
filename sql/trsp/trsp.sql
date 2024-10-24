/*PGR-GNU*****************************************************************
File: trsp.sql

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2022 Vicky Vergara
- Complete rewrite of one to one
- New: one to many, many to one, many to many, combinations

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
--v3.4
CREATE FUNCTION pgr_trsp(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  BIGINT, -- departure
  BIGINT, -- destination
  directed BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT
)
RETURNS SETOF record AS
$BODY$

  SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_trspv4(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    ARRAY[$3]::BIGINT[],
    ARRAY[$4]::BIGINT[],
    directed) AS a;

$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- ONE to MANY
--v3.4
CREATE FUNCTION pgr_trsp(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  BIGINT, -- departure
  ANYARRAY, -- destinations
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
  SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_trspv4(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    ARRAY[$3]::BIGINT[],
    $4::BIGINT[],
    directed) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to ONE
--v3.4
CREATE FUNCTION pgr_trsp(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  ANYARRAY, -- departures
  BIGINT, -- destination
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
  SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_trspv4(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    $3::BIGINT[],
    ARRAY[$4]::BIGINT[],
    $5) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to MANY
--v3.4
CREATE FUNCTION pgr_trsp(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  ANYARRAY, -- departures
  ANYARRAY, -- destinations
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
  SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_trspv4(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    $3::BIGINT[],
    $4::BIGINT[],
    $5) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- COMBINATIONS
--v3.4
CREATE FUNCTION pgr_trsp(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  TEXT, -- Combinations SQL
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
  SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_trspv4(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    _pgr_get_statement($3),
    $4) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_trsp(one to one)
- PROPOSED
- Parameters
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: cost, path
  - Departure vertex identifier
  - Destination vertex identifier
- Optional parameters
  - directed
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, BIGINT, ANYARRAY, BOOLEAN)
IS 'pgr_trsp(one to many)
- PROPOSED
- Parameters
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: cost, path
  - Departure vertex identifier
  - Destinations ARRAY[vertices identifier]
- Optional parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_trsp(many to one)
- PROPOSED
- Parameters
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: cost, path
  - Departures ARRAY[vertices identifier]
  - Destination vertex identifier
- Optional parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgr_trsp(many to many)
- PROPOSED
- Parameters
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: cost, path
  - Departures ARRAY[vertices identifier]
  - Destinations ARRAY[vertices identifier]
- Optional parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, TEXT, BOOLEAN)
IS 'pgr_trsp(combinations)
- PROPOSED
- Parameters
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: cost, path
  - Combinations SQL with columns: source, target
- Optional parameters
  - directed := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp.html
';


-- ONE to ONE
--v2.6
CREATE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    BIGINT, -- start_vid
    BIGINT, -- end_vid
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            ARRAY[$3]::BIGINT[],
            ARRAY[$4]::BIGINT[],
            directed) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- ONE to MANY
--v2.6
CREATE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    BIGINT, -- start_vid
    ANYARRAY, -- end_vids
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            ARRAY[$3]::BIGINT[],
            $4::bigint[],
            directed) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- MANY to ONE
--v2.6
CREATE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    ANYARRAY, -- start_vids
    BIGINT, -- end_vid
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            $3::bigint[],
            ARRAY[$4]::BIGINT[],
            $5) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- MANY to MANY
--v2.6
CREATE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    ANYARRAY, -- start_vids
    ANYARRAY, -- end_vids
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            $3::bigint[],
            $4::bigint[],
            $5) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION _pgr_trsp(TEXT, TEXT, BIGINT, BIGINT, BOOLEAN) IS 'pgRouting internal function deprecated on v3.4.0';
COMMENT ON FUNCTION _pgr_trsp(TEXT, TEXT, BIGINT, ANYARRAY, BOOLEAN) IS 'pgRouting internal function deprecated on v3.4.0';
COMMENT ON FUNCTION _pgr_trsp(TEXT, TEXT, ANYARRAY, BIGINT, BOOLEAN) IS 'pgRouting internal function deprecated on v3.4.0';
COMMENT ON FUNCTION _pgr_trsp(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN) IS 'pgRouting internal function deprecated on v3.4.0';

