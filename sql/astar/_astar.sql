/*PGR-GNU*****************************************************************
File: astarV3.0.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
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


CREATE OR REPLACE FUNCTION _pgr_astar(
    edges_sql TEXT, -- XY edges sql
    start_vids ANYARRAY,
    end_vids ANYARRAY,
    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor FLOAT DEFAULT 1.0,
    epsilon FLOAT DEFAULT 1.0,
    only_cost BOOLEAN DEFAULT false,
    normal BOOLEAN DEFAULT false,
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'${MODULE_PATHNAME}', 'astarManyToMany'
LANGUAGE c VOLATILE STRICT;
