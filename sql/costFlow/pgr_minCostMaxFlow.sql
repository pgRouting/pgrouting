/*PGR-GNU*****************************************************************
File: pgr_minCostMaxFlow.sql

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
-- pgr_minCostMaxFlow
------------------------

--    ONE TO ONE
CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    TEXT,   -- edges_sql (required)
    BIGINT,   -- source (required)
    BIGINT,   -- target (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE;

--    MANY TO ONE
CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    TEXT,   -- edges_sql (required)
    ANYARRAY,   -- source (required)
    BIGINT, -- targets (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE;

--    ONE TO MANY
CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    TEXT,   -- edges_sql (required)
    BIGINT, -- sources (required)
    ANYARRAY,   -- target (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE;

--    MANY TO MANY
CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow(
    TEXT,   -- edges_sql (required)
    ANYARRAY, -- sources (required)
    ANYARRAY, -- targets (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], only_cost := false);
$BODY$
LANGUAGE SQL VOLATILE;

-- COMMENTS

COMMENT ON FUNCTION pgr_minCostMaxFlow(TEXT, BIGINT, BIGINT)
IS 'EXPERIMENTAL pgr_minCostMaxFlow--One to One--(edges_sql(id,source,target,cost[,reverse_cost]), from_vid, to_vid';
COMMENT ON FUNCTION pgr_minCostMaxFlow(TEXT, BIGINT, ANYARRAY)
IS 'EXPERIMENTAL pgr_minCostMaxFlow--One to Many--(edges_sql(id,source,target,cost[,reverse_cost]), from_vid, to_vids';
COMMENT ON FUNCTION pgr_minCostMaxFlow(TEXT, ANYARRAY, BIGINT)
IS 'EXPERIMENTAL pgr_minCostMaxFlow--Many to One--(edges_sql(id,source,target,cost[,reverse_cost]), from_vids, to_vid';
COMMENT ON FUNCTION pgr_minCostMaxFlow(TEXT, ANYARRAY, ANYARRAY)
IS 'EXPERIMENTAL pgr_minCostMaxFlow--Many to Many--(edges_sql(id,source,target,cost[,reverse_cost]), from_vids, to_vids';
