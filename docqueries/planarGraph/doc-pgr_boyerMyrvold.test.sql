\echo -- q1
SELECT * FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
        FROM edge_table'
);

\echo -- q2
INSERT INTO edge_table (source, target, cost, reverse_cost) VALUES
(1,3,1,-1),(1,4,1,-1),(1,5,1,-1),(2,4,1,-1),(2,5,1,-1),(3,5,1,-1),(4,5,1,-1);

\echo -- q3

SELECT * FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
        FROM edge_table'
);

\echo -- q4
SELECT * FROM pgr_boyerMyrvold(
$$
 SELECT id, source, target, cost, reverse_cost FROM edge_table
    where source = any (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edge_table ')
                        WHERE component = 14)
                       )
                   OR
          target = any (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edge_table ')
                        WHERE component = 14)
                       )
$$
 );

 \echo -- q5
