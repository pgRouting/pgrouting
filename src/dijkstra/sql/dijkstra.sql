/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/


CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql text, start_vid bigint, end_vid bigint, directed boolean,
    only_cost boolean default false,
  OUT seq integer, OUT path_seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'one_to_one_dijkstra'
    LANGUAGE c IMMUTABLE STRICT;

    -- One to many

CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql text, start_vid bigint, end_vids_sql TEXT, directed boolean default true,
    only_cost boolean default false,
  OUT seq integer, OUT path_seq integer, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'one_to_many_dijkstra_sql'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql text, start_vid bigint, end_vids anyarray, directed boolean default true,
    only_cost boolean default false,
  OUT seq integer, OUT path_seq integer, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'one_to_many_dijkstra'
    LANGUAGE c IMMUTABLE STRICT;


--  many to one

CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql text, start_vids_sql TEXT, end_vid bigint, directed boolean default true,
    only_cost boolean default false,
    OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'many_to_one_dijkstra_sql'
LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql text, start_vids anyarray, end_vid bigint, directed boolean default true,
    only_cost boolean default false,
    OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'many_to_one_dijkstra'
LANGUAGE c IMMUTABLE STRICT;

--  many to many

CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql text, start_vids_sql TEXT, end_vids_sql TEXT, directed boolean default true,
    only_cost boolean default false,
    OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'many_to_many_dijkstra_sql'
LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql text, start_vids anyarray, end_vids anyarray, directed boolean default true,
    only_cost boolean default false,
    OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'many_to_many_dijkstra'
LANGUAGE c IMMUTABLE STRICT;





-- V2 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vid bigint, end_vid bigint, directed boolean, has_rcost boolean)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse boolean;
sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated function';
    has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);
    sql = edges_sql;
    if (has_reverse != has_rcost) then
        if (has_reverse) then
            sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a'; 
        else raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        end if;
    end if;

    return query SELECT seq-1 as seq, node::integer as id1, edge::integer as id2, cost
    FROM _pgr_dijkstra(sql, start_vid, end_vid, directed, false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- V3 signature 1 to 1
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vid bigint, end_vid bigint,
    OUT seq integer,  OUT path_seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
BEGIN
    RETURN query 
    SELECT * FROM _pgr_dijkstra(edges_sql, start_vid, end_vid, true, false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- V3 signature 1 to 1
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vid bigint, end_vid bigint, directed boolean,
    OUT seq integer,  OUT path_seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
BEGIN
    return query SELECT *
    FROM _pgr_dijkstra(edges_sql, start_vid, end_vid, directed, false) a;
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;






--     DIJKSTRA 

/***********************************
        ONE TO MANY
***********************************/
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vid bigint, end_vids TEXT, directed boolean default true,
  OUT seq integer, OUT path_seq integer, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra($1, $2, $3, $4, false) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vid bigint, end_vids anyarray, directed boolean default true,
  OUT seq integer, OUT path_seq integer, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra($1, $2, $3, $4, false) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



/***********************************
        MANY TO ONE
***********************************/
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vids TEXT, end_vid BIGINT, directed boolean default true,
  OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra($1, $2, $3, $4, false) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vids ANYARRAY, end_vid BIGINT, directed boolean default true,
  OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra($1, $2, $3, $4, false) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



/***********************************
        MANY TO MANY
***********************************/
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vids TEXT, end_vids TEXT, directed boolean default true,
    OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra($1, $2, $3, $4, false) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql text, start_vids ANYARRAY, end_vids ANYARRAY, directed boolean default true,
    OUT seq integer, OUT path_seq integer, OUT start_vid bigint, OUT end_vid bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra($1, $2, $3, $4, false) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;










--     DIJKSTRA COST

/***********************************
        ONE TO ONE
***********************************/
CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql text, BIGINT, BIGINT, directed boolean,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    return query SELECT $2 as start_vid, $3 as end_vid, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


/***********************************
        ONE TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql text, BIGINT, end_vids_sql TEXT, directed boolean default true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT $2 , a.end_vid, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql text, BIGINT, end_vids anyarray, directed boolean default true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT $2, a.end_vid, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


/***********************************
        MANY TO ONE
***********************************/
CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql text, start_vids TEXT, BIGINT, directed boolean default true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT a.start_vid, $3, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql text, start_vids ANYARRAY, BIGINT, directed boolean default true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT a.start_vid, $3, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



/***********************************
        MANY TO MANY
***********************************/
CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql text, start_vids TEXT, end_vids TEXT, directed boolean default true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT a.start_vid, a.end_vid, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_dijkstraCost(edges_sql text, start_vids ANYARRAY, end_vids ANYARRAY, directed boolean default true,
    OUT start_vid BIGINT, OUT end_vid BIGINT, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT a.start_vid, a.end_vid, a.agg_cost
    FROM _pgr_dijkstra($1, $2, $3, $4, true) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



