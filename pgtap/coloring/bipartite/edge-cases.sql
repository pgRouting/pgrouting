\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(16);

-- 0 edge, 0 vertex test

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

SELECT is_empty('q1', '1: Graph with 0 edge and 0 vertex');

PREPARE bipartite1 AS
SELECT *
FROM pgr_bipartite(
    'q1'
);

SELECT is_empty('bipartite1', '2: Graph with 0 edge and 0 vertex -> Empty row is returned');


-- 1 vertex test

PREPARE q2 AS
SELECT id, source, 2 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 2;

SELECT set_eq('q2', $$VALUES (2, 2, 2, -1, 1)$$, '3: Graph with only vertex 2');

PREPARE bipartite2 AS
SELECT *
FROM pgr_bipartite(
    'q2'
);
SELECT is_empty('bipartite2', '4: One vertex graph can not be bipartite-> Empty row is returned');



-- 2 vertices test (connected)

PREPARE q3 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 7;

SELECT set_eq('q3', $$VALUES (7, 8, 5, 1, 1)$$, '5: Graph with two connected vertices 8 and 5');

PREPARE bipartite3 AS
SELECT *
FROM pgr_bipartite(
    'q3'
);

SELECT set_eq('bipartite3', $$VALUES (8, 0), (5, 1)$$, '6: Graph is bi-partite');


-- linear tests

-- 3 vertices test

PREPARE q4 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 2;

SELECT set_eq('q4', $$VALUES (1, 1, 2, 1, 1), (2, 2, 3, -1, 1)$$, '7: Graph with three vertices 1, 2 and 3');

PREPARE bipartite4 AS
SELECT *
FROM pgr_bipartite(
    'q4'
);

SELECT set_eq('bipartite4', $$VALUES (1, 0), (2, 1), (3, 0)$$, '8: Bi-partite graph with 3 vertices');


-- 4 vertices test

PREPARE q5 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 3;

SELECT set_eq('q5',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 3, -1, 1),
        (3, 3, 4, -1, 1)
    $$,
    '9: Graph with four vertices 1, 2, 3 and 4'
);

PREPARE bipartite5 AS
SELECT *
FROM pgr_bipartite(
    'q5'
);

SELECT set_eq('bipartite5', $$VALUES (1, 0), (2, 1), (3, 0), (4, 1)$$, '10: Bi-partite graph with 4 vertices');





-- even lenght cycle test

-- 4 vertices length
PREPARE q6 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (8, 10, 11, 12);

SELECT set_eq('q6',
    $$VALUES
        (8, 5, 6, 1, 1),
        (10, 5, 10, 1, 1),
        (11, 6, 11, 1, -1),
        (12, 10, 11, 1, -1)
    $$,
    '11: Graph with four vertices 5, 6, 10 and 11 (cyclic)'
);

PREPARE bipartite6 AS
SELECT *
FROM pgr_bipartite(
    'q6'
);

SELECT set_eq('bipartite6', $$VALUES (5, 0), (6, 1), (10, 1), (11, 0)$$, '12: Cyclic and bipartite graph');


-- odd length cycle test

-- 3 vertices cyclic

CREATE TABLE three_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO three_vertices_table (source, target, cost, reverse_cost) VALUES
    (3, 6, 20, 15),
    (3, 8, 10, -10),
    (6, 8, -1, 12);

PREPARE q7 AS
SELECT id, source, target, cost, reverse_cost
FROM three_vertices_table;

SELECT set_eq('q7',
    $$VALUES
        (1, 3, 6, 20, 15),
        (2, 3, 8, 10, -10),
        (3, 6, 8, -1, 12)
    $$,
    '13: Cyclic Graph with three vertices 3, 6 and 8'
);

PREPARE bipartite7 AS
SELECT *
FROM pgr_bipartite(
    'q7'
);


SELECT is_empty('bipartite7', '14: Graph with odd lenght cycle -> Empty row is returned');


-- 5 vertices cyclic

CREATE TABLE five_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO five_vertices_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (2, 3, 1, 1),
    (3, 4, 1, -1),
    (4, 5, 1, 1),
    (5, 1, 1, -1);
    
PREPARE q8 AS
SELECT id, source, target, cost, reverse_cost
FROM five_vertices_table;

SELECT set_eq('q8',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 3, 1, 1),
        (3, 3, 4, 1, -1),
        (4, 4, 5, 1, 1),
        (5, 5, 1, 1, -1);
    $$,
    '15: Cyclic Graph with 5 vertices 3, 6 and 8'
);

PREPARE bipartite8 AS
SELECT *
FROM pgr_bipartite(
    'q8'
);


SELECT is_empty('bipartite8', '16: Graph with odd lenght cycle -> Empty row is returned');





SELECT * FROM finish();
ROLLBACK;
