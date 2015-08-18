/*PGR

Template:
Copyright (c) 2015 pgRouting developers

Function developer:
Copyright (c) <year> <name>
<email>

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

/* adjust to the function needs */
CREATE OR REPLACE FUNCTION _pgr_function1(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting-2.1', 'pgr_function1_1'  -- must match name in .c file
    LANGUAGE c IMMUTABLE STRICT;

/* overloading if needed */
CREATE OR REPLACE FUNCTION _pgr_function1(sql text, source_id bigint, target_id anyarray, directed boolean, has_rcost boolean,
  OUT seq integer, OUT end_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting-2.1', 'pgr_function1_2'  -- must match name in .c file
    LANGUAGE c IMMUTABLE STRICT;

-- unfortunately while moving to V3 delete if its new function
-- V2 signature
CREATE OR REPLACE FUNCTION pgr_function1(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean)
  RETURNS SETOF pgr_costresult AS
  $BODY$
  DECLARE
  has_reverse boolean;
  BEGIN
      -- raise notice 'This function signature will no longer be supported in V3';
      has_reverse =_pgr_parameter_check('function1', sql, false);    -- must modify common/sql/pgr_parametercheck.sql
      if (has_reverse != has_rcost) then
         if (has_reverse) then -- raise NOTICE 'has_rcost set to false but reverse_cost column found, Ignoring';
         else raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
         end if;
      end if;

      return query SELECT seq-1 as seq, node::integer as id1, edge::integer as id2, cost
                FROM _pgr_function1(sql, source_id, target_id, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- signature
CREATE OR REPLACE FUNCTION pgr_function1(sql text, source_id bigint, target_id bigint,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('function1', sql, true); -- must modify common/sql/pgr_parametercheck.sql
      return query SELECT * 
         FROM _pgr_function1(sql, source_id, target_id, true, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- for overloading
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('function1', sql, true);
      return query SELECT *
         FROM _pgr_function1(sql, source_id, target_id, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

