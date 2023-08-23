/*PGR-GNU*****************************************************************
File: _ksp.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2023 Aniket Agarwal
aniketgarg187 at gmail.com

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
---------------
---------------
-- pgr_ksp
---------------
---------------

--v2.6
CREATE FUNCTION _pgr_ksp(
    edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    k INTEGER,

    directed BOOLEAN,
    heap_paths BOOLEAN,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_ksp(TEXT, BIGINT, BIGINT, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';

--v3.6
CREATE FUNCTION _v4pgr_ksp(
    edges_sql TEXT,
    start_vids ANYARRAY,
    end_vids ANYARRAY,
    k INTEGER,

    directed BOOLEAN,
    heap_paths BOOLEAN,

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

--v3.6
CREATE FUNCTION _v4pgr_ksp(
    edges_sql TEXT,
    combinations TEXT,

    k INTEGER,

    directed BOOLEAN,
    heap_paths BOOLEAN,

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

-- COMMENTS

COMMENT ON FUNCTION _v4pgr_ksp(TEXT, ANYARRAY, ANYARRAY, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';

COMMENT ON FUNCTION _v4pgr_ksp(TEXT, TEXT, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
