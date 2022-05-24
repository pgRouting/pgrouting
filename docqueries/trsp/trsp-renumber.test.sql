


SELECT * FROM pgr_trsp(
    'SELECT id::INTEGER, (source+10)::INTEGER AS source, (target+10)::INTEGER AS target, cost::FLOAT FROM edge_table',
    11, 27, false, false
);

SELECT * FROM _pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    $$ SELECT 1 AS id, ARRAY[4,7] AS path, 100 AS cost $$,
    6, 1, false
);

SELECT * FROM _pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    $$ SELECT * FROM restrictions $$,
    6, 1, false
);


SELECT * FROM pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    6, 1, false, false,
    'SELECT to_cost, target_id::int4, via_path
    FROM old_restrictions'
);




