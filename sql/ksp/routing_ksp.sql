/*PGR-GNU*****************************************************************
File: routing_ksp.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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


CREATE OR REPLACE FUNCTION _pgr_ksp(edges_sql text, start_vid bigint, end_vid bigint, k integer, directed boolean, heap_paths boolean,
  OUT seq integer, OUT path_id integer, OUT path_seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
    '${MODULE_PATHNAME}', 'kshortest_path'
    LANGUAGE c STABLE STRICT;

-- V2 the graph is directed and there are no heap paths
CREATE OR REPLACE FUNCTION pgr_ksp(edges_sql text, start_vid integer, end_vid integer, k integer, has_rcost boolean)
  RETURNS SETOF pgr_costresult3 AS
  $BODY$
  DECLARE
  has_reverse boolean;
  sql TEXT;
  BEGIN
      RAISE NOTICE 'Deprecated signature of pgr_ksp';
      has_reverse =_pgr_parameter_check('dijkstra', edges_sql::text, false);
      sql = edges_sql;
      IF (has_reverse != has_rcost) THEN
         IF (has_rcost) THEN
           -- user says that it has reverse_cost but its not true
           RAISE EXCEPTION 'has_reverse_cost set to true but reverse_cost not found';
         ELSE
           -- user says that it does not have reverse_cost but it does have it
           -- to ignore we remove reverse_cost from the query
           sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a';
         END IF;
      END IF;

      RETURN query SELECT ((row_number() over()) -1)::integer  AS seq,  (path_id - 1)::integer AS id1, node::integer AS id2, edge::integer AS id3, cost
            FROM _pgr_ksp(sql::text, start_vid, end_vid, k, TRUE, FALSE) WHERE path_id <= k;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT
  COST 100
  ROWS 1000;


CREATE OR REPLACE FUNCTION pgr_ksp(edges_sql text, start_vid bigint, end_vid bigint, k integer,
  directed boolean default true, heap_paths boolean default false,
  --directed boolean, heap_paths boolean,
  OUT seq integer, OUT path_id integer, OUT path_seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  BEGIN
         RETURN query SELECT *
                FROM _pgr_ksp(edges_sql::text, start_vid, end_vid, k, directed, heap_paths);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

