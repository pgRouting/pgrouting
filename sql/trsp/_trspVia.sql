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

-----------------------
-- _pgr_trspVia
-----------------------


--v4.0
CREATE FUNCTION _pgr_trspVia (
  TEXT,
  TEXT,
  ANYARRAY,
  directed BOOLEAN,
  strict BOOLEAN,
  U_turn_on_edge BOOLEAN,

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT,
  OUT route_agg_cost FLOAT
)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE;

COMMENT ON FUNCTION _pgr_trspVia(TEXT, TEXT, ANYARRAY, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
