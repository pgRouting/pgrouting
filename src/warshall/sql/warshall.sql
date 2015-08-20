/*PGR

Template:
Copyright (c) 2015 pgRouting developers

Function developer:
Copyright (c) 2013 Vicky Vergara
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

/* adjust to the function needs */
CREATE OR REPLACE FUNCTION _pgr_warshall(sql text, directed boolean, has_rcost boolean,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting-2.1', 'pgr_warshall'  -- must match name in .c file
    LANGUAGE c IMMUTABLE STRICT;

-- signature
-- sql: id, source, target, cost, reverse_cost
CREATE OR REPLACE FUNCTION pgr_warshall(sql text,  directed boolean default true,
  OUT seq integer, OUT source bigint, OUT target bigint, OUT cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('warshall', sql, true); -- must modify common/sql/pgr_parametercheck.sql
      return query SELECT * 
         FROM _pgr_warshall(sql, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


