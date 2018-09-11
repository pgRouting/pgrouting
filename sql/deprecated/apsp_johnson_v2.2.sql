/*PGR-GNU*****************************************************************

File: apsp_johnson.sql

Template:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function developer:
Copyright (c) 2013 Vicky Vergara
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


CREATE OR REPLACE FUNCTION pgr_apspJohnson(edges_sql text)
    RETURNS SETOF pgr_costResult AS
  $BODY$
  DECLARE
  has_reverse boolean;
  sql TEXT;
  BEGIN
      RAISE NOTICE 'Deprecated function: Use pgr_johnson instead';
      has_reverse =_pgr_parameter_check('johnson', edges_sql, false);
      sql = edges_sql;
      IF (has_reverse) THEN
           RAISE NOTICE 'reverse_cost column found, removing.';
           sql = 'SELECT source, target, cost FROM (' || edges_sql || ') a';
      END IF;

      RETURN query
         SELECT (row_number() over () - 1)::integer as seq, start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
         FROM  pgr_johnson(sql, TRUE);
  END
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT
  COST 100
  ROWS 1000;


