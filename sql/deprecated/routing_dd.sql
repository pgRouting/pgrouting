/*PGR-GNU*****************************************************************

Copyright (c) 2017 Celia Virginia Vergara Castillo
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



-- OLD SIGNATURE
CREATE OR REPLACE FUNCTION pgr_drivingDistance(edges_sql text, source BIGINT, distance FLOAT, directed BOOLEAN, has_rcost BOOLEAN)
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
             -- can't do anything
             RAISE EXCEPTION 'has_rcost set to true but reverse_cost not found';
         END IF;
      END IF;

      RETURN query SELECT seq - 1 AS seq, node::integer AS id1, edge::integer AS id2, agg_cost AS cost
                FROM pgr_drivingDistance($1, ARRAY[$2]::BIGINT[], $3, $4, false);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT
  COST 100
  ROWS 1000;

