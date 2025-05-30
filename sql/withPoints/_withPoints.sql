/*PGR-GNU*****************************************************************
File: _withPoints.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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
CREATE FUNCTION _pgr_withPoints_v4(
    TEXT, -- edges
    TEXT, -- points

    ANYARRAY, -- departures
    ANYARRAY, -- arrivals

    BOOLEAN, --directed
    CHAR,    -- driving_side
    BOOLEAN, -- details

    BOOLEAN, -- only_cost
    BOOLEAN, -- normal

    BIGINT,  -- n_goals
    BOOLEAN, -- global

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
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_withPoints_v4(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN, CHAR, BOOLEAN, BOOLEAN, BOOLEAN,
  BIGINT, BOOLEAN)
IS 'pgRouting internal function';

--v4.0
CREATE FUNCTION _pgr_withPoints_v4(
    TEXT, -- edges
    TEXT, -- points
    TEXT, -- combinations

    BOOLEAN, --directed
    CHAR,    -- driving_side
    BOOLEAN, -- details
    BOOLEAN, -- only_cost

    BIGINT,  -- n_goals
    BOOLEAN, -- global

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
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_withPoints_v4(TEXT, TEXT, TEXT, BOOLEAN, CHAR, BOOLEAN, BOOLEAN, BIGINT, BOOLEAN)
IS 'pgRouting internal function';
