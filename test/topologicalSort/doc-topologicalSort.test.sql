CREATE TABLE edge_table1 (
    id serial,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision
);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (1,2,0,0);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (2,3,0,0);

SELECT * FROM pgr_topologicalSort(
    'SELECT id,
            source,
            target,
            cost,
            reverse_cost
    FROM edge_table1'
);

SELECT * FROM pgr_topologicalsort(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table1'
); 

\echo -- q1
