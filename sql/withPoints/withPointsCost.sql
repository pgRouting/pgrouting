/*PGR-GNU*****************************************************************
File: withPointsCost.sql

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

/*
ONE TO ONE
*/

CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    edges_sql TEXT,
    points_sql TEXT,
    BIGINT,
    BIGINT,
    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT $3, $4, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, ARRAY[$3]::BIGINT[], ARRAY[$4]::BIGINT[], $5, $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

/*
ONE TO MANY
*/

CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    edges_sql TEXT,
    points_sql TEXT,
    BIGINT,
    end_pids ANYARRAY,
    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT $3, a.end_pid, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, ARRAY[$3]::BIGINT[], $4::BIGINT[], $5, $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

/*
MANY TO ONE
*/

CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    edges_sql TEXT,
    points_sql TEXT,
    start_pids ANYARRAY,
    BIGINT,
    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.start_pid, $4, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, $3::BIGINT[], ARRAY[$4]::BIGINT[], $5, $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

/*
MANY TO MANY
*/

CREATE OR REPLACE FUNCTION pgr_withPointsCost(
    edges_sql TEXT,
    points_sql TEXT,
    start_pids ANYARRAY,
    end_pids ANYARRAY,
    directed BOOLEAN DEFAULT true,
    driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL

    OUT start_pid BIGINT,
    OUT end_pid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.start_pid, a.end_pid, a.agg_cost
    FROM _pgr_withPoints(_pgr_get_statement($1), $2, $3::BIGINT[], $4::BIGINT[], $5,  $6, TRUE, TRUE) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;
