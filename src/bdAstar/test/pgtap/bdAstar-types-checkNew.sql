
\i setup.sql

SELECT plan(41);


SELECT has_function('pgr_bdastar',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT function_returns('pgr_bdastar',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record','Returns set record');

SELECT has_function('pgr_bdastar',ARRAY['text', 'bigint', 'bigint']);
SELECT function_returns('pgr_bdastar', ARRAY['text', 'bigint', 'bigint'], 'setof record','Returns set record');


-- CHECKING THE INNER QUERY


PREPARE q1 AS
SELECT * FROM pgr_bdastar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table',
    2, 3, true);
SELECT lives_ok('q1', 'edges query accepts INTEGER & FLOAT');



SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'id accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'id accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'source accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::SMALLINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'source accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'target accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::SMALLINT, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'target accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::BIGINT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'cost accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'cost accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::SMALLINT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'cost accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::REAL, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'cost accepts REAL');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::BIGINT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'reverse_cost accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::INTEGER, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'reverse_cost accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::SMALLINT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'reverse_cost accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::REAL, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'reverse_cost accepts REAL');

-- x1
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::BIGINT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x1 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::INTEGER, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x1 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::SMALLINT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x1 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::REAL, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x1 accepts REAL');

-- x2
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::BIGINT, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x2 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::INTEGER, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x2 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::SMALLINT, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x2 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::REAL, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'x2 accepts REAL');


-- y1
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::BIGINT, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y1 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::INTEGER, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y1 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::SMALLINT, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y1 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::REAL, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y1 accepts REAL');

-- y2
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::BIGINT, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y2 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::INTEGER, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y2 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::SMALLINT, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y2 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::REAL, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true)',
        'y2 accepts REAL');



SELECT throws_ok(
    $$SELECT * FROM pgr_bdastar('SELECT * FROM edge_table',
        2, 3,
        true, 6, 1, 1)$$,
    'XX000',
    'Unknown heuristic',
        'SHOULD THROW because heuristic > 5'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_bdastar('SELECT * FROM edge_table',
        2, 3,
        true, -1, 1, 1)$$,
    'XX000',
    'Unknown heuristic',
    'SHOULD THROW because heuristic < 0'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_bdastar('SELECT * FROM edge_table',
        2, 3,
        true, 0, 0, 1)$$,
    'XX000',
    'Factor value out of range',
    'SHOULD THROW because factor = 0'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_bdastar('SELECT * FROM edge_table',
        2, 3,
        true, 0, -1.4, 1)$$,
    'XX000',
    'Factor value out of range',
    'SHOULD THROW because factor < 0'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_bdastar('SELECT * FROM edge_table',
        2, 3,
        true, 0, 1, -3)$$,
    'XX000',
    'Epsilon value out of range',
    'SHOULD THROW because epsilon < 0'
);
SELECT throws_ok(
    $$SELECT * FROM pgr_bdastar('SELECT * FROM edge_table',
        2, 3,
        true, 0, 1, 0.9)$$,
    'XX000',
    'Epsilon value out of range',
    'SHOULD THROW because epsilon < 1'
);




SELECT finish();
ROLLBACK;
