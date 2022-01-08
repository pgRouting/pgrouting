/*PGR-GNU*****************************************************************
File: trsp_withPointsVia.sql

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo

Copyright (c) 2022 pgRouting developers
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

--v4.0
CREATE FUNCTION pgr_trsp_withPointsVia(
  TEXT, -- Edges SQL
  TEXT, -- Restrictions SQL
  TEXT, -- Points SQL
  ANYARRAY,  -- via vids

  directed BOOLEAN DEFAULT true,

  strict BOOLEAN DEFAULT false,
  U_turn_on_edge BOOLEAN DEFAULT true,

  details BOOLEAN DEFAULT false,
  driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_pid BIGINT,
  OUT end_pid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT,
  OUT route_agg_cost FLOAT
)
RETURNS SETOF RECORD AS

$BODY$
BEGIN

  RETURN QUERY
  SELECT *
  FROM _pgr_trsp_withPointsVia(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    _pgr_get_statement($3),
    $4, $5, $6, $7, $8, $9);

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS
COMMENT ON FUNCTION pgr_trsp_withPointsVia(TEXT, TEXT, TEXT, ANYARRAY, BOOLEAN, BOOLEAN, BOOLEAN, BOOLEAN, CHAR)
IS 'pgr_trspVia
- PROPOSED
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Restrictions SQL with columns: id, cost, path
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - Via ARRAY[ordered vertices/points identifiers]
- Optional Parameters:
  - directed := ''true''
  - driving_side := ''b''
  - details := ''false''
  - U_turn_on_edge := ''true''
  - strict := ''false''
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp_withPointsVia.html
';
