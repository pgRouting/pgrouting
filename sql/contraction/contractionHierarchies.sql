/*PGR-GNU*****************************************************************
File: contractionHierarchies.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) Aurélie Bousquet - 2024
Mail: aurelie.bousquet at oslandia.com

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

---------------------------
---------------------------
-- contractionHierarchies
---------------------------
---------------------------

--------------------------------
-- pgr_contractionHierarchies
--------------------------------


--v3.8
CREATE FUNCTION pgr_contractionHierarchies(
    TEXT,     -- edges_sql (required)
    directed BOOLEAN DEFAULT true,
    forbidden BIGINT[] DEFAULT ARRAY[]::BIGINT[],

    OUT type TEXT,
    OUT id BIGINT,
    OUT contracted_vertices BIGINT[],
    OUT source BIGINT,
    OUT target BIGINT,
    OUT cost FLOAT,
    OUT metric BIGINT,
    OUT vertex_order BIGINT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT type, id, contracted_vertices, source, target, cost, metric, vertex_order
    FROM _pgr_contractionhierarchies(_pgr_get_statement($1), $3::BIGINT[], $2);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_contractionHierarchies(TEXT, BOOLEAN, BIGINT[])
IS 'pgr_contractionHierarchies
EXPERIMENTAL
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Optional Parameters
    - forbidden := ARRAY[]::BIGINT[]
    - directed := true
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_contractionHierarchies.html
';
