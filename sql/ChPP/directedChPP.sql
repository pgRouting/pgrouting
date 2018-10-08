/*PGR-GNU*****************************************************************
File: directedChPP.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

CREATE OR REPLACE FUNCTION _pgr_directedChPP(
    edges_sql TEXT,                 -- edges_sql
    only_cost BOOLEAN DEFAULT false,
        OUT seq INTEGER,            -- seq
    OUT node BIGINT,                -- node_id
    OUT edge BIGINT,                -- edge_id
    OUT cost FLOAT,                 -- cost
    OUT agg_cost FLOAT)             -- total cost

RETURNS SETOF RECORD AS
'MODULE_PATHNAME', 'directedChPP'
LANGUAGE c IMMUTABLE STRICT;
