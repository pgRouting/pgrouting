/*PGR-GNU*****************************************************************
File: _trsp_withPoints.sql

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo
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


--v3.4
CREATE FUNCTION _pgr_trsp_withPoints(
    TEXT,  -- edges_sql
    TEXT,  -- restrictions_sql
    TEXT,  -- points_sql
    ANYARRAY,  -- start_vid
    ANYARRAY,  -- end_vids

    directed BOOLEAN,
    driving_side CHAR,
    details BOOLEAN,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT departure BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE c VOLATILE;


--v3.4
CREATE FUNCTION _pgr_trsp_withPoints(
    TEXT,  -- edges_sql
    TEXT,  -- restrictions_sql
    TEXT,  -- points_sql
    TEXT,  -- combinations_sql

    directed BOOLEAN,
    driving_side CHAR,
    details BOOLEAN,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT departure BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

-- COMMENTS
COMMENT ON FUNCTION _pgr_trsp_withPoints(TEXT, TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN, CHAR, BOOLEAN)
IS 'pgRouting internal function';

COMMENT ON FUNCTION _pgr_trsp_withPoints(TEXT, TEXT, TEXT, TEXT, BOOLEAN, CHAR, BOOLEAN)
IS 'pgRouting internal function';
