/*PGR-GNU*****************************************************************
File: trsp.sql

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

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
-- trsp: one to one
-------------------------------
CREATE OR REPLACE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    BIGINT, -- start_vid
    BIGINT, -- end_vid
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            ARRAY[$3]::BIGINT[],
            ARRAY[$4]::BIGINT[],
            directed) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-------------------------------
-- trsp: one to many
-------------------------------
CREATE OR REPLACE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    BIGINT, -- start_vid
    ANYARRAY, -- end_vids
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            ARRAY[$3]::BIGINT[],
            $4::bigint[],
            directed) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-------------------------------
-- trsp: many to one
-------------------------------
CREATE OR REPLACE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    ANYARRAY, -- start_vids
    BIGINT, -- end_vid
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            $3::bigint[],
            ARRAY[$4]::BIGINT[],
            $5) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-------------------------------
-- trsp: many to many
-------------------------------
CREATE OR REPLACE FUNCTION _pgr_trsp(
    TEXT, -- edges_sql
    TEXT, -- restrictions_sql
    ANYARRAY, -- start_vids
    ANYARRAY, -- end_vids
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
        FROM _trsp(
            _pgr_get_statement($1),
            _pgr_get_statement($2),
            $3::bigint[],
            $4::bigint[],
            $5) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

