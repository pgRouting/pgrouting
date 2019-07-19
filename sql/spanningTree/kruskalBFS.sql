/*PGR-GNU*****************************************************************
File: kruskalBFS.sql

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
-- pgr_kruskalBFS
-----------------


CREATE OR REPLACE FUNCTION pgr_kruskalBFS(
    TEXT,   -- Edge sql (required)
    BIGINT, -- root vertex (required)

    max_depth BIGINT DEFAULT 9223372036854775807,

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
        RAISE EXCEPTION 'Negative value found on ''max_depth'''
        USING HINT = format('Value found: %s', $3);
    END IF;


    RETURN QUERY
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'BFS', $3, -1);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION pgr_kruskalBFS(
    TEXT,     -- Edge sql (required)
    ANYARRAY, -- root vertices (required)

    max_depth BIGINT DEFAULT 9223372036854775807,

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
        RAISE EXCEPTION 'Negative value found on ''max_depth'''
        USING HINT = format('Value found: %s', $3);
    END IF;


    RETURN QUERY
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), $2, 'BFS', $3, -1);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_kruskalBFS(TEXT, BIGINT, BIGINT)
IS 'pgr_kruskalBFS(Single Vertex)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From root vertex identifier
- Optional parameters
    - max_depth: default := 9223372036854775807
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_kruskalBFS.html
';


COMMENT ON FUNCTION pgr_kruskalBFS(TEXT, ANYARRAY, BIGINT)
IS 'pgr_kruskalBFS(multiple Vertices)
- Undirected graph
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - From ARRAY[root vertices identifiers]
- Optional parameters
    - max_depth: default := 9223372036854775807
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_kruskalBFS.html
';
