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


--  BIDIRECTIONAL ASTAR Matrix


CREATE OR REPLACE FUNCTION pgr_bdAstarCostMatrix(
    edges_sql TEXT,
    vids ANYARRAY,
    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor NUMERIC DEFAULT 1.0,
    epsilon NUMERIC DEFAULT 1.0,

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.start_vid, a.end_vid, a.agg_cost
    FROM _pgr_bdAstar(_pgr_get_statement($1), $2::BIGINT[], $2::BIGINT[], $3, $4, $5::FLOAT, $6::FLOAT, true) a;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;
COMMENT ON FUNCTION pgr_bdAstarCostMatrix(TEXT, ANYARRAY, BOOLEAN, INTEGER, NUMERIC, NUMERIC) IS 'pgr_bdAstarCostMatrix';



