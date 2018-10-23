/*PGR-GNU*****************************************************************
File: kruskalDD.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh28@gmail.com

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

CREATE OR REPLACE FUNCTION pgr_kruskalDD (
    TEXT,   -- Edge sql
    BIGINT, -- root vertex
    FLOAT,  -- distance

    OUT seq INTEGER,
    OUT from_v BIGINT,
    OUT depth BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT seq, from_v, depth, node, edge, cost, agg_cost
    FROM _pgr_kruskal(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], 'DFS', 0, $3);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


CREATE OR REPLACE FUNCTION pgr_kruskalDD (
    TEXT,   -- Edge sql
    ANYARRAY, -- root vertex

    FLOAT, -- distance

    OUT seq INTEGER,
    OUT from_v BIGINT,
    OUT depth BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), $2, 'DFS', 0, $3);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

COMMENT ON FUNCTION pgr_kruskalDD(TEXT, BIGINT, FLOAT) IS 'pgr_kruskalDD(Single vertex): Experimental, Undirected Graph';
COMMENT ON FUNCTION pgr_kruskalDD(TEXT, ANYARRAY, FLOAT) IS 'pgr_kruskalDD(Multiple vertices): Experimental, Undirected Graph';
