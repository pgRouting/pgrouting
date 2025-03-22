/*PGR-GNU*****************************************************************
File: deadEndContraction.sql

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

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

--v3.8
CREATE FUNCTION pgr_contractionDeadEnd(
    TEXT,     -- edges_sql (required)

    directed BOOLEAN DEFAULT true,
    forbidden BIGINT[] DEFAULT ARRAY[]::BIGINT[],

    OUT type TEXT,
    OUT id BIGINT,
    OUT contracted_vertices BIGINT[],
    OUT source BIGINT,
    OUT target BIGINT,
    OUT cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT type, id, contracted_vertices, source, target, cost
    FROM _pgr_contraction(_pgr_get_statement($1), ARRAY[1]::BIGINT[], 1, $3, $2);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

COMMENT ON FUNCTION pgr_contractionDeadEnd(TEXT, BOOLEAN, BIGINT[])
IS 'pgr_contractionDeadEnd
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Optional Parameters
    - directed := true
    - forbidden := ARRAY[]::BIGINT[]
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_contractionDeadEnd.html
';
