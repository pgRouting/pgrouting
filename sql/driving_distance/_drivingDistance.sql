/*PGR-GNU*****************************************************************
FILE: _drivingDistance.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: project@pgrouting.org

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

----------------------
-- pgr_drivingDistance
----------------------

CREATE OR REPLACE FUNCTION _pgr_drivingDistance(
    edges_sql TEXT,
    start_vids ANYARRAY,
    distance FLOAT,
    directed BOOLEAN DEFAULT TRUE,
    equicost BOOLEAN DEFAULT FALSE,
    OUT seq INTEGER,
    OUT from_v  BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE c VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_drivingDistance(TEXT, ANYARRAY, FLOAT, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
