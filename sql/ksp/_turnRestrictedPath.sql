/*PGR-GNU*****************************************************************
File: _turnRestrictedPath.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

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
CREATE FUNCTION _pgr_turnRestrictedPath_v4(
  TEXT,   -- edges_sql
  TEXT,   -- restrictions_sql
  ANYARRAY, -- start_vertex
  ANYARRAY, -- end_vertex
  INTEGER,  -- K cycles

  BOOLEAN,  -- directed
  BOOLEAN,  -- heap_paths
  BOOLEAN,
  BOOLEAN,

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_turnRestrictedPath_v4(TEXT, TEXT, ANYARRAY, ANYARRAY, INTEGER, BOOLEAN, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
