/*PGR-GNU*****************************************************************
File: bellman_ford.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

--MANY TO MANY

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    TEXT,
    ANYARRAY,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,
    
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_bellman_ford(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], directed, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;



--ONE TO ONE

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    TEXT,
    BIGINT,
    BIGINT,
    directed BOOLEAN DEFAULT true,
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT * FROM
     _pgr_bellman_ford(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed, false);
$BODY$
LANGUAGE SQL VOLATILE;



--ONE TO MANY

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    TEXT,
    BIGINT,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,

    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT * FROM
     _pgr_bellman_ford(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], directed, false);
$BODY$
LANGUAGE SQL VOLATILE;



--MANY TO ONE

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    TEXT,
    ANYARRAY,
    BIGINT,
    directed BOOLEAN DEFAULT true,
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT * FROM
     _pgr_bellman_ford(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], directed, false);
$BODY$
LANGUAGE SQL VOLATILE;