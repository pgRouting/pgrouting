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


-------------------------------
--MANY TO MANY (NEGATIVE EDGES)
-------------------------------

CREATE OR REPLACE FUNCTION pgr_bellmanFord(
    TEXT,
    TEXT,
    ANYARRAY,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,
    
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.start_vid, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_bellmanFordNeg(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], $4::BIGINT[], directed, false ) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT;



--ONE TO ONE

CREATE OR REPLACE FUNCTION pgr_bellmanFord(
    TEXT,
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
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost 
    FROM _pgr_bellmanFordNeg(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], ARRAY[$4]::BIGINT[], directed, false) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;



--ONE TO MANY

CREATE OR REPLACE FUNCTION pgr_bellmanFord(
    TEXT,
    TEXT,
    BIGINT,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,
    
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.end_vid, a.node, a.edge, a.cost, a.agg_cost 
    FROM _pgr_bellmanFordNeg(_pgr_get_statement($1), _pgr_get_statement($2), ARRAY[$3]::BIGINT[], $4::BIGINT[], directed, false) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;



--MANY TO ONE

CREATE OR REPLACE FUNCTION pgr_bellmanFord(
    TEXT,
    TEXT,
    ANYARRAY,
    BIGINT,
    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.start_vid, a.node, a.edge, a.cost, a.agg_cost 
    FROM _pgr_bellmanFordNeg(_pgr_get_statement($1), _pgr_get_statement($2), $3::BIGINT[], ARRAY[$4]::BIGINT[], directed, false) AS a;
$BODY$
LANGUAGE SQL VOLATILE STRICT;