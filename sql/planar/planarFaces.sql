/*PGR-GNU*****************************************************************
File: planarFaces.sql

Copyright (c) 2007-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2026 Sakir Ahmed
Mail: sakirahmed75531 at gmail.com

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

--v4.1
CREATE FUNCTION pgr_planarFaces(
  TEXT,   -- edges_sql (required)

  OUT seq     BIGINT,
  OUT face_id BIGINT,
  OUT edge_id BIGINT,
  OUT side    INTEGER)

RETURNS SETOF RECORD AS
$BODY$
  SELECT seq, face_id, edge_id, side
  FROM _pgr_planarFaces(_pgr_get_statement($1)) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST ${COST_HIGH} ROWS ${ROWS_HIGH};


COMMENT ON FUNCTION pgr_planarFaces(TEXT)
IS 'pgr_planarFaces
- EXPERIMENTAL
- Undirected graph
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_planarFaces.html
';
