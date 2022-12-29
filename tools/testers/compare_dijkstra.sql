
/*PGR-GNU*****************************************************************

Copyright (c) 2022  Vicky Vergara
Mail: vicky_vergara at hotmail.com

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


CREATE OR REPLACE FUNCTION compare_dijkstra_one_one(
  fn TEXT, directed BOOLEAN,
  ret_c TEXT default '*',
  where_c TEXT default '',
  restricted BOOLEAN default false,
  cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
restricted_sql TEXT := '';
k TEXT := '';
fn_sql TEXT;
BEGIN

  IF (restricted) THEN
    restricted_sql := '$$SELECT * FROM restrictions WHERE id > 10$$, ';
    k := ', 2';
  END IF;


  FOR i IN 1.. cant LOOP
    FOR j IN 1.. cant LOOP

      -- with reverse cost
      inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
      dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
        || ', directed => ' || directed || ') ' || where_c;

      fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$' || inner_sql || '$$, ' || restricted_sql || i || ', ' || j || k
        || ', directed => ' || directed || ') ' || where_c;
      RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);


      -- no reverse cost
      inner_sql := 'SELECT id, source, target, cost FROM edge_table';
      dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
        || ', directed => ' || directed || ') ' || where_c;

      fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$' || inner_sql || '$$, ' || restricted_sql || i || ', ' || j || k
        || ', directed => ' || directed || ') ' || where_c;
      RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

      END LOOP;
  END LOOP;

END
$BODY$
language plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_many_one(
  fn TEXT, directed BOOLEAN,
  ret_c TEXT default '*',
  where_c TEXT default '',
  cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  inner_sql TEXT;
  dijkstra_sql TEXT;
  fn_sql TEXT;
  arr TEXT;
BEGIN

  FOR j IN 1.. cant LOOP
    arr := 'ARRAY[1,2,3,4,5,6,7,8]';

    -- with reverse cost
    inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
    dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || arr || ', ' || j || ', directed => ' || directed || ')' || where_c;
    fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$'        || inner_sql || '$$, ' || arr || ', ' || j || ', directed => ' || directed || ')' || where_c;
    RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

    -- no reverse cost
    inner_sql := 'SELECT id, source, target, cost FROM edge_table';
    dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || arr || ', ' || j || ', directed => ' || directed || ')' || where_c;
    fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$'        || inner_sql || '$$, ' || arr || ', ' || j || ', directed => ' || directed || ')' || where_c;
    RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

  END LOOP;
END
$BODY$
language plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_one_many(
  fn TEXT, directed BOOLEAN,
  ret_c TEXT default '*',
  where_c TEXT default '',
  cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  inner_sql TEXT;
  dijkstra_sql TEXT;
  fn_sql TEXT;
  arr TEXT;
BEGIN

  FOR j IN 1.. cant LOOP
    arr := 'ARRAY[1,2,3,4,5,6,7,8]';

    -- with reverse cost
    inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
    dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || j || ', ' || arr || ', directed => ' || directed || ') ' || where_c;
    fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$'        || inner_sql || '$$, ' || j || ', ' || arr || ', directed => ' || directed || ') ' || where_c;
    RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

    -- no reverse cost
    inner_sql := 'SELECT id, source, target, cost FROM edge_table';
    dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || j || ', ' || arr || ', directed => ' || directed || ') ' || where_c;
    fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$'        || inner_sql || '$$, ' || j || ', ' || arr || ', directed => ' || directed || ') ' || where_c;
    RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

  END LOOP;
END
$BODY$
language plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_many_many(
  fn TEXT, directed BOOLEAN,
  ret_c TEXT default '*',
  where_c TEXT default '')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  inner_sql TEXT;
  dijkstra_sql TEXT;
  fn_sql TEXT;
  arr1 TEXT;
  arr2 TEXT;
BEGIN

  arr1 := 'ARRAY[1,2,3,4,5,6,7,8]';
  arr2 := 'ARRAY[9,10,11,12,13,14,15]';

  -- with reverse cost
  inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
  dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || arr1 || ', ' || arr2 || ', directed => ' || directed || ') ' || where_c;
  fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$'        || inner_sql || '$$, ' || arr1 || ', ' || arr2 || ', directed => ' || directed || ') ' || where_c;
  RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

  -- no reverse cost
  inner_sql := 'SELECT id, source, target, cost FROM edge_table';
  dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || arr1 || ', ' || arr2 || ', directed => ' || directed || ') ' || where_c;
  fn_sql := 'SELECT ' || ret_c || ' FROM '|| fn || '($$'        || inner_sql || '$$, ' || arr1 || ', ' || arr2 || ', directed => ' || directed || ') ' || where_c;
  RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

END
$BODY$
language plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_combinations(
  fn TEXT, directed BOOLEAN,
  ret_c TEXT default '*',
  where_c TEXT default '')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  inner_sql TEXT;
  dijkstra_sql TEXT;
  fn_sql TEXT;
BEGIN

  -- with reverse cost
  inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
  dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, $$SELECT source, target FROM combinations_table$$, directed => ' || directed || ') ' || where_c;
  fn_sql := 'SELECT ' || ret_c || ' FROM ' || fn || '($$' || inner_sql || '$$, $$SELECT source, target FROM combinations_table$$, directed => ' || directed || ') ' || where_c;
  RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

  -- no reverse cost
  inner_sql := 'SELECT id, source, target, cost FROM edge_table';
  dijkstra_sql := 'SELECT ' || ret_c || ' FROM pgr_dijkstra($$' || inner_sql || '$$, $$SELECT source, target FROM combinations_table$$, directed => ' || directed || ') ' || where_c;
  fn_sql := 'SELECT ' || ret_c || ' FROM ' || fn || '($$' || inner_sql || '$$, $$SELECT source, target FROM combinations_table$$, directed => ' || directed || ') ' || where_c;
  RETURN query SELECT set_eq(fn_sql, dijkstra_sql, fn_sql);

END
$BODY$
language plpgsql;
