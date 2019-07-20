/*PGR-GNU*****************************************************************

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Celia Virginia Vergara Castillo
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
-- _pgr_dijkstraNear
--------------------

-- ONE to MANY
CREATE OR REPLACE FUNCTION _pgr_dijkstraNear(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- from_vid (required)
    ANYARRAY, -- to_vids (required)
    BIGINT,   -- stop_at (required)

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
    SELECT seq, path_seq, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $5, false, true, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;


-- MANY to ONE
CREATE OR REPLACE FUNCTION _pgr_dijkstraNear(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT,   -- to_vid (required)
    BIGINT,   -- stop_at (required)

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
    SELECT seq, path_seq, start_vid, node, edge, cost, agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $5, false, false, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

-- MANY to MANY
CREATE OR REPLACE FUNCTION _pgr_dijkstraNear(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)
    BIGINT,   -- stop_at (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT end_vid BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $5, false, false, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION _pgr_dijkstraNear(TEXT, BIGINT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_dijkstraNear(One to Many)
- PRE-EXPERIMENTAL
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From vertex identifier
   - To ARRAY[vertices identifiers]
   - Stop at nth found
- Optional Parameters
   - directed := true
- Documentation: None
';

COMMENT ON FUNCTION  _pgr_dijkstraNear(TEXT, ANYARRAY, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_dijkstraNear(Many to One)
- PRE-EXPERIMENTAL
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From ARRAY[vertices identifiers]
   - To vertex identifier
   - Stop at nth found
- Optional Parameters
   - directed := true
- Documentation: None
';

COMMENT ON FUNCTION  _pgr_dijkstraNear(TEXT, ANYARRAY, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_dijkstraNear(Many to Many)
- PRE-EXPERIMENTAL
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From ARRAY[vertices identifiers]
   - To ARRAY[vertices identifiers]
   - Stop at nth found
- Optional Parameters
   - directed := true
- Documentation: None
';
