/*PGR-GNU*****************************************************************

Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Celia Virginia Vergara Castillo
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
-- pgr_dijkstraNear
--------------------

-- ONE to MANY
CREATE OR REPLACE FUNCTION pgr_dijkstraNear(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,

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
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], directed, false, true, cap);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to ONE
CREATE OR REPLACE FUNCTION pgr_dijkstraNear(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT,   -- to_vid (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,

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
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], directed, false, false, cap);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to MANY
CREATE OR REPLACE FUNCTION pgr_dijkstraNear(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,
    global   BOOLEAN DEFAULT true,

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
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], directed, false, true, cap, global);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- Combinations SQL signature
CREATE OR REPLACE FUNCTION pgr_dijkstraNear(
    TEXT,     -- edges_sql (required)
    TEXT,     -- combinations_sql (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,
    global   BOOLEAN DEFAULT true,

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
    FROM _pgr_dijkstra(_pgr_get_statement($1), _pgr_get_statement($2), directed, false, cap, global);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_dijkstraNear(TEXT, BIGINT, ANYARRAY, BOOLEAN, BIGINT)
IS 'pgr_dijkstraNear(One to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_dijkstraNear.html
';

COMMENT ON FUNCTION  pgr_dijkstraNear(TEXT, ANYARRAY, BIGINT, BOOLEAN, BIGINT)
IS 'pgr_dijkstraNear(Many to One)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To vertex identifier
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_dijkstraNear.html
';

COMMENT ON FUNCTION  pgr_dijkstraNear(TEXT, ANYARRAY, ANYARRAY, BOOLEAN, BIGINT, BOOLEAN)
IS 'pgr_dijkstraNear(Many to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_dijkstraNear.html
';

COMMENT ON FUNCTION  pgr_dijkstraNear(TEXT, TEXT, BOOLEAN, BIGINT, BOOLEAN)
IS 'pgr_dijkstraNear(Combinations)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Combinations SQL with columns: source, target
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_dijkstraNear.html
';
