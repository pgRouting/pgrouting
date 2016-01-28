/*PGR-GNU*****************************************************************

Copyright (c) 2015 Celia Virginia Vergara Castillo
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


CREATE OR REPLACE FUNCTION _pgr_drivingDistance(edges_sql text, start_vid bigint, distance float8, directed BOOLEAN, 
       OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
     '$libdir/${PGROUTING_LIBRARY_NAME}', 'driving_distance'
 LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_drivingDistance(edges_sql text, start_vids anyarray, distance float8, directed BOOLEAN, equicost BOOLEAN, 
       OUT seq integer, OUT start_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
     '$libdir/${PGROUTING_LIBRARY_NAME}', 'driving_many_to_dist'
 LANGUAGE c IMMUTABLE STRICT;


-- OLD SIGNATURE
CREATE OR REPLACE FUNCTION pgr_drivingDistance(edges_sql text, source bigint, distance float8, directed BOOLEAN, has_rcost BOOLEAN)
  RETURNS SETOF pgr_costresult AS
  $BODY$
  DECLARE
  has_reverse BOOLEAN;
  sql TEXT;
  BEGIN
      RAISE NOTICE 'Deprecated function';

      has_reverse =_pgr_parameter_check('dijkstra', edges_sql, FALSE);

      sql = edges_sql;
      IF (has_reverse != has_rcost) THEN
         IF (has_reverse) THEN 
             -- the user says it doesn't have reverse cost but its false
             -- removing from query
             RAISE NOTICE 'Contradiction found: has_rcost set to false but reverse_cost column found';
             sql = 'SELECT id, source, target, cost, -1 as reverse_cost FROM (' || edges_sql || ') __q ';
         ELSE
             -- the user says it has reverse cost but its false
             -- cant do anything
             RAISE EXCEPTION 'has_rcost set to true but reverse_cost not found';
         END IF;
      END IF;

      RETURN query SELECT seq - 1 AS seq, node::integer AS id1, edge::integer AS id2, agg_cost AS cost
                FROM _pgr_drivingDistance(sql, source, distance, directed);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;



CREATE OR REPLACE FUNCTION pgr_drivingDistance(edges_sql text, start_v bigint, distance float8, directed BOOLEAN DEFAULT TRUE,
       OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  BEGIN
      RETURN query
          SELECT * FROM _pgr_drivingDistance(edges_sql, start_v, distance, directed);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- the multi starting point
CREATE OR REPLACE FUNCTION pgr_drivingDistance(sql text, start_v anyarray, distance float8, directed BOOLEAN DEFAULT TRUE, equicost BOOLEAN DEFAULT FALSE,
       OUT seq integer, OUT from_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  BEGIN
      RETURN query
           SELECT * FROM _pgr_drivingDistance(sql, start_v, distance, directed, equicost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;



