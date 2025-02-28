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

--v3.8
CREATE FUNCTION pgr_contraction(
    TEXT,     -- edges_sql (required)
    directed BOOLEAN DEFAULT true,
    methods INTEGER[] DEFAULT ARRAY[1,2]::INTEGER[],
    cycles INTEGER DEFAULT 1,
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
  FROM _pgr_contraction(_pgr_get_statement($1), methods::BIGINT[], cycles, forbidden, directed);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

COMMENT ON FUNCTION pgr_contraction(TEXT, BOOLEAN, INTEGER[], INTEGER, BIGINT[])
IS 'pgr_contraction
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Optional Parameters
    - directed := true
    - methods := ARRAY[1,2]
    - cycles := 1
    - forbidden := ARRAY[]::BIGINT[]
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_contraction.html
';

--v3.0
CREATE FUNCTION pgr_contraction(
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
BEGIN
    RAISE NOTICE 'Deprecated Signature pgr_contraction(text,bigint[],integer,integer[],bigint[],boolean) in v3.8.0';
    RETURN QUERY
    SELECT a.type, a.id, a.contracted_vertices, a.source, a.target, a.cost
    FROM _pgr_contraction(_pgr_get_statement($1), $2::BIGINT[],  $3, $4, $5) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_contraction(TEXT, BIGINT[], INTEGER, BIGINT[], BOOLEAN)
IS 'pgr_contraction deprecated in 3.8.0';
