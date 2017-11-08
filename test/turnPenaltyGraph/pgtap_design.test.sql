
\echo -- one edge graph
SELECT * FROM pgr_turnPenaltyGraph(
    'SELECT id, source, target, cost FROM edge_table WHERE id = 1'
);

\echo -- two edge graphs
SELECT * FROM pgr_turnPenaltyGraph($$
    SELECT -id AS id, target, source, reverse_cost AS cost FROM edge_table WHERE id = 1
    UNION
    SELECT id, source, target, cost FROM edge_table WHERE id = 1
    $$
);
SELECT * FROM pgr_turnPenaltyGraph($$
    SELECT id, target, source, reverse_cost AS cost FROM edge_table WHERE id = 1
    UNION
    SELECT -id AS id, source, target, cost FROM edge_table WHERE id = 1
    $$
);
SELECT * FROM pgr_turnPenaltyGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1'
);
SELECT * FROM pgr_turnPenaltyGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 17'
);
SELECT * FROM pgr_turnPenaltyGraph(
    'SELECT id*100 AS id, source*1000 as source, target*1000 as target , cost FROM edge_table WHERE id = 1'
);
SELECT * FROM pgr_turnPenaltyGraph( 
    'SELECT id*100 AS id, source*1000 as source, target*1000 as target , cost FROM edge_table WHERE id in (6,7)'
);
SELECT * FROM pgr_turnPenaltyGraph(
    'SELECT id*100 AS id, source*1000 as source, target*1000 as target , cost FROM edge_table where id in (5,9,11)'
);

