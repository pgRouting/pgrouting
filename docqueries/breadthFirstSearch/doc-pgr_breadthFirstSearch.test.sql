/* --q1 */
SELECT * FROM pgr_breadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table order by id',
    2
);
/* --q2 */
SELECT * FROM pgr_breadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table order by id',
    ARRAY[11,12], max_depth => 2
);
/* --q3 */
SELECT * FROM pgr_breadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table order by id',
    ARRAY[11,12], max_depth => 2, directed => false
);
/* --q4 */
SELECT * FROM pgr_breadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table order by id',
    -10
);
/* --q5 */
