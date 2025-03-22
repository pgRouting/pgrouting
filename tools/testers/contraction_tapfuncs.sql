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

CREATE OR REPLACE PROCEDURE create_contracted_graph(directed BOOLEAN, fn TEXT, methods INTEGER[] DEFAULT NULL::INTEGER[]) AS
$BODY$

BEGIN

  IF NOT min_version('3.8.0') THEN RETURN; END IF;

  DELETE FROM edges WHERE id >= 19;
  ALTER SEQUENCE edges_id_seq RESTART WITH 19;
  UPDATE edges SET (is_new, contracted_vertices)=(false,NULL);
  UPDATE vertices SET (is_contracted, contracted_vertices)=(false,NULL);

  -- add extra columns to the edges and vertices table

  DROP TABLE IF EXISTS contraction_info;
  IF fn = 'pgr_contraction' THEN
    EXECUTE format($q$
    CREATE TABLE contraction_info AS
    SELECT * FROM %s(
      'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
      %s, '%s'::INTEGER[]);
    $q$, fn, directed::TEXT, methods);
  ELSE
    EXECUTE format($q$
    CREATE TABLE contraction_info AS
    SELECT * FROM %s(
      'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
      '%s');
    $q$, fn, directed::TEXT);
  END IF;

  -- add the new edges
  INSERT INTO edges(source, target, cost, reverse_cost, contracted_vertices, is_new)
  SELECT source, target, cost, -1, contracted_vertices, true
  FROM contraction_info
  WHERE type = 'e';

  -- Indicate vertices that were contracted
  UPDATE vertices
  SET is_contracted = true
  WHERE id IN (SELECT  unnest(contracted_vertices) FROM  contraction_info);

  -- add the contracted vertices on the vertices table
  UPDATE vertices
  SET contracted_vertices = contraction_info.contracted_vertices
  FROM contraction_info
  WHERE type = 'v' AND vertices.id = contraction_info.id;

  DROP VIEW IF EXISTS contracted_graph;
  CREATE VIEW contracted_graph AS
  SELECT id,source, target, cost, reverse_cost, contracted_vertices FROM edges
  WHERE
    EXISTS (SELECT id FROM vertices AS v WHERE NOT is_contracted AND v.id = edges.source)
    AND
    EXISTS (SELECT id FROM vertices AS v WHERE NOT is_contracted AND v.id = edges.target);

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION path_cost_contraction(BIGINT, BIGINT, BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE q TEXT;
BEGIN

    RETURN QUERY
      SELECT round(agg_cost::numeric, 3)::TEXT FROM pgr_dijkstra(
      'WITH cul_de_sac AS (
        SELECT contracted_vertices || id as v
        FROM vertices WHERE ' || $1 ||' = ANY(contracted_vertices) OR ' || $2 ||' = ANY(contracted_vertices)),
      linears_to_expand AS (
        SELECT id, contracted_vertices
        FROM edges WHERE is_new AND (' || $1 ||' = ANY(contracted_vertices) OR '|| $2 ||'  = ANY(contracted_vertices))
      ),
      verts AS (SELECT * FROM cul_de_sac UNION SELECT contracted_vertices FROM linears_to_expand)
      SELECT id, source, target, cost, reverse_cost FROM edges, verts WHERE source = ANY(v) OR target = ANY(v)

      UNION

      SELECT id, source, target, cost, reverse_cost FROM contracted_graph LEFT JOIN linears_to_expand c USING (id) WHERE c.id IS NULL',
      $1, $2, $3) WHERE edge = -1;

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_contraction(
    BIGINT, BIGINT, BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
    q TEXT;
BEGIN

  RETURN QUERY
  SELECT set_eq(
    $$SELECT * FROM path_cost_contraction($$|| $1 || $$,$$ || $2 || $$,$$ || $3::BOOLEAN || $$)$$,
    $$SELECT  round(agg_cost::numeric, 3)::TEXT
    FROM pgr_dijkstra(
      'SELECT id, source, target, cost, reverse_cost FROM edges WHERE id < 19',
      $$|| $1 || $$,$$ || $2 || $$,$$ || $3::BOOLEAN || $$) WHERE edge = -1
    $$,
    'From ' || $1 || ' to ' || $2 || 'Directed = ' || $3::BOOLEAN);

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_contraction(BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
    i INTEGER;
    j INTEGER;

BEGIN

  IF NOT min_version('3.8.0') THEN
    RETURN QUERY
    SELECT skip(1, 'contraction function/signature is new on 3.8.0');
    RETURN;
  END IF;

  FOR i IN 1..18 LOOP
    FOR j IN 1..18 LOOP
      RETURN QUERY
      SELECT compare_dijkstra_contraction(i, j, $1);
    END LOOP;
  END LOOP;

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION check_contracted_graph_deadend(BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_version('3.8.0') THEN
    RETURN QUERY SELECT skip(1, 'contraction function/signature is new on 3.8.0');
    RETURN;
  END IF;

  IF $1 THEN
    RETURN QUERY
    SELECT set_eq($$SELECT type, id, contracted_vertices, source, target FROM contraction_info$$,
      $$ VALUES  ('v',7,ARRAY[1,3],-1,-1), ('v',4,ARRAY[2],-1,-1), ('v',8,ARRAY[9],-1,-1),
         ('v',6,ARRAY[5],-1,-1), ('v',14,ARRAY[13],-1,-1) $$);

    RETURN QUERY SELECT is_empty($$SELECT id FROM edges WHERE is_new$$);

  ELSE
    RETURN QUERY
    SELECT set_eq($$SELECT type, id, contracted_vertices, source, target FROM contraction_info$$,
      $$ VALUES  ('v',7,ARRAY[1,3],-1,-1), ('v',4,ARRAY[2],-1,-1), ('v',8,ARRAY[9],-1,-1),
         ('v',6,ARRAY[5],-1,-1), ('v',14,ARRAY[13],-1,-1) $$);

    RETURN QUERY SELECT is_empty($$SELECT id FROM edges WHERE is_new$$);

  END IF;

  PREPARE c_expected_graph AS
  SELECT source, target, cost, reverse_cost
  FROM (VALUES
    (6,  10, '-0.996',  '1.004'),
    (6,   7,  '1.016',  '1.016'),
    (7,  11,  '1.064',  '1.064'),
    (7,   8,  '1.100',  '1.100'),
    (8,  12,  '1.144', '-0.856'),
    (10, 11,  '1.025', '-0.975'),
    (10, 15, '-0.991',  '1.009'),
    (11, 16,  '1.081',  '1.081'),
    (11, 12,  '1.121', '-0.879'),
    (12, 17,  '1.169', '-0.831'),
    (15, 16,  '1.256',  '1.256'),
    (15, 16,  '1.256',  '1.256'),
    (16, 17,  '1.225',  '1.225'))
  AS t(source, target, cost, reverse_cost);

  RETURN QUERY
  SELECT set_eq('SELECT source, target, round(cost::numeric, 3)::TEXT AS cost, round(reverse_cost::numeric, 3)::TEXT FROM contracted_graph',
    'c_expected_graph', 'The contracted graph');

  DEALLOCATE PREPARE c_expected_graph;

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE create_linear_edge_cases() AS
$BODY$

BEGIN

  CREATE TABLE graphs (
    id BIGINT PRIMARY KEY GENERATED BY DEFAULT AS IDENTITY,
    source BIGINT,
    target BIGINT,
    cost BIGINT default 1,
    reverse_cost BIGINT default 1,
    dead_case INTEGER
);

/* 11 -> {12 -> {13, 14} [dir=both]} [dir=both]; 13 ->14 [dir=both] */
INSERT INTO graphs(source, target, dead_case)
VALUES
(11, 12, 0),
(11, 13, 0),
(11, 14, 0),
(12, 13, 0),
(12, 14, 0),
(13, 14, 0);

INSERT INTO graphs(source, target, cost, reverse_cost, dead_case)
VALUES
/* 1->11 */
(1, 11, 1, -1, 1),

/* 11->1 */
(11, 1, 1, -1, 2),

/* 1 -> 2 -> 11 */
(1, 2, 1, -1, 3),
(2, 11, 1, -1, 3),

/* 11 -> 2 -> 1 */
(11, 2, 1, -1, 4),
(2, 1, 1, -1, 4),

/* 11 -> 2 [dir=both]; 2 -> 1 [dir=both] */
(11, 2, 1, 1, 5),
(2, 1, 1, 1, 5),

/* 11 -> {1,3} [dir=both]; 1->3 [dir=both]*/
(11, 1, 1, 1, 6),
(1, 3, 1, 1, 6),
(3, 11, 1, 1, 6),

/* 11 -> {1,3} [dir=both]; 2 -> {1,3} [dir=both]; 2->3 [dir=both]*/
(11, 1, 1, 1, 7),
(1, 2, 1, 1, 7),
(2, 3, 1, 1, 7),
(3, 11, 1, -1, 7);

CREATE TABLE results (
    type text,
    contracted_vertices BIGINT[],
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    result_case INTEGER
);

INSERT INTO results(type, contracted_vertices, source, target, cost, result_case)
VALUES
('e',ARRAY[3]::BIGINT[],1,7,2, 1),
('e',ARRAY[3],7,1,2, 1),
('e',ARRAY[3],1,7,2, 2),
('e',ARRAY[3],7,1,2, 2),
('e',ARRAY[3,7],1,11,3, 3),
('e',ARRAY[3,7],11,1,3, 3),
('e',ARRAY[7],3,11,2, 4),
('e',ARRAY[7],11,3,2, 4),
('e',ARRAY[16],15,11,2, 5),
('e',ARRAY[16],11,15,2, 5),
('e',ARRAY[10],15,11,2, 5),
('e',ARRAY[16],15,11,2, 6),
('e',ARRAY[16],11,15,2, 6),
('e',ARRAY[10],15,11,2, 7),
('e',ARRAY[7],6,11,2, 8),
('e',ARRAY[7],11,6,2, 8),
('e',ARRAY[3],1,7,2, 9),
('e',ARRAY[3],7,1,2, 9),
('e',ARRAY[3],1,7,2, 11),
('e',ARRAY[3],7,1,2, 12),
('e',ARRAY[3,7],1,11,3, 13),
('e',ARRAY[7],3,11,2, 14),
('e',ARRAY[10,15],11,16,3, 15),
('e',ARRAY[10,11],15,16,3, 25),
('e',ARRAY[15,16],10,11,3, 16),
('e',ARRAY[10,15],11,16,3, 17),
('e',ARRAY[6,10],7,11,3, 18),
('e',ARRAY[10,11],6,7,3, 28),
('e',ARRAY[6,7],10,11,3, 38),
('e',ARRAY[3],7,1,2, 19),
('e',ARRAY[15],10,16,2, 19),
('e',ARRAY[17],16,12,2, 19),
('e',ARRAY[15],10,16,2, 20),
('e',ARRAY[17],16,12,2, 20)
;

prepare get_solution(integer, integer, text) AS
WITH a AS (SELECT CASE WHEN $3::BOOLEAN THEN $1 ELSE $2 END r)
SELECT type,contracted_vertices,source,target,cost FROM results, a WHERE result_case = r;

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE create_deadend_edge_cases() AS
$BODY$

BEGIN

  CREATE TABLE graphs (
    id BIGINT PRIMARY KEY GENERATED BY DEFAULT AS IDENTITY,
    source BIGINT,
    target BIGINT,
    cost BIGINT default 1,
    reverse_cost BIGINT default 1,
    dead_case INTEGER
  );

  /* 2 -- {3 -- 4} ; 3 -- 4 */
  INSERT INTO graphs(source, target, dead_case)
  VALUES
  (2, 3, 0),
  (2, 4, 0),
  (3, 4, 0);

  INSERT INTO graphs(source, target, cost, reverse_cost, dead_case)
  VALUES
  /* 2->{3->4 [dir=both]} [dir=both]; 1->2 */
  (1, 2, 1, -1, 1),

  /* 2->{3->4 [dir=both]} [dir=both]; 2->1 */
  (2, 1, 1, -1, 2),

  /* 2->{3->4 [dir=both]} [dir=both]; 2->1 [dir=both] */
  (1, 2, 1, 1, 3),

  /* 2->{3->4 [dir=both]} [dir=both]; 2->1,2->1 */
  (2, 1, 1, -1, 4),
  (2, 1, 1, -1, 4),

  /* 2->{3->4 [dir=both]} [dir=both]; {2,3}->1 */
  (2, 1, 1, -1, 5),
  (3, 1, 1, -1, 5),

  /* 2->{3->4 [dir=both]} [dir=both]; 2->1;2->1;3->1 */
  (2, 1, 1, -1, 6),
  (2, 1, 1, -1, 6),
  (3, 1, 1, -1, 6);

  CREATE TABLE results (
    type text,
    id INTEGER,
    contracted_vertices BIGINT[],
    result_case INTEGER
  );

  INSERT INTO results(type, id, contracted_vertices, result_case)
  VALUES
  ('v',6,ARRAY[5]::BIGINT[], 1),
  ('v',5,ARRAY[6], 2),
  ('v',15,ARRAY[10], 3),
  ('v',10,ARRAY[15], 4),
  ('v',12,ARRAY[8], 5),
  ('v',8,ARRAY[12], 6),
  ('v',15,ARRAY[6,10], 7),
  ('v',6,ARRAY[10,15], 8),
  ('v',10,ARRAY[6,15], 9),
  ('v',10,ARRAY[15], 10),
  ('v',10,ARRAY[6], 11),
  ('v',8,ARRAY[11,12], 12),
  ('v',8,ARRAY[12], 13),
  ('v',11,ARRAY[12], 13),
  ('v',11,ARRAY[8,12], 113),
  ('v',12,ARRAY[8,11], 14),
  ('v',12,ARRAY[11], 15),

  ('v',10,ARRAY[6], 16),
  ('v',12,ARRAY[8], 16),
  ('v',6,ARRAY[10], 17),
  ('v',12,ARRAY[8], 17),
  ('v',10,ARRAY[6], 18),
  ('v',8,ARRAY[12], 18),
  ('v',6,ARRAY[10], 19),
  ('v',8,ARRAY[12], 19),
  ('v',6,ARRAY[10], 20),

  ('v',17,ARRAY[10,11,12], 21),
  ('v',10,ARRAY[11,12,17], 22),
  ('v',11,ARRAY[10,12,17], 23),
  ('v',12,ARRAY[10,11,17], 24),

  ('v',8,ARRAY[9], 25),
  ('v',7,ARRAY[1,3], 25),
  ('v',6,ARRAY[5], 25),

  ('v',8,ARRAY[9], 26),
  ('v',3,ARRAY[1], 26),
  ('v',6,ARRAY[5], 26)
  ;

  PREPARE get_solution(integer, integer, text) AS
  WITH a AS (SELECT CASE WHEN $3::BOOLEAN THEN $1 ELSE $2 END r)
  SELECT type,id,contracted_vertices FROM results, a WHERE result_case = r;

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION deadend_edge_cases(directed text)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

RETURN QUERY SELECT is_empty(format($$"graph" ('0',%1$s)$$,directed),'empty graph: 0,'||directed);

RETURN QUERY SELECT isnt_empty(format($$"graph" ('1',%1$s)$$,directed), 'not empty graph: 1,'||directed);
RETURN QUERY SELECT set_eq(format($$"graph" ('1',%1$s)$$,directed), $$ VALUES ('v',2,ARRAY[1]::BIGINT[],-1,-1,-1) $$, 'expected graph: 1,'||directed) ;

RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,1',%1$s)$$,directed), 'not empty graph: 0,1,'||directed);
RETURN QUERY SELECT set_eq(format($$"graph" ('0,1',%1$s)$$,directed), $$ VALUES ('v',2,ARRAY[1]::BIGINT[],-1,-1,-1) $$, 'expected graph: 0,1,'||directed) ;

RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,2',%1$s)$$,directed), 'not empty graph: 0,2,'||directed);
RETURN QUERY SELECT set_eq(format($$"graph" ('0,2',%1$s)$$,directed), $$ VALUES ('v',2,ARRAY[1]::BIGINT[],-1,-1,-1) $$, 'expected graph: 0,2,'||directed) ;

RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,3',%1$s)$$,directed), 'not empty graph: 0,3,'||directed);
RETURN QUERY SELECT set_eq(format($$"graph" ('0,3',%1$s)$$,directed), $$ VALUES ('v',2,ARRAY[1]::BIGINT[],-1,-1,-1) $$, 'expected graph: 0,3,'||directed) ;

RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,4',%1$s)$$,directed), 'not empty graph: 0,4,'||directed);
RETURN QUERY SELECT set_eq(format($$"graph" ('0,4',%1$s)$$,directed), $$ VALUES ('v',2,ARRAY[1]::BIGINT[],-1,-1,-1) $$, 'expected graph: 0,4,'||directed) ;

RETURN QUERY
SELECT CASE WHEN directed::BOOLEAN THEN
  collect_tap(
    isnt_empty(format($$"graph" ('0,5',%1$s)$$,directed), 'empty graph: 0,5,'||directed),
    set_eq(format($$"graph" ('0,5',%1$s)$$,directed), $$ VALUES ('v',2,ARRAY[1]::BIGINT[],-1,-1,-1), ('v',3,ARRAY[1]::BIGINT[],-1,-1,-1) $$, 'expected graph: 0,4,'||directed),
    isnt_empty(format($$"graph" ('0,6',%1$s)$$,directed), 'empty graph: 0,6,'||directed),
    set_eq(format($$"graph" ('0,6',%1$s)$$,directed), $$ VALUES ('v',2,ARRAY[1]::BIGINT[],-1,-1,-1), ('v',3,ARRAY[1]::BIGINT[],-1,-1,-1) $$, 'expected graph: 0,6,'||directed))
ELSE
  collect_tap(
    is_empty(format($$"graph" ('0,5',%1$s)$$,directed), 'empty graph: 0,5,'||directed),
    is_empty(format($$"graph" ('0,6',%1$s)$$,directed), 'empty graph: 0,6,'||directed))
END;

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION deadend_edge_cases_sampledata(directed text)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  -- SINGLE edge

  RETURN QUERY
  SELECT set_eq($$"graph1" ('1','{}',$$||directed||$$)$$, $$"get_solution" (1,1,$$||directed||$$)$$, '5 <=> 6: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('1','{6}',$$||directed||$$)$$, $$"get_solution" (1,1,$$||directed||$$) $$, '5 <=> 6: Directed=' || directed ||', forbid 5');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('1','{5}',$$||directed||$$)$$, $$"get_solution" (2,2,$$||directed||$$)$$, '5 <=> 6: Directed=' || directed ||', forbid 6');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('1','{5,6}',$$||directed||$$)$$, '5 <=> 6: Directed=' || directed ||', forbid 5,6');

  RETURN QUERY
  SELECT set_eq($$"graph1" ('3','{}',$$||directed||$$)$$, $$"get_solution" (3,3,$$||directed||$$)  $$, '10 -> 15: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('3','{10}',$$||directed||$$)$$, $$"get_solution" (4,4,$$||directed||$$) $$, '10 -> 15: Directed=' || directed ||', forbid 10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('3','{15}',$$||directed||$$)$$, $$"get_solution" (3,3,$$||directed||$$) $$, '10 -> 15: Directed=' || directed ||', forbid 15');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('3','{10,15}',$$||directed||$$) $$, '5 <=> 6: Directed=' || directed ||', forbid 10,15');

  RETURN QUERY
  SELECT set_eq($$"graph1" ('12','{}',$$||directed||$$)$$, $$"get_solution" (5,5,$$||directed||$$) $$, '8 -> 12: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('12','{8}',$$||directed||$$)$$, $$"get_solution" (6,6,$$||directed||$$) $$, '8 -> 12: Directed=' || directed ||', forbid 8');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('12','{12}',$$||directed||$$)$$, $$"get_solution" (5,5,$$||directed||$$) $$, '8 -> 12: Directed=' || directed ||', forbid 12');

  -- TWO edges

  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,3','{}',$$||directed||$$)$$, $$"get_solution" (7,7,$$||directed||$$) $$, '15 -> 10 -> 6: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,3','{6}',$$||directed||$$)$$, $$"get_solution" (8,8,$$||directed||$$) $$, '15 -> 10 -> 6: Directed=' || directed ||', forbid 6');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,3','{10}',$$||directed||$$)$$, $$"get_solution" (9,9,$$||directed||$$) $$, '15 -> 10 -> 6: Directed=' || directed ||', forbid 10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,3','{15}',$$||directed||$$)$$, $$"get_solution" (7,7,$$||directed||$$) $$, '15 -> 10 -> 6: Directed=' || directed ||', forbid 15');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,3','{6,10}',$$||directed||$$)$$, $$"get_solution" (10,10,$$||directed||$$) $$, '15 -> 10 -> 6: Directed=' || directed ||', forbid 6,10');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('2,3','{6,15}',$$||directed||$$)$$, '15 -> 10 -> 6: Directed=' || directed ||', forbid 6,15');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,3','{10,15}',$$||directed||$$)$$, $$"get_solution" (11,11,$$||directed||$$) $$, '15 -> 10 -> 6: Directed=' || directed ||', forbid 10,15');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('2,3','{6,10,15}',$$||directed||$$)$$, '15 -> 10 -> 6: Directed=' || directed ||', forbid 6,10,15');


  RETURN QUERY
  SELECT set_eq($$"graph1" ('11,12','{}',$$||directed||$$)$$, $$"get_solution" (12,12,$$||directed||$$) $$, '11 -> 12 <- 8: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('11,12','{8}',$$||directed||$$)$$, $$"get_solution" (12,12,$$||directed||$$) $$, '11 -> 12 <- 8: Directed=' || directed ||', forbid 8');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('11,12','{11}',$$||directed||$$)$$, $$"get_solution" (13,113,$$||directed||$$) $$, '11 -> 12 <- 8: Directed=' || directed ||', forbid 11');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('11,12','{12}',$$||directed||$$)$$, $$"get_solution" (14,14,$$||directed||$$) $$, '11 -> 12 <- 8: Directed=' || directed ||', forbid 12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('11,12','{8,11}',$$||directed||$$)$$, $$"get_solution" (13,213,$$||directed||$$) $$, '11 -> 12 <- 8: Directed=' || directed ||', forbid 8,11');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('11,12','{8,12}',$$||directed||$$)$$, $$"get_solution" (15,15,$$||directed||$$) $$, '11 -> 12 <- 8: Directed=' || directed ||', forbid 8,12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('11,12','{11,12}',$$||directed||$$)$$, $$"get_solution" (5,5,$$||directed||$$) $$, '11 -> 12 <- 8: Directed=' || directed ||', forbid 11,12');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('11,12','{8,11,12}',$$||directed||$$)$$, '11 -> 12 <- 8: Directed=' || directed ||', forbid 8,11,12');


  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{}',$$||directed||$$)$$, $$"get_solution" (16,16,$$||directed||$$) $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{6}',$$||directed||$$)$$, $$"get_solution" (17,17,$$||directed||$$)  $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{8}',$$||directed||$$)$$, $$"get_solution" (18,18,$$||directed||$$)  $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 8');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{10}',$$||directed||$$)$$, $$"get_solution" (16,16,$$||directed||$$)  $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{12}',$$||directed||$$)$$, $$"get_solution" (16,16,$$||directed||$$)  $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{6,8}',$$||directed||$$)$$, $$"get_solution" (19,19,$$||directed||$$)  $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6,8');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{6,10}',$$||directed||$$)$$, $$"get_solution" (5,5,$$||directed||$$) $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6,10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{6,12}',$$||directed||$$)$$, $$"get_solution" (17,17,$$||directed||$$)  $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6,12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{8,10}',$$||directed||$$)$$, $$"get_solution" (18,18,$$||directed||$$)  $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 8,10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{8,12}',$$||directed||$$)$$, $$"get_solution" (11,11,$$||directed||$$) $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 8,12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{6,8,10}',$$||directed||$$)$$, $$"get_solution" (6,6,$$||directed||$$) $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6,8,10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{6,8,12}',$$||directed||$$)$$, $$"get_solution" (20,20,$$||directed||$$) $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6,8,12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{6,10,12}',$$||directed||$$)$$, $$"get_solution" (5,5,$$||directed||$$) $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6,10,12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,12','{8,10,12}',$$||directed||$$)$$, $$"get_solution" (11,11,$$||directed||$$) $$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 8,10,12');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('2,12','{6,8,10,12}',$$||directed||$$)$$, '10 -> 6, 8 -> 12: Directed=' || directed ||', forbid 6,8,10,12');


  -- THREE edges
  RETURN QUERY
  SELECT set_eq($$"graph1" ('5,11,13','{}',$$||directed||$$)$$, $$"get_solution" (21,21,$$||directed||$$) $$, '10 -> 11 -> 12 -> 17: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('5,11,13','{10}',$$||directed||$$)$$, $$"get_solution" (22,22,$$||directed||$$) $$, '10 -> 11 -> 12 -> 17: Directed=' || directed ||', forbid 10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('5,11,13','{11}',$$||directed||$$)$$, $$"get_solution" (23,23,$$||directed||$$) $$, '10 -> 11 -> 12 -> 17: Directed=' || directed ||', forbid 11');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('5,11,13','{12}',$$||directed||$$)$$, $$"get_solution" (24,24,$$||directed||$$) $$, '10 -> 11 -> 12 -> 17: Directed=' || directed ||', forbid 12');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('5,11,13','{17}',$$||directed||$$)$$, $$"get_solution" (21,21,$$||directed||$$) $$, '10 -> 11 -> 12 -> 17: Directed=' || directed ||', forbid 17');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('5,11,13','{10,17}',$$||directed||$$)$$, '10 -> 11 -> 12 -> 17: Directed=' || directed ||', forbid 10,17');

  -- The graph1
  RETURN QUERY
  SELECT set_eq($$"graph1" ('1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16','{}',$$||directed||$$)$$,  $$"get_solution" (25,25,$$||directed||$$) $$, 'the graph1: Directed=' || directed ||', no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16','{3}',$$||directed||$$)$$,  $$"get_solution" (26,26,$$||directed||$$) $$, 'the graph1: Directed=' || directed ||', forbid 3');

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION check_contracted_graph_linear(BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_version('3.8.0') THEN
    RETURN QUERY SELECT skip(1, 'contraction function/signature is new on 3.8.0');
    RETURN;
  END IF;

  IF $1 THEN
    RETURN QUERY
    SELECT set_eq($$SELECT type, id, contracted_vertices, source, target FROM contraction_info$$,
      $$ VALUES   ('e',-1,ARRAY[3],7,1), ('e',-2,ARRAY[3],1,7)$$, 'The edges') ;

    RETURN QUERY
    SELECT set_eq(
      $$SELECT id FROM edges WHERE is_new$$,
      $$SELECT unnest(ARRAY[19, 20])$$, 'The vertices'
    );

    PREPARE c_expected_graph AS
    SELECT source, target, cost, reverse_cost
    FROM (VALUES
      (1,   7,  '2.085', '-1.000'),
      (2,   4,  '1.289',  '1.289'),
      (5,   6,  '1.001',  '1.001'),
      (6,   7,  '1.016',  '1.016'),
      (6,  10, '-0.996',  '1.004'),
      (7,   1,  '2.085', '-1.000'),
      (7,   8,  '1.100',  '1.100'),
      (7,  11,  '1.064',  '1.064'),
      (8,   9,  '1.196',  '1.196'),
      (8,  12,  '1.144', '-0.856'),
      (10, 11,  '1.025', '-0.975'),
      (10, 15, '-0.991',  '1.009'),
      (11, 12,  '1.121', '-0.879'),
      (11, 16,  '1.081',  '1.081'),
      (12, 17,  '1.169', '-0.831'),
      (13, 14,  '1.324',  '1.324'),
      (15, 16,  '1.256',  '1.256'),
      (16, 17,  '1.225',  '1.225'))
    AS t(source, target, cost, reverse_cost);

    RETURN QUERY
    SELECT set_eq('SELECT source, target, round(cost::numeric, 3)::TEXT AS cost, round(reverse_cost::numeric, 3)::TEXT FROM contracted_graph',
      'c_expected_graph', 'The contracted graph');
  ELSE
    RETURN QUERY
    SELECT set_eq($$SELECT type, id, contracted_vertices, source, target FROM contraction_info$$,
      $$ VALUES   ('e',-3,ARRAY[17],16,12), ('e',-2,ARRAY[3],7,1), ('e', -1, ARRAY[15], 10, 16)$$) ;

    RETURN QUERY
    SELECT set_eq(
      $$SELECT id FROM edges WHERE is_new$$,
      $$SELECT unnest(ARRAY[19, 20, 21])$$
    );

    PREPARE c_expected_graph AS
    SELECT source, target, cost, reverse_cost
    FROM (VALUES
      (2,   4,  '1.289',  '1.289'),
      (5,   6,  '1.001',  '1.001'),
      (6,  10, '-0.996',  '1.004'),
      (6,   7,  '1.016',  '1.016'),
      (7,   1,  '2.085', '-1.000'),
      (7,   8,  '1.100',  '1.100'),
      (7,  11,  '1.064',  '1.064'),
      (8,   9,  '1.196',  '1.196'),
      (8,  12,  '1.144', '-0.856'),
      (10, 11,  '1.025', '-0.975'),
      (10, 16,  '2.265', '-1.000'),
      (11, 12,  '1.121', '-0.879'),
      (11, 16,  '1.081',  '1.081'),
      (13, 14,  '1.324',  '1.324'),
      (16, 12,  '2.394', '-1.000'))
    AS t(source, target, cost, reverse_cost);

    RETURN QUERY
    SELECT set_eq('SELECT source, target, round(cost::numeric, 3)::TEXT AS cost, round(reverse_cost::numeric, 3)::TEXT FROM contracted_graph',
      'c_expected_graph', 'The contracted graph');

  END IF;
  DEALLOCATE PREPARE c_expected_graph;
END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION linear_edge_cases_sampledata(directed text)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  -- SINGLE edge

  RETURN QUERY SELECT is_empty($$"graph1" ('6','{}',$$||directed||$$)$$, '1<=>3: Directed=' || directed ||' no forbidden vertices');

  -- TWO edges

  RETURN QUERY SELECT isnt_empty($$"graph1" ('6,7','{}',$$||directed||$$)$$, '1<=>3<=>7: Directed=' || directed ||' no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7','{}',$$||directed||$$)$$, $$"get_solution" (1,11,$$||directed||$$)$$ , '1<=>3<=>7: Directed=' || directed ||' no forbidden');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7','{1}',$$||directed||$$)$$, $$"get_solution" (1,11,$$||directed||$$)$$, '1<=>3<=>7: Directed=' || directed ||' , forbid 1');
  RETURN QUERY SELECT is_empty($$"graph1" ('6,7','{3}',$$||directed||$$)$$, '1<=>3<=>7: Directed=' || directed ||' , forbid 3');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7','{7}',$$||directed||$$)$$, $$"get_solution" (1,11,$$||directed||$$)$$, '1<=>3<=>7: Directed=' || directed ||' , forbid 7');

  -- T shape

  RETURN QUERY SELECT isnt_empty($$"graph1" ('6,7,4,10','{}',$$||directed||$$)$$, '1<=>3<=>7<->6,8: Directed=' || directed ||' no forbidden vertices');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,4,10','{}',$$||directed||$$)$$, $$"get_solution" (2,12,$$||directed||$$)$$, '1<=>3<=>7<->6,8 : Directed=' || directed ||' no forbidden');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,4,10','{1}',$$||directed||$$)$$, $$"get_solution" (2,12,$$||directed||$$)$$, '1<=>3<=>7<->6,8 : Directed=' || directed ||', forbid 1');
  RETURN QUERY
  SELECT is_empty($$"graph1" ('6,7,4,10','{3}',$$||directed||$$)$$, '1<=>3<=>7<->6,8 : Directed=' || directed ||', forbid 3');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,4,10','{7}',$$||directed||$$)$$, $$"get_solution" (2,12,$$||directed||$$)$$, '1<=>3<=>7<->6,8 : Directed=' || directed ||', forbid 7');


  -- THREE edges

  RETURN QUERY SELECT isnt_empty($$"graph1" ('6,7,8','{}',$$||directed||$$)$$, '1<=>3<=>7<=>11: Directed=' || directed ||' no forbidden');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,8','{}',$$||directed||$$)$$, $$"get_solution" (3,13,$$||directed||$$)$$, '1<=>3<=>7<=>11: Directed=' || directed ||' no forbidden');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,8','{1}',$$||directed||$$)$$, $$"get_solution" (3,13,$$||directed||$$)$$, '1<=>3<=>7<=>11: Directed=' || directed ||', forbid 1');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,8','{11}',$$||directed||$$)$$, $$"get_solution" (3,13,$$||directed||$$)$$, '1<=>3<=>7<=>11: Directed=' || directed ||', forbid 11');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,8','{3}',$$||directed||$$)$$, $$"get_solution" (4,14,$$||directed||$$)$$, '1<=>3<=>7<=>11: Directed=' || directed ||', forbid 3');

  RETURN QUERY
  SELECT set_eq($$"graph1" ('6,7,8','{7}',$$||directed||$$)$$, $$"get_solution" (1,11,$$||directed||$$)$$, '1<=>3<=>7<=>11: Directed=' || directed ||', forbid 7');

  -- FOUR edges
  -- circling square
  RETURN QUERY SELECT isnt_empty($$"graph1" ('3,5,9,16','{}',$$||directed||$$)$$, '10->11<=>16<=>15->10: Directed=' || directed ||' no forbidden');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('3,5,9,16','{}',$$||directed||$$)$$, $$"get_solution" (5,15,$$||directed||$$)$$, '10->11<=>16<=>15->10: Directed=' || directed ||' no forbidden');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('3,5,9,16','{11}',$$||directed||$$)$$, $$"get_solution" (5,15,$$||directed||$$)$$, '10->11<=>16<=>15->10: Directed=' || directed ||', forbid 11');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('3,5,9,16','{15}',$$||directed||$$)$$, $$"get_solution" (5,25,$$||directed||$$)$$, '10->11<=>16<=>15->10: Directed=' || directed ||', forbid 15');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('3,5,9,16','{10}',$$||directed||$$)$$, $$"get_solution" (6,16,$$||directed||$$)$$, '10->11<=>16<=>15->10: Directed=' || directed ||', forbid 10');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('3,5,9,16','{16}',$$||directed||$$)$$, $$"get_solution" (7,17,$$||directed||$$)$$, '10->11<=>16<=>15->10: Directed=' || directed ||', forbid 16');

  -- non circling square
  RETURN QUERY SELECT isnt_empty($$"graph1" ('2,4,5,8','{}',$$||directed||$$)$$, '11<-10->6<=>7<=>11->10: Directed=' || directed ||' no forbidden');

  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,4,5,8','{}',$$||directed||$$)$$, $$"get_solution" (8,18,$$||directed||$$)$$, '11<-10->6<=>7<=>11->10: Directed=' || directed ||' no forbidden');

  if directed THEN
    RETURN QUERY
    SELECT is_empty($$"graph1" ('2,4,5,8','{7}',$$||directed||$$)$$, '11<-10->6<=>7<=>11->10: Directed=' || directed ||' forbid 7');
  ELSE
    RETURN QUERY
    SELECT set_eq($$"graph1" ('2,4,5,8','{7}',$$||directed||$$)$$, $$"get_solution" (8,18,$$||directed||$$)$$, '11<-10->6<=>7<=>11->10: Directed=' || directed ||' forbid 7');
  END IF;
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,4,5,8','{11}',$$||directed||$$)$$, $$"get_solution" (8,18,$$||directed||$$)$$, '11<-10->6<=>7<=>11->10: Directed=' || directed ||' forbid 11');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,4,5,8','{6}',$$||directed||$$)$$, $$"get_solution" (8,28,$$||directed||$$)$$, '11<-10->6<=>7<=>11->10: Directed=' || directed ||' forbid 6');
  RETURN QUERY
  SELECT set_eq($$"graph1" ('2,4,5,8','{10}',$$||directed||$$)$$, $$"get_solution" (8,38,$$||directed||$$)$$, '11<-10->6<=>7<=>11->10: Directed=' || directed ||' forbid 10');

  -- The graph1
  RETURN QUERY
  SELECT set_eq($$"graph1" ('1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18','{}',$$||directed||$$)$$, $$"get_solution" (9,19,$$||directed||$$)$$, 'the graph: Directed=' || directed ||' no forbidden');
  IF directed THEN
    RETURN QUERY
    SELECT is_empty($$"graph1" ('1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18','{3}',$$||directed||$$)$$, 'the graph1: Directed=' || directed ||', forbid 3');
  ELSE
    RETURN QUERY
    SELECT set_eq($$"graph1" ('1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18','{3}',$$||directed||$$)$$, $$"get_solution" (1,20,$$||directed||$$)$$, 'the graph1: Directed=' || directed ||', forbid 3');
  END IF;


END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION linear_edge_cases(directed text)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  RETURN QUERY SELECT is_empty(format($$"graph" ('0',%1$s)$$,directed),'empty graph: 0,'||directed);
  RETURN QUERY SELECT is_empty(format($$"graph" ('1',%1$s)$$,directed),'empty graph: 1,'||directed);
  RETURN QUERY SELECT is_empty(format($$"graph" ('2',%1$s)$$,directed),'empty graph: 2,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('3',%1$s)$$,directed),'not empty graph: 3,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('4',%1$s)$$,directed),'not empty graph: 4,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('5',%1$s)$$,directed),'not empty graph: 5,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('6',%1$s)$$,directed),'not empty graph: 6,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('7',%1$s)$$,directed),'not empty graph: 7,'||directed);

  RETURN QUERY SELECT is_empty(format($$"graph" ('0,1',%1$s)$$,directed), 'empty graph: 0,1,'||directed);
  RETURN QUERY SELECT is_empty(format($$"graph" ('0,2',%1$s)$$,directed), 'empty graph: 0,2,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,3',%1$s)$$,directed), 'not empty graph: 0,3,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,4',%1$s)$$,directed), 'not empty graph: 0,4,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,5',%1$s)$$,directed), 'not empty graph: 0,5,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,6',%1$s)$$,directed), 'not empty graph: 0,6,'||directed);
  RETURN QUERY SELECT isnt_empty(format($$"graph" ('0,7',%1$s)$$,directed), 'not empty graph: 0,7,'||directed);

  RETURN QUERY SELECT set_eq(format($$"graph" ('3',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],1,11,2) $$, 'expected graph: 3,'||directed) ;
  RETURN QUERY SELECT set_eq(format($$"graph" ('4',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2) $$, 'expected graph: 4,'||directed) ;

  if directed THEN
    RETURN QUERY SELECT set_eq(format($$"graph" ('5',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2),('e',-2,ARRAY[2]::BIGINT[],1,11,2) $$, 'expected graph: 5,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('6',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[11]::BIGINT[],1,3,2), ('e',-2,ARRAY[11]::BIGINT[],3,1,2) $$, 'expected graph: 6,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('7',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[1,2]::BIGINT[],11,3,3), ('e',-2,ARRAY[1,2]::BIGINT[],3,11,3) $$, 'expected graph: 7,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,3',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],1,11,2) $$, 'expected graph: 0,3,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,4',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2) $$, 'expected graph: 0,4,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,5',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2),('e',-2,ARRAY[2]::BIGINT[],1,11,2) $$, 'expected graph: 0,5,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,6',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[1]::BIGINT[],11,3,2),('e',-2,ARRAY[1]::BIGINT[],3,11,2) $$, 'expected graph: 0,6,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,7',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[1,2]::BIGINT[],11,3,3),('e',-2,ARRAY[1,2]::BIGINT[],3,11,3) $$, 'expected graph: 0,7,'||directed) ;
  ELSE
    RETURN QUERY SELECT set_eq(format($$"graph" ('5',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2) $$, 'expected graph: 5,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('6',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[11]::BIGINT[],1,3,2) $$, 'expected graph: 6,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('7',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[1,11]::BIGINT[],2,3,3) $$, 'expected graph: 7,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,3',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2) $$, 'expected graph: 0,3,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,4',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2) $$, 'expected graph: 0,4,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,5',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[2]::BIGINT[],11,1,2) $$, 'expected graph: 0,5,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,6',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[1]::BIGINT[],11,3,2) $$, 'expected graph: 0,6,'||directed) ;
    RETURN QUERY SELECT set_eq(format($$"graph" ('0,7',%1$s)$$,directed), $$ VALUES ('e',-1,ARRAY[1,2]::BIGINT[],11,3,3) $$, 'expected graph: 0,7,'||directed) ;
  END IF;

END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION test_ids_size(pow INTEGER, directed BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
vids BIGINT[] = ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18];
x BIGINT;
q TEXT;
mainq TEXT;
otherq TEXT;
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


  q = format($d$ SELECT type, id, unnest(contracted_vertices) FROM pgr_contraction(
      $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
      ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], '%1$s'::BOOLEAN)$d$, directed);

  expected = format($d$
    SELECT type, id, unnest
    FROM (VALUES
      ('v'::CHAR, %1$s::BIGINT, %2$s),
      ('v', %3$s, %4$s),
      ('v', %5$s, %6$s),
      ('v', %5$s, %7$s),
      ('v', %8$s, %9$s),
      ('v', %10$s, %11$s)
    ) AS t(type, id, unnest)$d$,
    vids[4], vids[2],
    vids[6], vids[5],
    vids[7], vids[1], vids[3],
    vids[8], vids[9],
    vids[14], vids[13]);

    RETURN QUERY
    SELECT set_eq(q, expected);

    UPDATE edges
    SET id = id - POWER(10, pow), source = source - POWER(10, pow), target = target - POWER(10, pow);
END;
$BODY$
LANGUAGE plpgSQL;
