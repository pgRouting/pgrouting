/*PGR-GNU*****************************************************************

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2021 Vicky Vergara
Mail: vicky at georepublic dot de

2021:
- Complete rewrite of one to one
- New: one to many, many to one, many to many, combinations
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

--------------
--------------
-- trsp
--------------
--------------

--v4.0
CREATE FUNCTION _pgr_trsp(
  TEXT, -- edges sql
  TEXT, -- restrictions sql
  ANYARRAY,
  ANYARRAY,
  directed BOOLEAN,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT departure BIGINT,
  OUT destination BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE 'c' VOLATILE;


--v4.0
CREATE FUNCTION _pgr_trsp(
  TEXT, -- edges sql
  TEXT, -- restrictions sql
  TEXT, -- combinations sql
  directed BOOLEAN,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT departure BIGINT,
  OUT destination BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE 'c' VOLATILE;


-- COMMENTS
COMMENT ON FUNCTION _pgr_trsp(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgRouting internal function';
COMMENT ON FUNCTION _pgr_trsp(TEXT, TEXT, TEXT, BOOLEAN)
IS 'pgRouting internal function';
