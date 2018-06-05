/*PGR-GNU*****************************************************************
File: minCostMaxFlow_Cost.sql

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

CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow_Cost(
    edges_sql TEXT,                 -- edges_sql
    sources ANYARRAY,               -- sources
    targets ANYARRAY)               -- targets
  RETURNS FLOAT AS
  $BODY$
        SELECT cost
        FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], only_cost := true);
  $BODY$
  LANGUAGE SQL VOLATILE;

------------------------
--    ONE TO MANY
------------------------

CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow_Cost(
    edges_sql TEXT,                 -- edges_sql
    sources BIGINT,                 -- source
    targets ANYARRAY)               -- targets
  RETURNS FLOAT AS
  $BODY$
        SELECT cost
        FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], only_cost := true);
  $BODY$
  LANGUAGE SQL VOLATILE;

------------------------
--    MANY TO ONE 
------------------------

CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow_Cost(
    edges_sql TEXT,                 -- edges_sql
    sources ANYARRAY,               -- sources
    targets BIGINT)                 -- target
  RETURNS FLOAT AS
  $BODY$
        SELECT cost
        FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := true);
  $BODY$
  LANGUAGE SQL VOLATILE;

------------------------
--    ONE TO ONE 
------------------------

CREATE OR REPLACE FUNCTION pgr_minCostMaxFlow_Cost(
    edges_sql TEXT,                 -- edges_sql
    sources BIGINT,                 -- source
    targets BIGINT)                 -- target
  RETURNS FLOAT AS
  $BODY$
        SELECT cost
        FROM _pgr_minCostMaxFlow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], only_cost := true);
  $BODY$
  LANGUAGE SQL VOLATILE;

