SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 3
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 3,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    2, 3
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    2, 3,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    6, 8
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    6, 8,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    6, 8
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    6, 8,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    1, 17
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    1, 17,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    1, 17
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    1, 17,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 2
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 2,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    2, 2
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict where id > 10',
    2, 2,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (4, 7, 17)',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 14
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (4, 7, 17)',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 14,
    FALSE
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (4, 7, 17)',
    'SELECT * FROM restrict where id > 10',
    2, 14
);

SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (4, 7, 17)',
    'SELECT * FROM restrict where id > 10',
    2, 14,
    FALSE
);
