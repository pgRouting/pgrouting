\i setup.sql

SELECT plan(4);

PREPARE q1 AS
SELECT * FROM pgr_withPoints (
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (-1)',
    1, -2
);

SELECT * FROM lives_ok('q1');
SELECT * FROM is_empty('q1');

PREPARE q2 AS
SELECT * FROM pgr_withPoints (
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (-1)',
    1, 2
);

SELECT * FROM lives_ok('q2');
SELECT * FROM isnt_empty('q2');

SELECT finish();
