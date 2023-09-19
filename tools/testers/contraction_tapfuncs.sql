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
CREATE OR REPLACE FUNCTION test_ids_size(pow INTEGER, directed BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
vids BIGINT[] = ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18];
x BIGINT;
q TEXT;
mainq TEXT;
expected TEXT;
BEGIN
  UPDATE edges
  SET id = id + POWER(10, pow), source = source + POWER(10, pow), target = target + POWER(10, pow);

  FOR i IN 1..18 LOOP
    vids[i] := POWER(10, pow) + vids[i];
  END LOOP;

  mainq = $d$ SELECT * FROM pgr_contraction(
      $$SELECT id, source, target, cost, reverse_cost FROM edges WHERE id  = ANY('%1$s'::BIGINT[]) $$,
      ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], '%2$s'::BOOLEAN)$d$;

  q = format(mainq, ARRAY[vids[5]], directed);

  RETURN QUERY
  SELECT set_eq(q,
    $$SELECT
    'v'::CHAR AS type,
    $$ || vids[11] || $$::BIGINT AS id,
    ARRAY[ $$ || vids[10] || $$]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

  q = format(mainq, ARRAY[vids[2],vids[4],vids[5],vids[8]], directed);

  RETURN QUERY
  SELECT is_empty(q);

  q = format(mainq, ARRAY[vids[1]], directed);

  RETURN QUERY
  SELECT set_eq(q,
    $$SELECT
    'v'::CHAR AS type,
    $$ || vids[6] || $$::BIGINT AS id,
    ARRAY[ $$ || vids[5] || $$]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

  q = format(mainq, ARRAY[vids[1], vids[2]], directed);

  RETURN QUERY
  SELECT set_eq(q,
    $$SELECT
    'v'::CHAR AS type,
     $$ || vids[10] || $$::BIGINT AS id,
    ARRAY[ $$ || vids[5] || $$,$$ ||  vids[6] || $$]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

  q = format(mainq, ARRAY[vids[1], vids[2], vids[3]], directed);

  expected = format($d$
    SELECT type, id, contracted_vertices, source, target, cost
    FROM (VALUES
      ('v'::CHAR, %1$s::BIGINT, '%2$s'::BIGINT[], -1::BIGINT, -1::BIGINT, -1::FLOAT)
    ) AS t(type, id, contracted_vertices, source, target, cost )$d$,
    vids[6], ARRAY[vids[5], vids[10], vids[15]]) ;

  RETURN QUERY
  SELECT set_eq(q, expected);

  q = format($d$ SELECT * FROM pgr_contraction(
      $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
      ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], '%1$s'::BOOLEAN)$d$, directed);

  expected = format($d$
    SELECT type, id, contracted_vertices, source, target, cost
    FROM (VALUES
      ('v'::CHAR, %1$s::BIGINT, '%2$s'::BIGINT[], -1::BIGINT, -1::BIGINT, -1::FLOAT),
      ('v', %3$s, '%4$s'::BIGINT[], -1, -1, -1),
      ('v', %5$s, '%6$s'::BIGINT[], -1, -1, -1),
      ('v', %7$s, '%8$s'::BIGINT[], -1, -1, -1),
      ('v', %9$s, '%10$s'::BIGINT[], -1, -1, -1)
    ) AS t(type, id, contracted_vertices, source, target, cost )$d$,
    vids[4], ARRAY[vids[2]],
    vids[6], ARRAY[vids[5]],
    vids[7], ARRAY[vids[1], vids[3]],
    vids[8], ARRAY[vids[9]],
    vids[14], ARRAY[vids[13]]);

    RETURN QUERY
    SELECT set_eq(q, expected);

    UPDATE edges
    SET id = id - POWER(10, pow), source = source - POWER(10, pow), target = target - POWER(10, pow);
END;
$BODY$
LANGUAGE plpgSQL;
