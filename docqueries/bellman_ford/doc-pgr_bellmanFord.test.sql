
/* -- q1 */
SELECT * FROM pgr_bellmanFord(
    'SELECT id, source, target, cost, reverse_cost 
    FROM edge_table',
    2, 3
);

/* -- q2 */
SELECT * FROM pgr_bellmanFord(
    'SELECT id, source, target, cost, reverse_cost 
    FROM edge_table',
    2, 3,
    FALSE
);
/* -- q3 */
SELECT * FROM pgr_bellmanFord(
    'SELECT id, source, target, cost, reverse_cost 
    FROM edge_table',
    2, ARRAY[3,5],
    FALSE
);
/* -- q4 */
SELECT * FROM pgr_bellmanFord(
    'SELECT id, source, target, cost, reverse_cost 
    FROM edge_table',
    ARRAY[2,11], 5
);
/* -- q5 */
SELECT * FROM pgr_bellmanFord(
    'SELECT id, source, target, cost, reverse_cost 
    FROM edge_table',
    ARRAY[2,11], ARRAY[3,5]
);
/* -- q51 */
SELECT * FROM combinations_table;
/* -- q6 */
SELECT * FROM pgr_bellmanFord(
    'SELECT id, source, target, cost, reverse_cost 
    FROM edge_table',
    'SELECT * FROM combinations_table'
);
/* -- q7 */
