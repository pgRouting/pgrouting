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


CREATE OR REPLACE FUNCTION _pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting', 'shortest_path'
    LANGUAGE c IMMUTABLE STRICT;


-- V2 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean)
  RETURNS SETOF pgr_costresult AS
  $BODY$
  DECLARE
  has_reverse boolean;
  BEGIN
      -- raise notice 'This function signature will no longer be supported in V3';
      has_reverse =_pgr_parameter_check('dijkstra', sql, false);
      if (has_reverse != has_rcost) then
         if (has_reverse) then -- raise NOTICE 'has_rcost set to false but reverse_cost column found, Ignoring';
         else raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
         end if;
      end if;

      return query SELECT seq, node::integer as id1, edge::integer as id2, cost
                FROM _pgr_dijkstra(sql, source_id, target_id, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- V3 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      -- raise notice 'This function signature belongs to  V3';
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      return query SELECT * -- seq, id1, id2, cost
         FROM _pgr_dijkstra(sql, source_id, target_id, true, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- V3 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      -- raise notice 'This function signature belongs to  V3';
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      return query SELECT * --seq, id1, id2, cost
         FROM _pgr_dijkstra(sql, source_id, target_id, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;
