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
CREATE FUNCTION _pgr_withPointsDDv4(
    TEXT,     -- edges SQL
    TEXT,     -- points SQL
    ANYARRAY, -- roots
    FLOAT,    -- distance
    CHAR,     -- driving_side,

    BOOLEAN,  -- directed
    BOOLEAN,  -- details
    BOOLEAN,  -- equicost

    OUT seq BIGINT,
    OUT depth BIGINT,
    OUT start_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_withPointsDDv4(TEXT, TEXT, ANYARRAY, FLOAT, CHAR, BOOLEAN, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
