\i setup.sql

SELECT plan(10);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--
PREPARE prim1 AS
SELECT * FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id > 18',
    21, 3
);

SELECT set_eq('prim1',
    $$VALUES (1, 0, 21, 21, -1, 0, 0) $$,
    '1: Empty Graph -> Only root vertex is returned');


PREPARE prim2 AS
SELECT *
FROM pgr_primBFS(
    'SELECT id, source, target, cost
     FROM edge_table WHERE id > 18',
    ARRAY[21, 45],
    3
);

SELECT set_eq('prim2',
    $$VALUES
        (1, 0, 21, 21, -1, 0, 0),
        (2, 0, 45, 45, -1, 0, 0)
    $$,
    '2: Empty graph -> Only root vertices are returned');


--
PREPARE prim3 AS
SELECT *
FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table',
    21, 3
);

SELECT set_eq('prim3',
    $$VALUES (1, 0, 21, 21, -1, 0, 0) $$,
    '3: Root not in Graph -> Only root vertex is returned');

--
PREPARE prim4 AS
SELECT seq, start_vid, depth, node, edge, agg_cost <= 3.5
FROM pgr_primBFS(
    'SELECT id, source, target, cost
     FROM edge_table',
    ARRAY[21, 4],
    3
);

SELECT set_eq('prim4',
    $$VALUES
        (1, 4, 0, 4, -1, true),
        (2, 4, 1, 9, 16, true),
        (3, 4, 2, 6, 9, true),
        (4, 4, 3, 3, 5, true),
        (5, 4, 3, 5, 8, true),
        (6, 4, 3, 11, 11, true),
        (7, 21, 0, 21, -1, true)
    $$,
    '4: Root not in Graph -> Only root vertex is returned, Root in graph -> spanning tree is returned');


--
PREPARE prim5 AS
SELECT seq, start_vid, depth, node, edge, depth <= 3
FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    0, 3
);

SELECT set_eq('prim5',
    $$VALUES
        (1, 1, 0, 1, -1, true),
        (2, 1, 1, 2, 1, true),
        (3, 1, 2, 3, 2, true),
        (4, 1, 2, 5, 4, true),
        (5, 1, 3, 4, 3, true),
        (6, 1, 3, 6, 5, true),
        (7, 1, 3, 8, 7, true),
        (8, 1, 3, 10, 10, true),
        (9, 14, 0, 14, -1, true),
        (10, 14, 1, 15, 17, true),
        (11, 16, 0, 16, -1, true),
        (12, 16, 1, 17, 18, true)
    $$,
    '5: root = 0 -> forest (with random root vertices)');

--
PREPARE prim6 AS
SELECT *
FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    4, -3
);


SELECT throws_ok('prim6',
    'P0001',
    'Negative value found on ''max_depth''',
    '6: Negative max_depth throws');



PREPARE prim7 AS
SELECT *
FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[4, 10], -3
);

SELECT throws_ok('prim7',
    'P0001',
    'Negative value found on ''max_depth''',
    '7: Negative max_depth throws');

--
PREPARE prim8 AS
SELECT *
FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    4, 0
);

SELECT set_eq('prim8',
    $$VALUES (1, 0, 4, 4, -1, 0, 0) $$,
    '8: 0 max_depth -> Only root vertex is returned');

--
PREPARE prim9 AS
SELECT seq, depth, start_vid, node, edge, depth <= 9223372036854775807
FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    4
);

SELECT set_eq('prim9',
    $$VALUES
        (1,  0, 4,  4, -1, true),
        (2,  1, 4,  3,  3, true),
        (3,  2, 4,  2,  2, true),
        (4,  2, 4,  6,  5, true),
        (5,  3, 4,  1,  1, true),
        (6,  3, 4,  5,  4, true),
        (7,  3, 4,  9,  9, true),
        (8,  3, 4, 11, 11, true),
        (9,  4, 4,  8,  7, true),
        (10, 4, 4, 10, 10, true),
        (11, 4, 4, 12, 13, true),
        (12, 5, 4,  7,  6, true),
        (13, 5, 4, 13, 14, true)
    $$,
    '9: default max_depth -> complete tree from root');

--
PREPARE prim10 AS
SELECT seq, start_vid, depth, node, edge, depth <= 9223372036854775807
FROM pgr_primBFS(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    0
);

SELECT set_eq('prim10',
    $$VALUES
       (1,  1, 0, 1,-1, true),
       (2,  1, 1, 2, 1, true),
       (3,  1, 2, 3, 2, true),
       (4,  1, 2, 5, 4, true),
       (5,  1, 3, 4, 3, true),
       (6,  1, 3, 6, 5, true),
       (7,  1, 3, 8, 7, true),
       (8,  1, 3,10,10, true),
       (9,  1, 4, 9, 9, true),
       (10,  1, 4,11,11, true),
       (11,  1, 4, 7, 6, true),
       (12,  1, 4,13,14, true),
       (13,  1, 5,12,13, true),
       (14, 14, 0,14,-1, true),
       (15, 14, 1,15,17, true),
       (16, 16, 0,16,-1, true),
       (17, 16, 1,17,18, true)
    $$,
    '10: root = 0 -> forest (with random root vertices)');

SELECT * FROM finish();
ROLLBACK;
