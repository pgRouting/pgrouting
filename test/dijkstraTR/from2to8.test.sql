
SELECT *
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions', 2, 8);

