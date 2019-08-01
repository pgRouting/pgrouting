/*PGR-GNU*****************************************************************
File: bellman_ford_neg.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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
-------------------------
-------------------------
-- bellman_ford
-------------------------
-------------------------

CREATE OR REPLACE FUNCTION _pgr_bellmanFord(
    edges_sql TEXT,
    from_vids ANYARRAY,
    to_vids   ANYARRAY,
    directed  BOOLEAN,
    only_cost BOOLEAN,

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
LANGUAGE c IMMUTABLE STRICT;


-- COMMENTS

COMMENT ON FUNCTION _pgr_bellmanFord(TEXT, ANYARRAY, ANYARRAY, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
