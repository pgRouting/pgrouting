/*PGR-GNU*****************************************************************
File: _withPointsDD.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project at pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Yige Huang
Mail: square1ge at gmail.com

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

-------------------
-- pgr_withPointsDD
-------------------

--v3.6
CREATE FUNCTION _pgr_v4withPointsDD(
    edges_sql TEXT,
    points_sql TEXT,
    start_pid ANYARRAY,
    distance FLOAT,
    driving_side CHAR,

    directed BOOLEAN DEFAULT true,
    details BOOLEAN DEFAULT false,
    equicost BOOLEAN DEFAULT false,

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_v4withPointsDD(TEXT, TEXT, ANYARRAY, FLOAT, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';

/* TODO remove on v4 */
--v3.0
CREATE FUNCTION _pgr_withPointsDD(
    edges_sql TEXT,
    points_sql TEXT,
    start_pid ANYARRAY,
    distance FLOAT,

    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b',
    details BOOLEAN DEFAULT false,
    equicost BOOLEAN DEFAULT false,

    OUT seq INTEGER,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_withPointsDD(TEXT, TEXT, ANYARRAY, FLOAT, BOOLEAN, CHAR, BOOLEAN, BOOLEAN)
IS 'pgRouting deprecated function';
