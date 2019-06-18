SELECT * FROM pgr_topologicalsort(
    'SELECT id,
            source,
            target,
            cost,
            reverse_cost
    FROM edge_table'
    
);

SELECT * FROM pgr_topologicalsort(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2'
); 

\echo -- q1
