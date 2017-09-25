


SELECT * FROM pgr_trsp(
    'SELECT id::INTEGER, (source+10)::INTEGER AS source, (target+10)::INTEGER AS target, cost::FLOAT FROM edge_table',
    17, 22, false, false
);

SELECT * FROM pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    2, 7, false, false,
    'SELECT to_cost, target_id::int4,
    from_edge || coalesce('','' || via_path, '''') AS via_path
    FROM restrictions'
);


select 1 AS id, array[4,7] AS path , 0 AS cost INTO new_restrictions;

SELECT * FROM _pgr_trsp(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
    2, 7, false, false,
    'SELECT to_cost, target_id::int4,
    from_edge || coalesce('','' || via_path, '''') AS via_path
    FROM restrictions',
    $$ SELECT * FROM new_restrictions $$
);
