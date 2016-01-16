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


--     DIJKSTRA COST

/***********************************
        ONE TO ONE
***********************************/
CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql TEXT, BIGINT, BIGINT, directed BOOLEAN DEFAULT true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    return query SELECT $2, $3, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


/***********************************
        ONE TO MANY
***********************************/


CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql TEXT, BIGINT, end_vids ANYARRAY, directed BOOLEAN DEFAULT true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT $2, a.end_vid, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


/***********************************
        MANY TO ONE
***********************************/

CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql TEXT, start_vids ANYARRAY, BIGINT, directed BOOLEAN DEFAULT true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT a.start_vid, $3, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



/***********************************
        MANY TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql TEXT, start_vids ANYARRAY, end_vids ANYARRAY, directed BOOLEAN DEFAULT true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT a.start_vid, a.end_vid, a.agg_cost
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



