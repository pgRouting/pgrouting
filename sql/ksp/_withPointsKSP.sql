/*PGR-GNU*****************************************************************
File: _withPointsKSP.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

--------------------
-- pgr_withPointsKSP
--------------------

CREATE OR REPLACE FUNCTION _pgr_withPointsKSP(
    edges_sql TEXT,
    points_sql TEXT,
    start_pid BIGINT,
    end_pid BIGINT,
    k INTEGER,

    directed BOOLEAN,
    heap_paths BOOLEAN,
    driving_side CHAR,
    details BOOLEAN,

    OUT seq INTEGER, OUT path_id INTEGER, OUT path_seq INTEGER,
    OUT node BIGINT, OUT edge BIGINT,
    OUT cost FLOAT, OUT agg_cost FLOAT)
  RETURNS SETOF RECORD AS
    'MODULE_PATHNAME'
    LANGUAGE c STABLE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_withPointsKSP(TEXT, TEXT, BIGINT, BIGINT, INTEGER, BOOLEAN, BOOLEAN, CHAR, BOOLEAN)
IS 'pgRouting internal function';
