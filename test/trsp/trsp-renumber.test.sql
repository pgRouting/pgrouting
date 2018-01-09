


SELECT * FROM pgr_trsp(
    'SELECT id::INTEGER, (source+10)::INTEGER AS source, (target+10)::INTEGER AS target, cost::FLOAT FROM edge_table',
    17, 22, false, false
);

-- select string_to_array(reverse(array_to_string(array_prepend(target_id, ('{'||(via_path)||'}')::integer[]),',')),',') as via, to_cost as cost from old_restriction;
CREATE TABLE new_restrictions (
    id SERIAL PRIMARY KEY,
    path BIGINT[],
    cost float);

INSERT INTO new_restrictions (path, cost) VALUES
 (ARRAY[4,7],   100),
 (ARRAY[8,11],  100),
 (ARRAY[7,01],  100),
 (ARRAY[3,8],     4),
 (ARRAY[61,9],  100),
 (ARRAY[3,5,8],   4);

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
    2, 7, false, false,
    'SELECT to_cost, target_id::int4,
    from_edge || coalesce('','' || via_path, '''') AS via_path
    FROM restrictions'
);




