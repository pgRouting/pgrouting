/*PGR-GNU*****************************************************************
File: randomSpanningTree.sql

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

-----------------------
-- pgr_randomSpanTree
-----------------------


CREATE OR REPLACE FUNCTION pgr_randomSpanTree(
    TEXT,   -- edges_sql (required)
    BIGINT, -- root (required)

    directed BOOLEAN DEFAULT TRUE,

    OUT seq BIGINT,            -- Seq
    OUT root_vertex BIGINT,
    OUT edge BIGINT,	     	-- Edge linked to that node
    OUT cost FLOAT,             -- Cost of edge
    OUT tree_cost FLOAT)        -- Spanning tree cost
RETURNS SETOF RECORD AS
$BODY$
DECLARE
    connectedComponent BIGINT;
BEGIN
    SELECT COUNT(DISTINCT component) INTO connectedComponent
    FROM pgr_connectedComponents(
        'SELECT id, source, target, cost, reverse_cost
        FROM edge_table'
    );

    IF (connectedComponent = 1) THEN
       SELECT *
       FROM _pgr_randomSpanTree(_pgr_get_statement($1), $2, $3, TRUE);
    ELSE
       SELECT *
       FROM _pgr_randomSpanTree(_pgr_get_statement($1), $2, $3, FALSE);
    END IF;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_randomSpanTree(TEXT, BIGINT)
IS 'pgr_randomSpanTree
- EXPERIMENTAL
- Undirected graph
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Root vertex identifier
- Optional Parameters
  - directed: default := ''true''
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_randomSpanTree.html
';
