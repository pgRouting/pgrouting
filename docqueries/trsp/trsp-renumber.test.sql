


SELECT * FROM pgr_trsp(
    'SELECT id::INTEGER, (source+10)::INTEGER AS source, (target+10)::INTEGER AS target, cost::FLOAT FROM edge_table',
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$,
    17, 22, false
);

SELECT * FROM _pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    $$ SELECT 1 AS id, ARRAY[4,7] AS path, 100 AS cost $$,
    2, 7, false
);

SELECT * FROM _pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    $$ SELECT * FROM new_restrictions $$,
    2, 7, false
);


SELECT * FROM pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    'SELECT to_cost, target_id::int4,
    from_edge || coalesce('','' || via_path, '''') AS via_path
    FROM restrictions',
    2, 7, false
);
