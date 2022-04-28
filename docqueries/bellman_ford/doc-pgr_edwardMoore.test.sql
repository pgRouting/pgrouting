/* -- q2 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3,
    FALSE
);
/* -- q3 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    2, ARRAY[3,5],
    FALSE
);
/* -- q4 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2,11], 5
);
/* -- q5 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2,11], ARRAY[3,5],
    FALSE
);
/* -- q51 */
SELECT * FROM combinations_table;
/* -- q6 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM combinations_table',
    FALSE
);
/* -- q7 */

-- Examples for :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------

/* -- q8 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 5
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[3,5]
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    11, 3
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    11, 5
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2,11], 5
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2, 11], ARRAY[3,5]
);
/* -- q9 */


-- Examples for :ref:`fig2-undirect-Cost-Reverse`
-------------------------------------------------------------------------------


/* -- q10 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 5,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    11, 3,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    11, 5,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2,11], 5,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[3,5],
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[2, 11], ARRAY[3,5],
    FALSE
);
/* -- q11 */


-- Examples for :ref:`fig3-direct-Cost`
-------------------------------------------------------------------------------


/* -- q12 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    2, 3
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    2, 5
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    11, 3
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    11, 5
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[2,11], 5
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    2, ARRAY[3,5]
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[2, 11], ARRAY[3,5]
);
/* -- q13 */


-- Examples for :ref:`fig4-undirect-Cost`
-------------------------------------------------------------------------------

/* -- q14 */
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    2, 3,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    2, 5,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    11, 3,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    11, 5,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[2,11], 5,
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    2, ARRAY[3,5],
    FALSE
);
SELECT * FROM pgr_edwardMoore(
    'SELECT id, source, target, cost FROM edge_table',
    ARRAY[2, 11], ARRAY[3,5],
    FALSE
);
/* -- q15 */
