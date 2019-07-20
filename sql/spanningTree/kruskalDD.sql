/*PGR-GNU*****************************************************************
File: kruskalDD.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh28@gmail.com

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


-----------------
-- pgr_kruskalDD
-----------------


-- SINGLE VERTEX
CREATE OR REPLACE FUNCTION pgr_kruskalDD (
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
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'DD', -1, $3::FLOAT);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_kruskalDD (
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
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'DD', -1, $3::FLOAT);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- MULTIPLE VERTICES
CREATE OR REPLACE FUNCTION pgr_kruskalDD (
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
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), $2, 'DD', -1, $3);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION pgr_kruskalDD (
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
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), $2, 'DD', -1, $3::FLOAT);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_kruskalDD(TEXT, BIGINT, NUMERIC)
IS 'pgr_kruskalDD(Single Vertex)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From root vertex identifier
    - Distance
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_kruskalDD.html
';


COMMENT ON FUNCTION pgr_kruskalDD(TEXT, ANYARRAY, NUMERIC)
IS 'pgr_kruskalDD(Multiple Vertices)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[root vertices identifiers]
    - Distance
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_kruskalDD.html
';


COMMENT ON FUNCTION pgr_kruskalDD(TEXT, BIGINT, FLOAT)
IS 'pgr_kruskalDD(Single Vertex)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From root vertex identifier
    - Distance
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_kruskalDD.html
';


COMMENT ON FUNCTION pgr_kruskalDD(TEXT, ANYARRAY, FLOAT)
IS 'pgr_kruskalDD(Multiple Vertices)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[root vertices identifiers]
    - Distance
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_kruskalDD.html
';
