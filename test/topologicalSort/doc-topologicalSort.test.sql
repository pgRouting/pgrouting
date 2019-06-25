CREATE TABLE edge_table1 (
    id serial,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision
);
INSERT INTO edge_table1 (source,target,cost,reverse_cost) VALUES (1,2,1,-1);

SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table where id=2'
);
SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table1'
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
SELECT * FROM pgr_topologicalsort(
  'SELECT id,source,target,cost,reverse_cost FROM edge_table where cost=-1 or reverse_cost=-1'
);


\echo -- q1
