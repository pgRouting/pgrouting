
/* -- q1 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, 3
);
/* -- q2 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, 3,
  false
);
/* -- q3 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  2, ARRAY[3,5],
  false
);
/* -- q4 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2,11], 5
);
/* -- q5 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2,11], ARRAY[3,5],
  false
);
/* -- q6 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT * FROM combinations_table',
  false
);
/* -- q61 */


-- Examples for :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------

/* -- q7 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, 3
);
/* -- q71 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, 5
);
/* -- q72 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  11, 3
);
/* -- q73 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  11, 5
);
/* -- q74 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, ARRAY[3, 5]
);
/* -- q75 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2, 11], 5
);
/* -- q76 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2, 11], ARRAY[3,5]
);
/* -- q77 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3)) AS combinations (source, target)'
);
/* -- q8 */


-- Examples for :ref:`fig2-undirect-Cost-Reverse`
-------------------------------------------------------------------------------


/* -- q9 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, 3,
  false
);
/* -- q91 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, 5,
  false
);
/* -- q92 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  11, 3,
  false
);
/* -- q93 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  11, 5,
  false
);
/* -- q94 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, ARRAY[3,5],
  false
);
/* -- q95 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2,11], 5,
  false
);
/* -- q96 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2, 11], ARRAY[3,5],
  false
);
/* -- q97 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3)) AS combinations (source, target)',
  false
);
/* -- q10 */


-- Examples for :ref:`fig3-direct-Cost`
-------------------------------------------------------------------------------


/* -- q11 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  2, 3
);
/* -- q111 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  2, 5
);
/* -- q112 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  11, 3
);
/* -- q113 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  11, 5
);
/* -- q114 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  2, ARRAY[3,5]
);
/* -- q115 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  ARRAY[2,11], 5
);
/* -- q116 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  ARRAY[2, 11], ARRAY[3,5]
);
/* -- q117 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3)) AS combinations (source, target)'
);
/* -- q12 */


-- Examples for :ref:`fig4-undirect-Cost`
-------------------------------------------------------------------------------

/* -- q13 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  2, 3,
  false
);
/* -- q131 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  2, 5,
  false
);
/* -- q132 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  11, 3,
  false
);
/* -- q133 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  11, 5,
  false
);
/* -- q134 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  2, ARRAY[3,5],
  false
);
/* -- q135 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  ARRAY[2,11], 5,
  false
);
/* -- q136 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  ARRAY[2, 11], ARRAY[3,5],
  false
);
/* -- q137 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost FROM edge_table',
  'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3)) AS combinations (source, target)',
  false
);
/* -- q14 */




-- Equivalences for :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------

/* -- q15 */
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, 3
);
/* -- q151*/
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  2, ARRAY[3]
);
/* -- q152*/
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2], 3
);
/* -- q153*/
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[2], ARRAY[3]
);
/* -- q154*/
SELECT * FROM pgr_dijkstra(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT * FROM (VALUES(2, 3)) AS combinations (source, target)'
);
/* -- q16 */
