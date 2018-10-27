\i setup.sql

SELECT plan(5);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--
PREPARE kruskal1 AS
SELECT * FROM pgr_kruskalDD(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id > 18',
    21, 3.5
);

SELECT set_eq('kruskal1',
    $$VALUES (1,21,0,21,-1,0,0) $$,
    '1: Empty Graph -> Only root vertex is returned');


PREPARE kruskal2 AS
SELECT *
FROM pgr_kruskalDD(
    'SELECT id, source, target, cost
     FROM edge_table WHERE id > 18',
    ARRAY[21, 45],
    3.5
);

SELECT set_eq('kruskal2',
    $$VALUES
        (1,21,0,21,-1,0,0),
        (2,45,0,45,-1,0,0)
    $$,
    '2: Empty graph -> Only root vertices are returned');


--
PREPARE kruskal3 AS
SELECT *
FROM pgr_kruskalDD(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table',
    21, 3
);

SELECT set_eq('kruskal3',
    $$VALUES (1,21,0,21,-1,0,0) $$,
    '1: Root not in Graph -> Only root vertex is returned');

--
PREPARE kruskal4 AS
SELECT seq, start_vid, depth, node, edge, agg_cost <= 3.5
FROM pgr_kruskalDD(
    'SELECT id, source, target, cost
     FROM edge_table',
    ARRAY[21, 4],
    3.5
);

SELECT set_eq('kruskal4',
    $$VALUES
        (1,4,1,4,-1,true),
        (2,4,2,9,16,true),
        (3,4,3,6,9,true),
        (4,4,4,3,5,true),
        (5,4,4,5,8,true),
        (6,4,4,11,11,true),
        (7,21,0,21,-1,true)
    $$,
    '4: Root not in Graph -> Only root vertex is returned, Root in graph -> spanning tree is returned');


--
PREPARE kruskal5 AS
SELECT seq, start_vid, depth, node, edge, agg_cost <= 3.5
FROM pgr_kruskalDD(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    0, 3.5
);

SELECT set_eq('kruskal5',
    $$VALUES
        (1,1,1,1,-1,true),
        (2,1,2,2,1,true),
        (3,1,3,3,2,true),
        (4,1,4,4,3,true),
        (5,1,4,6,5,true),
        (6,1,3,5,4,true),
        (7,1,4,8,7,true),
        (8,1,4,10,10,true),
        (9,14,1,14,-1,true),
        (10,14,2,15,17,true),
        (11,16,1,16,-1,true),
        (12,16,2,17,18,true)
    $$,
    '5: root = 0 -> forset (with random root vertices)');



SELECT * FROM finish();
ROLLBACK;
