/*PGR-GNU*****************************************************************
File: dagShortestPath.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
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
CREATE OR REPLACE FUNCTION pgr_dagShortestPath(
    TEXT,
    ANYARRAY,
    ANYARRAY,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dagShortestPath(
    TEXT,
    ANYARRAY,
    BIGINT,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dagShortestPath(
    TEXT,
    BIGINT,
    ANYARRAY,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;



CREATE OR REPLACE FUNCTION pgr_dagShortestPath(
    TEXT,
    BIGINT,
    BIGINT,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dagShortestPath(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], true, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;
