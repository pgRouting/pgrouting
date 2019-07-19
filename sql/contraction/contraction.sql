/*PGR-GNU*****************************************************************
File: contraction.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Rohith Reddy
Mail:

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

--------------------
--------------------
-- contraction
--------------------
--------------------

--------------------
-- pgr_contraction
--------------------


CREATE OR REPLACE FUNCTION pgr_contraction(
    TEXT,     -- edges_sql (required)
    BIGINT[], -- contraction_order (required)

    max_cycles INTEGER DEFAULT 1,
    forbidden_vertices BIGINT[] DEFAULT ARRAY[]::BIGINT[],
    directed BOOLEAN DEFAULT true,

    OUT type TEXT,
    OUT id BIGINT,
    OUT contracted_vertices BIGINT[],
    OUT source BIGINT,
    OUT target BIGINT,
    OUT cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_contraction(_pgr_get_statement($1), $2::BIGINT[],  $3, $4, $5);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_contraction(TEXT, BIGINT[], INTEGER, BIGINT[], BOOLEAN)
IS 'pgr_contraction
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
    - ARRAY [Contraction order]
- Optional Parameters
    - max_cycles := 1
    - forbidden_vertices := ARRAY[]::BIGINT[]
    - directed := true
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_contraction.html
';
