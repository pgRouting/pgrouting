-- documentation queries
\echo -- q0
SELECT * FROM pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 3, 9]
);

\echo -- q1
SELECT * FROM pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 5, 3, 9, 4]
);

\echo -- q2
SELECT agg_cost FROM  pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 5, 3, 9, 4]
) 
WHERE path_id = 3 AND edge <0;


\echo -- q3
SELECT path_id, node, route_agg_cost FROM  pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 5, 3, 9, 4]
) 
WHERE  edge < 0;

SELECT route_agg_cost FROM  pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 5, 3, 9, 4]
)
WHERE path_id = 3 AND edge < -1;


\echo -- q4
SELECT row_number() over () as node_seq, node 
FROM  pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 5, 3, 9, 4]
) 
WHERE edge <> -1 ORDER BY seq;


\echo -- q5
SELECT path_id, route_agg_cost FROM  pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 5, 3, 9, 4]
) 
WHERE edge < 0;


\echo -- q6
SELECT seq, route_agg_cost, node, agg_cost ,
CASE WHEN edge = -1 THEN 'visits'
ELSE 'passes in front'
END as status
FROM  pgr_dijkstraViaVertex(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
    ARRAY[1, 5, 3, 9, 4])
WHERE node = 9 and (agg_cost  <> 0 or seq = 1);


