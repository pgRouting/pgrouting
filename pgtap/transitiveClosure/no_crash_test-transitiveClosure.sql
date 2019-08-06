\i setup.sql

SELECT plan(4);
-- flags
-- error

SELECT throws_ok(
    'SELECT * FROM pgr_topologicalsort(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table id < 2'',
        3
    )','42883','function pgr_topologicalsort(unknown, integer) does not exist',
    '6: Documentation says it does not work with 1 flags');


SELECT lives_ok(
    'SELECT * FROM pgr_topologicalsort(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 ''
    )',
    '4: Documentation says works with no flags');

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


PREPARE graphchain AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table1 WHERE id = 1 or id = 4 or id = 6',
    ARRAY[1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE graphall AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table1',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

PREPARE graphe1246 AS
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table1 WHERE id = 1 or id =2 or id = 4 or id = 6',
    ARRAY[1, 1]::integer[], 1, ARRAY[]::integer[], true);

SELECT set_eq('graphchain', 'graphall', '1: Chain and complete graph');

SELECT set_eq('graphchain', 'graphe1246', '2: Chain and subgraph');






ROLLBACK;
