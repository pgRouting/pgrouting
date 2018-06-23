SELECT * FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 8
);

SELECT * FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrict WHERE id IN (1)',
    2, 8,
    FALSE
);
