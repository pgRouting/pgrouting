/*PGR-GNU*****************************************************************
File: kruskal.sql

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

CREATE OR REPLACE FUNCTION pgr_kruskal(
    TEXT,             -- Edge sql

    get_component BOOLEAN DEFAULT false,
    order_by INTEGER DEFAULT 0,

    OUT seq INTEGER,
    OUT component BIGINT,
    OUT depth BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)


RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), 0, $2, $3, false);
$BODY$
LANGUAGE SQL VOLATILE STRICT;




CREATE OR REPLACE FUNCTION pgr_kruskal(
    TEXT,        -- Edge sql
    BIGINT, -- root vertex

    get_component BOOLEAN DEFAULT false,
    order_by INTEGER DEFAULT 1,

    OUT seq INTEGER,
    OUT component BIGINT,
    OUT depth BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)


RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_kruskal(_pgr_get_statement($1), $2, $3, $4, true);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


COMMENT ON FUNCTION pgr_kruskal(TEXT, BOOLEAN, INTEGER) IS 'pgr_kruskal(edge_sql): Experimental, Undirected Graph';
COMMENT ON FUNCTION pgr_kruskal(TEXT, BIGINT, BOOLEAN, INTEGER) IS 'pgr_kruskal(edge_sql, root): Experimental, Undirected Graph';
