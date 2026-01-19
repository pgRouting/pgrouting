/*PGR-GNU*****************************************************************

Copyright (c) 2023  pgRouting developers
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

CREATE OR REPLACE FUNCTION test_agg_cost(fn TEXT, directed BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
 q1 TEXT;
 q2 TEXT;
 q3 TEXT;
 q4 TEXT;

BEGIN

  q1 := format(
    $$ SELECT * FROM %1$s('SELECT id, source, target, cost, reverse_cost FROM edges',
      %2$L::BOOLEAN) WHERE agg_cost < 0
    $$, fn, directed);
  RETURN QUERY
  SELECT is_empty(q1, fn || ': c & rc, ' || directed || ': No cost can be negative');

  q2 := format($$ SELECT * FROM %1$s( 'SELECT id, source, target, cost FROM edges',
      %2$L::BOOLEAN) WHERE agg_cost < 0
    $$, fn, directed);
  RETURN QUERY
  SELECT is_empty(q2, fn || ': c, ' || directed || ': No cost can be negative');

  q3 := format($$ SELECT * FROM %1$s( 'SELECT id, source, target, cost, -1 as reverse_cost FROM edges',
      %2$L::BOOLEAN) WHERE agg_cost < 0
    $$, fn, directed);
  RETURN QUERY
  SELECT is_empty(q3, fn || ': c & -1, ' || directed || ': No cost can be negative');

  q4 := format($$ SELECT * FROM %1$s( 'SELECT id, source, target, -1 AS cost, reverse_cost FROM edges',
      %2$L::BOOLEAN) WHERE agg_cost < 0
    $$, fn, directed);
  RETURN QUERY
  SELECT is_empty(q4, fn || ' -1 & rc, ' || directed || ': No cost can be negative');

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION allPairs_compare_dijkstra(fn TEXT, directed BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
 q1 TEXT;
 e1 TEXT;

BEGIN

  q1 := format($$ SELECT start_vid, end_vid, round(agg_cost::NUMERIC,2) FROM %1$s( 'SELECT id, source, target, cost, reverse_cost FROM edges WHERE id = 1',
      %2$L::BOOLEAN)
    $$, fn, directed);
  e1 := format($$ SELECT start_vid, end_vid, round(agg_cost::NUMERIC,2) FROM pgr_dijkstraCostMatrix( 'SELECT id, source, target, cost, reverse_cost FROM edges',
      %2$L::BIGINT[], %3$L::BOOLEAN)
    $$, fn, ARRAY[5,6], directed);
  RETURN QUERY
  SELECT set_eq(q1, e1, fn || ': compare subgraph with 2 vertices');

  q1 := format($$ SELECT start_vid, end_vid, round(agg_cost::NUMERIC,2) FROM %1$s( 'SELECT id, source, target, cost, reverse_cost FROM edges',
      %2$L::BOOLEAN)
    $$, fn, directed);
  e1 := format($$
    SELECT start_vid, end_vid, round(agg_cost::NUMERIC,2) FROM pgr_dijkstraCostMatrix(
      'SELECT id, source, target, cost, reverse_cost FROM edges',
      %2$L::BIGINT[],
      %3$L::BOOLEAN)$$,
    fn, ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17], directed);
  RETURN QUERY
  SELECT set_eq(q1, e1, fn || ': compare complete graph');

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION allPairs_test_flags(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
 q1 TEXT;
 e1 TEXT;

BEGIN

  q1 := format($$ SELECT * FROM %1$s(
      'SELECT id, source, target, cost, reverse_cost FROM edges')
    $$, fn);
  RETURN QUERY
  SELECT lives_ok(q1, fn || ': lives with no flags');

  q1 := format($$ SELECT * FROM %1$s(
      'SELECT id, source, target, cost, reverse_cost FROM edges', FALSE)
    $$, fn);
  RETURN QUERY
  SELECT lives_ok(q1, fn || ': lives with 1 flag');

  q1 := format($$ SELECT * FROM %1$s(
      'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
      directed => FALSE)$$, fn);
  RETURN QUERY
  SELECT lives_ok(q1, fn || ': lives with named flag');

  q1 := format($$ SELECT * FROM %1$s(
      'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
      dir =>FALSE)$$, fn);
  RETURN QUERY
  SELECT throws_ok(q1,
    '42883','function ' || fn || '(unknown, dir => boolean) does not exist',
    fn || ': throws with invalid flags name');

  q1 := format($$ SELECT * FROM %1$s(
      'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
      FALSE, TRUE)$$, fn);
  RETURN QUERY
  SELECT throws_ok(q1,
    '42883','function ' || fn || '(unknown, boolean, boolean) does not exist',
    fn || ': throws with invalid flags name');

END;
$BODY$
LANGUAGE plpgsql;
