/*PGR-GNU*****************************************************************
File: minCostMaxFlow.sql

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

------------------------
--    MANY TO MANY
------------------------

CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    edges_sql TEXT,                 -- edges_sql
    sources ANYARRAY,               -- sources
    targets ANYARRAY,               -- targets
    only_cost BOOLEAN DEFAULT false,
        OUT seq INTEGER,            -- seq
    OUT edge BIGINT,                -- edge_id
    OUT source BIGINT,              -- start vertex
    OUT target BIGINT,              -- end vertex
    OUT flow BIGINT,                -- flow
    OUT residual_capacity BIGINT,   -- residual capacity
    OUT cost FLOAT,                 -- cost
    OUT agg_cost FLOAT)             -- total cost

RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'minCostMaxFlow_many_to_many'
LANGUAGE c IMMUTABLE STRICT;

------------------------
--    ONE TO MANY
------------------------

CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    edges_sql TEXT,                 -- edges_sql
    sources BIGINT,                 -- source
    targets ANYARRAY,               -- targets
    only_cost BOOLEAN DEFAULT false,
        OUT seq INTEGER,            -- seq
    OUT edge BIGINT,                -- edge_id
    OUT source BIGINT,              -- start vertex
    OUT target BIGINT,              -- end vertex
    OUT flow BIGINT,                -- flow
    OUT residual_capacity BIGINT,   -- residual capacity
    OUT cost FLOAT,                 -- cost
    OUT agg_cost FLOAT)             -- total cost

  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * 
        FROM pgr_minCostMaxFlow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], only_cost);
  $BODY$
  LANGUAGE SQL VOLATILE;

------------------------
--    MANY TO ONE 
------------------------
CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    edges_sql TEXT,                 -- edges_sql
    sources ANYARRAY,               -- sources
    targets BIGINT,                 -- target
    only_cost BOOLEAN DEFAULT false,
        OUT seq INTEGER,            -- seq
    OUT edge BIGINT,                -- edge_id
    OUT source BIGINT,              -- start vertex
    OUT target BIGINT,              -- end vertex
    OUT flow BIGINT,                -- flow
    OUT residual_capacity BIGINT,   -- residual capacity
    OUT cost FLOAT,                 -- cost
    OUT agg_cost FLOAT)             -- total cost

  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM pgr_minCostMaxFlow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], only_cost);
  $BODY$
  LANGUAGE SQL VOLATILE;

------------------------
--    ONE TO ONE 
------------------------
CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    edges_sql TEXT,                 -- edges_sql
    sources BIGINT,                 -- source
    targets BIGINT,                 -- target
    only_cost BOOLEAN DEFAULT false,
        OUT seq INTEGER,            -- seq
    OUT edge BIGINT,                -- edge_id
    OUT source BIGINT,              -- start vertex
    OUT target BIGINT,              -- end vertex
    OUT flow BIGINT,                -- flow
    OUT residual_capacity BIGINT,   -- residual capacity
    OUT cost FLOAT,                 -- cost
    OUT agg_cost FLOAT)             -- total cost

  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * 
        FROM pgr_minCostMaxFlow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], only_cost);
  $BODY$
  LANGUAGE SQL VOLATILE;
