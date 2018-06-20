
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
    1,
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
