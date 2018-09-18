/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com

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

-- V3 signature 1 to 1
CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,

    OUT seq integer,
    OUT path_seq integer,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost float,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], true, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- V3 signature 1 to 1
CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    directed BOOLEAN,

    OUT seq integer,
    OUT path_seq integer,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost float,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;



CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vid BIGINT,
    end_vids ANYARRAY,
    directed BOOLEAN DEFAULT true,

    OUT seq integer,
    OUT path_seq integer,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost float,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $4, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;




CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vids ANYARRAY,
    end_vid BIGINT,
    directed BOOLEAN DEFAULT true,
    OUT seq integer,
    OUT path_seq integer,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost float,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.start_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $4, false, false) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;




CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vids ANYARRAY,
    end_vids ANYARRAY,
    directed BOOLEAN DEFAULT true,
    OUT seq integer, OUT path_seq integer,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost float,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.start_vid, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], $4, false, true) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION  pgr_dijkstra(TEXT, BIGINT, BIGINT) IS 'pgr_dijkstra(One to One)';
COMMENT ON FUNCTION  pgr_dijkstra(TEXT, BIGINT, BIGINT, BOOLEAN) IS 'pgr_dijkstra(One to One)';
COMMENT ON FUNCTION  pgr_dijkstra(TEXT, BIGINT, ANYARRAY, BOOLEAN) IS 'pgr_dijkstra(One to Many)';
COMMENT ON FUNCTION  pgr_dijkstra(TEXT, ANYARRAY, BIGINT, BOOLEAN) IS 'pgr_dijkstra(Many to One)';
COMMENT ON FUNCTION  pgr_dijkstra(TEXT, ANYARRAY, ANYARRAY, BOOLEAN) IS 'pgr_dijkstra(Many to Many)';
