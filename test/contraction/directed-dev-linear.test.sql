
UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

/*
-- GRAPH 1 <=> 2
-- EXPECTED
--   empty
*/
SELECT * FROM pgr_contractGraph(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (1)$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

/*
-- GRAPH 4->3->6
-- EXPECTED
-- 1 | e    | -1 | {3}                 |      4 |      6 |    2
-- New edges:  4->6
*/
SELECT * FROM pgr_contractGraph(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (3, 5)$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);


/*
-- GRAPH 10 <=> 5 <=> 6
-- EXPECTED
--   1 | e    | -1 | {5}                 |      6 |     10 |    2
--   2 | e    | -2 | {5}                 |     10 |      6 |    2
-- New edges:  6 <=> 10
*/

SELECT * FROM pgr_contractGraph(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (8, 10)$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);


/*
-- GRAPH 3 -> 2 <=> 5
-- EXPECTED
--   1 | e    | -1 | {2}                 |      3 |      5 |    2
-- New edges:  3 -> 5
-- New graph: 3 ->{2}  5 -> 2
-- vertex 2 becomes a dead end
*/

\echo -- TODO fix Wrong Result
SELECT * FROM pgr_contractGraph(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (2, 4)$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);


/*
-- GRAPH 3 -> 6 <=> 9
-- EXPECTED
--   1 | e    | -1 | {6}                 |      3 |      9 |    2
-- New edges:  3 -> 5
-- New graph: 3 ->{6}  9 -> 6
-- vertex 6 becomes a dead end
*/
\echo -- TODO fix Wrong Result
SELECT * FROM pgr_contractGraph(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (5, 9)$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);


/*
-- GRAPH (all graph)
-- EXPECTED
--   1 | e    | -1 | {8}                 |      5 |      7 |    2
--   1 | e    | -1 | {8}                 |      7 |      5 |    2
-- New edges:  3 <=> 5
-- New graph: edges 6 & 7 removed in both ways
-- vertex 6 becomes a dead end
*/
SELECT * FROM pgr_contractGraph(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);


/*
-- GRAPH
--       5 <=> 6
--       ||    -|
--       2 <-  3
-- EXPECTED
--   1 | e    | -1 | {5}                 |      2 |      6 |    2
--   2 | e    | -1 | {5}                 |      6 |      2 |    2
-- New edges:  2 <=> 6
-- New graph:
--       ===== 6
--       ||    -|
--       2 <-  3
*/

SELECT * FROM pgr_contractGraph(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id IN (2, 4, 5, 8)$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);
