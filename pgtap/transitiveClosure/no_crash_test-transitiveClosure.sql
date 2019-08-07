\i setup.sql

SELECT plan(2);
-- flags
-- error

SELECT throws_ok(
    'SELECT * FROM pgr_transitiveclosure(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table id < 2'',
        3
    )','42883','function pgr_transitiveclosure(unknown, integer) does not exist',
    '6: Documentation says it does not work with 1 flags');


SELECT lives_ok(
    'SELECT * FROM pgr_transitiveclosure(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 ''
    )',
    '4: Documentation says works with no flags');

ROLLBACK;
