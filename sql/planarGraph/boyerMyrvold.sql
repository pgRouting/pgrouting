/*PGR-GNU*****************************************************************
File: boyerMyrvold.sql

Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Himanshu Raj
Mail: raj.himanshu2@gmail.com

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

------------------
-- pgr_boyerMyrvold
------------------

CREATE OR REPLACE FUNCTION pgr_boyerMyrvold(
    TEXT,   -- edges_sql (required)
    OUT seq BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_boyerMyrvold(_pgr_get_statement($1)) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_boyerMyrvold(TEXT)
IS 'pgr_boyerMyrvold
- EXPERIMENTAL
- Undirected graph
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_boyerMyrvold.html
';
