/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakr@yahoo.com, estebanzimanyi@gmail.com

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

---------------
-- pgr_dijkstra
---------------

-- ONE to ONE
CREATE OR REPLACE FUNCTION pgr_dijkstra(
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
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- ONE to MANY
CREATE OR REPLACE FUNCTION pgr_dijkstra(
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
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $4, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to ONE
CREATE OR REPLACE FUNCTION pgr_dijkstra(
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
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $4, false, false) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to MANY
CREATE OR REPLACE FUNCTION pgr_dijkstra(
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
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], $4, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- Combinations SQL signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(
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
    FROM _pgr_dijkstra(_pgr_get_statement($1), _pgr_get_statement($2), $3, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_dijkstra(TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_dijkstra(One to One)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From vertex identifier
   - To vertex identifier
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_dijkstra.html
';

COMMENT ON FUNCTION pgr_dijkstra(TEXT, BIGINT, ANYARRAY, BOOLEAN)
IS 'pgr_dijkstra(One to Many)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From vertex identifier
   - To ARRAY[vertices identifiers]
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_dijkstra.html
';

COMMENT ON FUNCTION pgr_dijkstra(TEXT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_dijkstra(Many to One)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From ARRAY[vertices identifiers]
   - To vertex identifier
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_dijkstra.html
';

COMMENT ON FUNCTION pgr_dijkstra(TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgr_dijkstra(Many to Many)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From ARRAY[vertices identifiers]
   - To ARRAY[vertices identifiers]
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_dijkstra.html
';

COMMENT ON FUNCTION pgr_dijkstra(TEXT, TEXT, BOOLEAN)
IS 'pgr_dijkstra(One to One)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - Combinations SQL with columns: source, target
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_dijkstra.html
';
