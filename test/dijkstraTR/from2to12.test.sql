
SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id < 3',
    2, 12,
    1
);

SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id < 3',
    2, 12,
    1,
    stop_on_first:=false
);

SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id < 3',
    2, 12,
    3,
    stop_on_first:=false
);
-- all have restrictions
SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    1
);

SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    2,
    stop_on_first:=false
);

SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    3,
    stop_on_first:=false
);
-- all have restrictions
SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    1,
    heap_paths:=true
);

SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    2,
    heap_paths:=true,
    stop_on_first:=false
);

SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    3,
    heap_paths:=true,
    stop_on_first:=false
);
