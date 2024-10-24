/*PGR-GNU*****************************************************************
File: _trsp.sql

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org
Copyright (c) 2022 Vicky Vergara
* Added combinations internal function

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


--------------
-- _trsp
--------------

--v3.6
CREATE FUNCTION _pgr_trspv4(
    TEXT,     -- edges SQL
    TEXT,     -- restrictions SQL
    ANYARRAY, -- departures
    ANYARRAY, -- destinations
    BOOLEAN,  -- directed

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE 'c' VOLATILE;

--v3.6
CREATE FUNCTION _pgr_trspv4(
    TEXT, -- edges SQL
    TEXT, -- restrictions SQL
    TEXT, -- combinations SQL
    BOOLEAN,  -- directed

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE 'c' VOLATILE;

COMMENT ON FUNCTION _pgr_trspv4(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgRouting internal function';

COMMENT ON FUNCTION _pgr_trspv4(TEXT, TEXT, TEXT, BOOLEAN)
IS 'pgRouting internal function';

--v2.6
CREATE FUNCTION _trsp(
    TEXT, -- edges SQL
    TEXT, -- restrictions SQL
    ANYARRAY,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE 'c' VOLATILE;

--v3.4
CREATE FUNCTION _v4trsp(
    TEXT, -- edges SQL
    TEXT, -- restrictions SQL
    ANYARRAY,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
  SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_trspv4( $1, $2, $3, $4, $5);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

--v3.4
CREATE FUNCTION _v4trsp(
    TEXT, -- edges SQL
    TEXT, -- restrictions SQL
    TEXT, -- combinations SQL
    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
  SELECT seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
  FROM _pgr_trspv4( $1, $2, $3, $4);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS
COMMENT ON FUNCTION _trsp(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgRouting internal function deprecated on v3.4.0';

COMMENT ON FUNCTION _v4trsp(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgRouting internal function deprecated on v3.6.0';

COMMENT ON FUNCTION _v4trsp(TEXT, TEXT, TEXT, BOOLEAN)
IS 'pgRouting internal function deprecated on v3.6.0';
