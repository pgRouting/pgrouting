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


CREATE OR REPLACE FUNCTION _pgr_ksp(sql text, source_id bigint, target_id bigint, no_paths integer, has_reverse_cost boolean, directed boolean,
  OUT seq integer, OUT route bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
    '$libdir/librouting-2.1', 'kshortest_path'
    LANGUAGE c STABLE STRICT;


-- V2 the graph is directed and there are no heap paths 
CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id integer, target_id integer, no_paths integer, has_rcost boolean)
  RETURNS SETOF pgr_costresult3 AS
  $BODY$  
  DECLARE
  has_reverse boolean;
  BEGIN
      has_reverse =_pgr_parameter_check('ksp', sql::text, false);

      if (has_reverse != has_rcost) then 
         if (has_reverse) then -- raise NOTICE 'has_reverse_cost set to false but reverse_cost column found, Ignoring';
         else raise EXCEPTION 'has_reverse_cost set to true but reverse_cost not found';
         end if;
      end if;

      return query SELECT seq, route::integer, node::integer, edge::integer, cost 
            FROM _pgr_ksp(sql::text, source_id, target_id, no_paths, has_reverse, true) where route < no_paths;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

--V3 DEFAULTS directed:=true heap_paths:=false
CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id bigint, target_id bigint, no_paths integer,
  directed boolean default true, heap_paths boolean default false,
  OUT seq integer, OUT route bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('ksp', sql::text, true);
      if heap_paths = false then
         return query SELECT *
                FROM _pgr_ksp(sql::text, source_id, target_id, no_paths, has_rcost, directed) a where a.route < no_paths;
      else
         return query SELECT *
                FROM _pgr_ksp(sql::text, source_id, target_id, no_paths, has_rcost, directed);
      end if;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


