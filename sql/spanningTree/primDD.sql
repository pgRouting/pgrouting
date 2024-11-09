/*PGR-GNU*****************************************************************
File: primDD.sql

Copyright (c) 2018 Vicky Vergara
Mail: vicky at vicky at erosion dot dev

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

--------------
-- pgr_primDD
--------------


-- SINGLE VERTEX
--v3.7
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    BIGINT, -- root vertex
    NUMERIC,  -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, depth, start_vid, pred, node, edge, cost, agg_cost
    FROM _pgr_primv4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'DD', -1, $3::FLOAT);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

--v3.7
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    BIGINT, -- root vertex
    FLOAT,  -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, depth, start_vid, pred, node, edge, cost, agg_cost
    FROM _pgr_primv4(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'DD', -1, $3::FLOAT);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--v3.7
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    ANYARRAY, -- root vertex

    NUMERIC, -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, depth, start_vid, pred, node, edge, cost, agg_cost
    FROM _pgr_primv4(_pgr_get_statement($1), $2, 'DD', -1, $3);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


--v3.7
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    ANYARRAY, -- root vertex

    FLOAT, -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, depth, start_vid, pred, node, edge, cost, agg_cost
    FROM _pgr_primv4(_pgr_get_statement($1), $2, 'DD', -1, $3::FLOAT);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_primDD(TEXT, BIGINT, NUMERIC)
IS 'pgr_primDD(Single Vertex)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From root vertex identifier
    - Distance
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_primDD.html
';


COMMENT ON FUNCTION pgr_primDD(TEXT, ANYARRAY, NUMERIC)
IS 'pgr_primDD(Multiple Vertices)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[root vertices identifiers]
    - Distance
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_primDD.html
';


COMMENT ON FUNCTION pgr_primDD(TEXT, BIGINT, FLOAT)
IS 'pgr_primDD(Single Vertex)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From root vertex identifier
    - Distance
- DocumentatiEdgeson:
    - ${PROJECT_DOC_LINK}/pgr_primDD.html
';


COMMENT ON FUNCTION pgr_primDD(TEXT, ANYARRAY, FLOAT)
IS 'pgr_primDD(Multiple Vertices)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[root vertices identifiers]
    - Distance
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_primDD.html
';
