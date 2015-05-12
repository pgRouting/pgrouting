--
-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
--               2015 Celia Vriginia Vergara Castillo.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
--


-----------------------------------------------------------------------
-- Core function for Dijkstra shortest_path computation
-----------------------------------------------------------------------
/*
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id integer, 
        target_id integer, directed boolean, has_reverse_cost boolean)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting', 'shortest_path'
        LANGUAGE c IMMUTABLE STRICT;
*/

CREATE OR REPLACE FUNCTION _pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean)
    RETURNS SETOF pgr_costResultBig
    AS '$libdir/librouting', 'shortest_path'
    LANGUAGE c IMMUTABLE STRICT;


-- invert the comments when pgRouting decides for bigints 
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean)
  --RETURNS SETOF pgr_costresultBig AS
  RETURNS SETOF pgr_costresult AS
  $BODY$
  DECLARE
  has_reverse boolean;
  BEGIN
      has_reverse =_pgr_parameter_check(sql);
      -- for backwards comptability uncomment latter if keeping the flag:

      if (has_reverse != has_rcost) then
         if (has_reverse) then raise NOTICE 'has_reverse_cost set to false but reverse_cost column found, Ignoring';
         else raise EXCEPTION 'has_reverse_cost set to true but reverse_cost not found';
         end if;
      end if;

      return query SELECT seq, id1::integer, id2::integer, cost
                FROM _pgr_dijkstra(sql, source_id, target_id, directed, has_reverse);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

/* invert the comments when pgRouting decides for bigints */
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint)
  --RETURNS SETOF pgr_costresultBig AS
  RETURNS SETOF pgr_costresult AS
  $BODY$
  DECLARE
  has_reverse boolean;
  BEGIN
         has_reverse =_pgr_parameter_check(sql);
         return query SELECT seq, id1::integer , id2::integer, cost FROM pgr_dijkstra(sql, source_id, target_id, false, has_reverse);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;
