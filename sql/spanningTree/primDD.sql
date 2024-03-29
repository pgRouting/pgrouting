/*PGR-GNU*****************************************************************
File: primDD.sql

Copyright (c) 2018 Vicky Vergara
Mail: vicky at georepublic dot de

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
--v3.0
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    BIGINT, -- root vertex
    NUMERIC,  -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    IF $3 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''distance'''
        USING HINT = format('Value found: %s', $3);
    END IF;

    RETURN QUERY
    SELECT a.seq, a.depth, a.start_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_prim(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'DD', -1, $3::FLOAT) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

--v3.0
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    BIGINT, -- root vertex
    FLOAT,  -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    IF $3 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''distance'''
        USING HINT = format('Value found: %s', $3);
    END IF;

    RETURN QUERY
    SELECT a.seq, a.depth, a.start_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_prim(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'DD', -1, $3::FLOAT) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- MULTIPLE VERTICES
--v3.0
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    ANYARRAY, -- root vertex

    NUMERIC, -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    IF $3 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''distance'''
        USING HINT = format('Value found: %s', $3);
    END IF;

    RETURN QUERY
    SELECT a.seq, a.depth, a.start_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_prim(_pgr_get_statement($1), $2, 'DD', -1, $3) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


--v3.0
CREATE FUNCTION pgr_primDD (
    TEXT,   -- Edge sql
    ANYARRAY, -- root vertex

    FLOAT, -- distance

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    IF $3 < 0 THEN
        RAISE EXCEPTION 'Negative value found on ''distance'''
        USING HINT = format('Value found: %s', $3);
    END IF;

    RETURN QUERY
    SELECT a.seq, a.depth, a.start_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_prim(_pgr_get_statement($1), $2, 'DD', -1, $3::FLOAT) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


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
