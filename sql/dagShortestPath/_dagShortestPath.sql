/*PGR-GNU*****************************************************************
File: _dagShortestPath.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Sourabh Garg
Mail: sourabh.garg.mat@gmail.com

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
CREATE FUNCTION _pgr_dagShortestPath_v4(
    TEXT, -- edges

    ANYARRAY, -- departures
    ANYARRAY, -- arrivals

    BOOLEAN, -- only_cost
    BOOLEAN, -- normal

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

--v4.0
CREATE FUNCTION _pgr_dagShortestPath_v4(
    TEXT, -- edges
    TEXT, -- combinations

    BOOLEAN, -- only_cost

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

-- COMMENTS

COMMENT ON FUNCTION _pgr_dagShortestPath_v4(TEXT, ANYARRAY, ANYARRAY, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';

COMMENT ON FUNCTION _pgr_dagShortestPath_v4(TEXT, TEXT, BOOLEAN)
IS 'pgRouting internal function';
