/*PGR-GNU*****************************************************************
File: bellman_ford.sql

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

--MANY TO MANY

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    edges_sql TEXT,
    start_vids ANYARRAY,
    end_vids ANYARRAY,
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'bellman_ford'
LANGUAGE c IMMUTABLE STRICT;



--ONE TO ONE

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    edges_sql TEXT,
    sources BIGINT,
    targets BIGINT,
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT * FROM
     pgr_bellman_ford(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed, only_cost);
$BODY$
LANGUAGE SQL VOLATILE;



--ONE TO MANY

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    edges_sql TEXT,
    sources BIGINT,
    targets ANYARRAY,
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT * FROM
     pgr_bellman_ford(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], directed, only_cost);
$BODY$
LANGUAGE SQL VOLATILE;



--MANY TO ONE

CREATE OR REPLACE FUNCTION pgr_bellman_ford(
    edges_sql TEXT,
    sources ANYARRAY,
    targets BIGINT,
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT * FROM
     pgr_bellman_ford(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], directed, only_cost);
$BODY$
LANGUAGE SQL VOLATILE;