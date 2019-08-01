/*PGR-GNU*****************************************************************
File: lineGraphFull.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Anthony Nicola Tasca
Mail: atasca10@gmail.com

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

CREATE OR REPLACE FUNCTION pgr_lineGraphFull(
    TEXT, -- edges_sql (required)

    OUT seq INTEGER,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT cost FLOAT,
    OUT edge BIGINT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_lineGraphFull(_pgr_get_statement($1))
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_lineGraphFull(TEXT)
IS 'pgr_lineGraphFull
- EXPERIMENTAL
- For Directed Graph
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_lineGraphFull.html
';
