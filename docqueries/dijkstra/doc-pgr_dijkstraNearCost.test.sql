/* -- q1 */
SELECT * FROM pgr_dijkstraNearCost(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  6, ARRAY[10, 11, 1]);
/* -- q2 */
SELECT * FROM pgr_dijkstraNearCost(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  ARRAY[10, 11, 1], 6,
  true,
  cap => 2) ORDER BY agg_cost;
/* -- q3 */
SELECT * FROM pgr_dijkstraNearCost(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  ARRAY[15, 16], ARRAY[10, 11, 1],
  directed => false);
/* -- q4 */
SELECT unnest(ARRAY[10, 11, 1]) as source, target
FROM (SELECT unnest(ARRAY[15, 16]) AS target) a
  UNION
SELECT unnest(ARRAY[15, 16]), target
FROM (SELECT unnest(ARRAY[10, 11, 1]) AS target) b ORDER BY source, target;
/* -- q41 */
SELECT * FROM pgr_dijkstraNearCost(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  'SELECT unnest(ARRAY[10, 11, 1]) as source, target
   FROM (SELECT unnest(ARRAY[15, 16]) AS target) a
     UNION
   SELECT unnest(ARRAY[15, 16]), target
   FROM (SELECT unnest(ARRAY[10, 11, 1]) AS target) b',
  global => false);
/* -- q5 */
