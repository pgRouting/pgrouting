/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
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
CREATE FUNCTION pgr_trspVia(
  TEXT, -- edges SQL (required)
  TEXT, -- restrictions SQL
  ANYARRAY,  -- via vids (required)
  directed BOOLEAN DEFAULT true,
  strict BOOLEAN DEFAULT false,
  U_turn_on_edge BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT departure BIGINT,
  OUT destination BIGINT,
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
  FROM _pgr_trspVia(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    $3, $4, $5, $6);

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_trspVia(TEXT, TEXT, ANYARRAY, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgr_trspVia
- PROTOTYPE
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - ARRAY[Via vertices identifiers
    - directed
    - has reverse cost
- Optional parameters
    - restrictions_sql := NULL
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trspVia.html
';
