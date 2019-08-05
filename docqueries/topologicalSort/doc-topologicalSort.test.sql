CREATE TABLE edge_table1 (
    id serial,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision
);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (0,1,1,-1);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (0,3,1,-1);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (0,2,1,-1);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (1,3,1,-1);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (1,2,1,-1);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (3,2,1,-1);

\echo -- q1

SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table1'
);

\echo -- q2

SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table where id=2'
);
SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table where id=3'
);

SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table where id=2 or id=3'
);

SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table where id=11'
);

\echo -- q3

SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table where cost=-1 or reverse_cost=-1'
);


\echo -- q4
