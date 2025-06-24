/*PGR-GNU*****************************************************************
File: sequentialVertexColoring.sql

Generated with Template by:
Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Ashish Kumar
Mail: ashishkr23438@gmail.com

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

--3.2
CREATE FUNCTION pgr_sequentialVertexColoring(
    TEXT,    -- edges_sql (required)

    OUT node BIGINT,
    OUT color BIGINT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT vertex_id, color_id
    FROM _pgr_sequentialVertexColoring(_pgr_get_statement($1));
$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_sequentialVertexColoring(TEXT)
IS 'pgr_sequentialVertexColoring
- PROPOSED
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_sequentialVertexColoring.html
';
