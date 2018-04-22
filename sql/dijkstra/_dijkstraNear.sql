/*PGR-GNU*****************************************************************

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Celia Virginia Vergara Castillo
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


CREATE OR REPLACE FUNCTION _pgr_dijkstraNear(
    TEXT,     -- edges_sql
    BIGINT,   -- source
    ANYARRAY, -- array of targets
    BIGINT,   -- stop at value should be >=1
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
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $5, false, true, $4) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;
COMMENT ON FUNCTION  _pgr_dijkstraNear(TEXT, BIGINT, ANYARRAY, BIGINT, BOOLEAN) IS '_pgr_dijkstra(1 to Many with limit of goals)';



CREATE OR REPLACE FUNCTION _pgr_dijkstraNear(
    TEXT,     -- edges_sql
    ANYARRAY, -- array of sources
    BIGINT,   -- target
    BIGINT,   -- stop at value should be >=1
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
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $5, false, false, $4) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;
COMMENT ON FUNCTION  _pgr_dijkstraNear(TEXT, ANYARRAY, BIGINT, BIGINT, BOOLEAN) IS '_pgr_dijkstraNear(Many to 1 with limit of goals)';

CREATE OR REPLACE FUNCTION _pgr_dijkstraNear(
    TEXT,     -- edges_sql
    ANYARRAY, -- array of sources
    ANYARRAY, -- array of targets
    BIGINT,   -- stop at value should be >=1
    directed BOOLEAN DEFAULT true,

    OUT seq integer,
    OUT path_seq integer,
    OUT end_vid BIGINT,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost float,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.path_seq, a.start_vid, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $5, false, false, $4) AS a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;
COMMENT ON FUNCTION  _pgr_dijkstraNear(TEXT, ANYARRAY, ANYARRAY, BIGINT, BOOLEAN) IS '_pgr_dijkstraNear(1 to Many with limit of goals)';

