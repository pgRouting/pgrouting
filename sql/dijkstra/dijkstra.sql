/*PGR-GNU*****************************************************************
File: dijkstra.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
Mail: m_attia_sakr at yahoo.com, estebanzimanyi at gmail.com

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
--v3.0
CREATE FUNCTION pgr_dijkstra(
    TEXT,   -- edges_sql (required)
    BIGINT, -- start_vid (required)
    BIGINT, -- end_vid (required)

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
    FROM _pgr_dijkstra_v4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[],
        $4, false, true, 0, false);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- ONE TO MANY
--v3.0
CREATE FUNCTION pgr_dijkstra(
    TEXT,     -- edges_sql (required)
    BIGINT,   -- start_vid (required)
    ANYARRAY, -- end_vid (required)

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
    FROM _pgr_dijkstra_v4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[],
       $4, false, true, 0, false);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY TO ONE
--v3.0
CREATE FUNCTION pgr_dijkstra(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- start_vid (required)
    BIGINT,   -- end_vid (required)

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
    FROM _pgr_dijkstra_v4(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[],
$4, false, false, 0, false);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY TO MANY
--v3.0
CREATE FUNCTION pgr_dijkstra(
    TEXT,     -- edges_sql (required)
    ANYARRAY, -- start_vid (required)
    ANYARRAY, -- end_vid (required)

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
    FROM _pgr_dijkstra_v4(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[],
      $4, false, true, 0, false);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- Combinations SQL signature
--v3.1
CREATE FUNCTION pgr_dijkstra(
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
    SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
    FROM _pgr_dijkstra_v4(_pgr_get_statement($1), _pgr_get_statement($2),
      $3, false, 0, false);
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;



COMMENT ON FUNCTION pgr_dijkstra(TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_dijkstra(One to One)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From vertex identifier
   - To vertex identifier
- Optional Parameters
   - directed := true
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_dijkstra.html
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
   - ${PROJECT_DOC_LINK}/pgr_dijkstra.html
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
   - ${PROJECT_DOC_LINK}/pgr_dijkstra.html
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
   - ${PROJECT_DOC_LINK}/pgr_dijkstra.html
';

COMMENT ON FUNCTION pgr_dijkstra(TEXT, TEXT, BOOLEAN)
IS 'pgr_dijkstra(Combinations)
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
   - Combinations SQL with columns: source, target
- Optional Parameters
   - directed := true
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_dijkstra.html
';
