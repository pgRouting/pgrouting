

CREATE TABLE maptest as
SELECT  671222::integer as id, 318255::integer as  source, 222698::integer as target, 14.02::float8 as cost, 14.02::float8 as reverse_cost UNION ALL
SELECT  671223, 222698, 36655, 197.16, 197.16 UNION ALL
SELECT  582877, 408918, 5556, 458.09, 458.09 UNION ALL
SELECT  582876, 318255, 408918, 3.89, 3.89 UNION ALL
SELECT  585280, 5556, 454424, 54.84, 54.84;
SELECT seq, id1 AS node, id2 AS edge, cost::numeric(11,4)
FROM pgr_trsp(
    'select * from maptest',
    582877,     -- edge_id for start
    0,          -- ini_offset
    582877,     -- edge_id of route end
    0.5,        -- end_offset
    true,        -- directed graph?
    true,      -- has_reverse_cost?
    null -- include the turn restrictions
) PG ;

SELECT seq, id1 AS node, id2 AS edge, cost::numeric(11,4)
FROM pgr_trsp(
    'select * from maptest',
    582877,     -- edge_id for start
    0.0000000001,          -- ini_offset
    582877,     -- edge_id of route end
    0.5,        -- end_offset
    true,        -- directed graph?
    true,      -- has_reverse_cost?
    null -- include the turn restrictions
) PG ;

SELECT seq, id1 AS node, id2 AS edge, cost::numeric(11,4)
FROM pgr_trsp(
    'select * from maptest',
    582877,     -- edge_id for start
    0,         -- ini_offset
    582877,     -- edge_id of route end
    0.5,        -- end_offset
    true,        -- directed graph?
    true,      -- has_reverse_cost?
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
) PG ;

SELECT seq, id1 AS node, id2 AS edge, cost::numeric(11,4)
FROM pgr_trsp(
    'select * from maptest',
    582877,     -- edge_id for start
    0.0000000001,          -- ini_offset
    582877,     -- edge_id of route end
    0.5,        -- end_offset
    true,        -- directed graph?
    true,      -- has_reverse_cost?
    $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$
) PG ;

SELECT seq, id1 AS node, id2 AS edge, cost::numeric(11,4)
FROM pgr_trsp(
    'select * from maptest',
    582877,     -- edge_id for start
    0,          -- ini_offset
    582877,     -- edge_id of route end
    1,        -- end_offset
    true,        -- directed graph?
    true      -- has_reverse_cost?
);

