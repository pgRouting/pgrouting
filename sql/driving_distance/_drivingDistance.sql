/*PGR-GNU*****************************************************************
FILE: _drivingDistance.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky AT erosion.dev

Copyright (c) 2023 Aryan Gupta
Mail: guptaaryan1010 AT gmail.com

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

--v3.6
CREATE FUNCTION _pgr_drivingDistancev4(
    TEXT,     -- edges_sql
    ANYARRAY, -- start_vids
    FLOAT,    -- distance
    BOOLEAN,  -- directed
    BOOLEAN,  -- equicost
    OUT seq BIGINT,
    OUT depth  BIGINT,
    OUT start_vid  BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE c VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_drivingDistancev4(TEXT, ANYARRAY, FLOAT, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
