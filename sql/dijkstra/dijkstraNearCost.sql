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
-- pgr_dijkstraNearCost
--------------------

-- ONE to MANY
--v3.2
CREATE FUNCTION pgr_dijkstraNearCost(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], directed, true, true, cap, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to ONE
--v3.2
CREATE FUNCTION pgr_dijkstraNearCost(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT,   -- to_vid (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], directed, true, false, cap, true);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to MANY
--v3.2
CREATE FUNCTION pgr_dijkstraNearCost(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,
    global   BOOLEAN DEFAULT true,

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], directed, true, true, cap, global);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- Combinations SQL signature
--v3.2
CREATE FUNCTION pgr_dijkstraNearCost(
    TEXT,     -- edges_sql (required)
    TEXT,     -- combinations_sql (required)

    directed BOOLEAN DEFAULT true,
    cap      BIGINT DEFAULT 1,
    global   BOOLEAN DEFAULT true,

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT start_vid, end_vid, agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), _pgr_get_statement($2), directed, true, cap, global);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_dijkstraNearCost(TEXT, BIGINT, ANYARRAY, BOOLEAN, BIGINT)
IS 'pgr_dijkstraNearCost(One to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_dijkstraNearCost.html
';

COMMENT ON FUNCTION  pgr_dijkstraNearCost(TEXT, ANYARRAY, BIGINT, BOOLEAN, BIGINT)
IS 'pgr_dijkstraNearCost(Many to One)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To vertex identifier
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_dijkstraNearCost.html
';

COMMENT ON FUNCTION  pgr_dijkstraNearCost(TEXT, ANYARRAY, ANYARRAY, BOOLEAN, BIGINT, BOOLEAN)
IS 'pgr_dijkstraNearCost(Many to Many)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_dijkstraNearCost.html
';

COMMENT ON FUNCTION  pgr_dijkstraNearCost(TEXT, TEXT, BOOLEAN, BIGINT, BOOLEAN)
IS 'pgr_dijkstraNearCost(Combinations)
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Combinations SQL with columns: source, target
- Optional Parameters
  - directed => true
  - cap => 1 (nth found)
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_dijkstraNearCost.html
';
