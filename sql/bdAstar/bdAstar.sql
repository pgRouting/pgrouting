/*PGR-GNU*****************************************************************

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2017 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com

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
--------------------
-- pgr_bdAstar
--------------------


-- one to one
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    TEXT,   -- edges_sql (required)
    BIGINT, -- from_vid (required)
    BIGINT, -- to_vid (required)

    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor NUMERIC DEFAULT 1.0,
    epsilon NUMERIC DEFAULT 1.0,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_bdAstar(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4, $5, $6::FLOAT, $7::FLOAT, false) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- one to many
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor NUMERIC DEFAULT 1.0,
    epsilon NUMERIC DEFAULT 1.0,

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
    FROM _pgr_bdAstar(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $4, $5, $6::FLOAT, $7::FLOAT, false) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- many to one
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT,   -- to_vid (required)

    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor NUMERIC DEFAULT 1.0,
    epsilon NUMERIC DEFAULT 1.0,

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
    FROM _pgr_bdAstar(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $4, $5, $6::FLOAT, $7::FLOAT, false) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- many to many
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor NUMERIC DEFAULT 1.0,
    epsilon NUMERIC DEFAULT 1.0,

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
    SELECT *
    FROM _pgr_bdAstar(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], $4, $5, $6::FLOAT, $7::FLOAT, false);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_bdAstar(TEXT, BIGINT, BIGINT, BOOLEAN, INTEGER, NUMERIC, NUMERIC)
IS 'pgr_bdAstar(One to One)
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost], x1, y1, x2, y2
  - From vertex identifier
  - To vertex identifier
- Optional Parameters: 
  - directed := true
  - heuristic := 5
  - factor := 1
  - epsilon := 1
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_bdAstar.html
';

COMMENT ON FUNCTION pgr_bdAstar(TEXT, BIGINT, ANYARRAY, BOOLEAN, INTEGER, NUMERIC, NUMERIC)
IS 'pgr_bdAstar(One to Many)
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost], x1, y1, x2, y2
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Optional Parameters: 
  - directed := true
  - heuristic := 5
  - factor := 1
  - epsilon := 1
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_bdAstar.html
';


COMMENT ON FUNCTION pgr_bdAstar(TEXT, ANYARRAY, BIGINT, BOOLEAN, INTEGER, NUMERIC, NUMERIC)
IS 'pgr_bdAstar(Many to One)
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost], x1, y1, x2, y2
  - From ARRAY[vertices identifiers]
  - To vertex identifier
- Optional Parameters: 
  - directed := true
  - heuristic := 5
  - factor := 1
  - epsilon := 1
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_bdAstar.html
';

COMMENT ON FUNCTION pgr_bdAstar(TEXT, ANYARRAY, ANYARRAY, BOOLEAN, INTEGER, NUMERIC, NUMERIC)
IS 'pgr_bdAstar(Many to Many)
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost], x1, y1, x2, y2
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Optional Parameters: 
  - directed := true
  - heuristic := 5
  - factor := 1
  - epsilon := 1
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_bdAstar.html
';
