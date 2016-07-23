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


CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql TEXT, start_vid BIGINT, end_vid BIGINT, directed BOOLEAN,
    only_cost BOOLEAN DEFAULT false,
  OUT seq integer, OUT path_seq integer, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'one_to_one_dijkstra'
    LANGUAGE c IMMUTABLE STRICT;

    -- One to many


CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql TEXT, start_vid BIGINT, end_vids ANYARRAY, directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
  OUT seq integer, OUT path_seq integer, OUT end_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'one_to_many_dijkstra'
    LANGUAGE c IMMUTABLE STRICT;


--  many to one


CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql TEXT, start_vids ANYARRAY, end_vid BIGINT, directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
    OUT seq integer, OUT path_seq integer, OUT start_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'many_to_one_dijkstra'
LANGUAGE c IMMUTABLE STRICT;

--  many to many


CREATE OR REPLACE FUNCTION _pgr_dijkstra(edges_sql TEXT, start_vids ANYARRAY, end_vids ANYARRAY, directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
    OUT seq integer, OUT path_seq integer, OUT start_vid BIGINT, OUT end_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'many_to_many_dijkstra'
LANGUAGE c IMMUTABLE STRICT;





-- V2 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql TEXT, start_vid INTEGER, end_vid INTEGER, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated function';
    has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);
    sql = edges_sql;
    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    RETURN query SELECT seq-1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM _pgr_dijkstra(sql, start_vid, end_vid, directed, false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- V3 signature 1 to 1
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql TEXT, start_vid BIGINT, end_vid BIGINT,
    OUT seq integer,  OUT path_seq integer, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
 statement_txt record;
 sql TEXT;
BEGIN
    RETURN query
    SELECT * FROM _pgr_dijkstra(_pgr_get_statement($1), start_vid, end_vid, true, false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- V3 signature 1 to 1
CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql TEXT, start_vid BIGINT, end_vid BIGINT, directed BOOLEAN,
    OUT seq integer,  OUT path_seq integer, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra(_pgr_get_statement($1), start_vid, end_vid, directed, false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;






--     DIJKSTRA

/* ********** FOR USERS DOCUMENTATION

pgr_dijkstra_parameters_start

Description of the parameters of the signatures
...............................................................................

============== ================== ====== =================================================
Column         Type               Default     Description
============== ================== ====== =================================================
**sql**        ``TEXT``                  SQL query as decribed above.
**start_vid**  ``BIGINT``                Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[BIGINT]``         Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``                Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[BIGINT]``         Array of identifiers of ending vertices.
**directed**   ``BOOLEAN``         true  When ``true`` Graph is considered `Directed`
                                         When ``false`` the graph is considered as Undirected.
============== ================== ====== =================================================

pgr_dijkstra_parameters_end

*/

/***********************************
        ONE TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql TEXT, start_vid BIGINT, end_vids ANYARRAY, directed BOOLEAN DEFAULT true,
  OUT seq integer, OUT path_seq integer, OUT end_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2, $3, $4);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



/***********************************
        MANY TO ONE
***********************************/

CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql TEXT, start_vids ANYARRAY, end_vid BIGINT, directed BOOLEAN DEFAULT true,
  OUT seq integer, OUT path_seq integer, OUT start_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2, $3, $4);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



/***********************************
        MANY TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_dijkstra(edges_sql TEXT, start_vids ANYARRAY, end_vids ANYARRAY, directed BOOLEAN DEFAULT true,
    OUT seq integer, OUT path_seq integer, OUT start_vid BIGINT, OUT end_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost float, OUT agg_cost float)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN query SELECT *
    FROM _pgr_dijkstra(_pgr_get_statement($1), $2, $3, $4);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
