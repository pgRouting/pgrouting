
\i setup.sql

SELECT plan(8);


SELECT has_function('pgr_bdastar',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT function_returns('pgr_bdastar',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record','Returns set record');


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
