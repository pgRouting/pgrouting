\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(81);

PREPARE edges AS
SELECT id,
    source,
    target,
    capacity,
     reverse_capacity
FROM edge_table
ORDER BY id;

-- A source should not be a sink
PREPARE combinations AS
SELECT source, target  FROM combinations_table WHERE target > 2;

PREPARE null_ret AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);

PREPARE null_ret_arr AS
SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1);

PREPARE null_combinations AS
SELECT source, target FROM combinations_table WHERE source IN (-1);

SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
SELECT isnt_empty('combinations', 'Should be not empty to tests be meaningful');
SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');
SELECT is_empty('null_combinations', 'Should be empty to tests be meaningful');
SELECT set_eq('null_ret_arr', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');

CREATE OR REPLACE FUNCTION test(params TEXT[], subs TEXT[])
RETURNS SETOF TEXT AS
$BODY$
DECLARE
mp TEXT[];
q1 TEXT;
q TEXT;
separator TEXT;
BEGIN
    FOR i IN 0..array_length(params, 1) LOOP
        separator = ' ';
        mp := params;
        IF i != 0 THEN
            mp[i] = subs[i];
        END IF;

        q1 := 'SELECT * FROM pgr_maxFlow (';

        FOR i IN 1..array_length(mp, 1) LOOP
            q1 := q1 || separator || mp[i];
            separator := ',';
        END LOOP;

        q1 := q1 || ')';

        RETURN query SELECT * FROM lives_ok(q1);
        IF i = 0 THEN
            RETURN query SELECT * FROM isnt_empty(q1);
        ELSE
            RETURN query SELECT * FROM set_eq(q1, 'SELECT NULL::BIGINT');
        END IF;
    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    -- one to one
    params = ARRAY['$$edges$$',
    '1::BIGINT',
    '2::BIGINT'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM test(params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);
    -- one to many

    params = ARRAY['$$edges$$',
    '1::BIGINT',
    'ARRAY[2,5]::BIGINT[]'
    ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);

    -- many to one
    params = ARRAY['$$edges$$',
        'ARRAY[2,5]::BIGINT[]',
        '1'
        ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);

    -- many to many
    params = ARRAY['$$edges$$',
        'ARRAY[1]::BIGINT[]',
        'ARRAY[2,5]::BIGINT[]'
        ]::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);

    -- Combinations SQL
    params = ARRAY['$$edges$$', '$$combinations$$']::TEXT[];
    subs = ARRAY[
    'NULL',
    '$$(SELECT source, target FROM combinations_table  WHERE source IN (-1))$$'
    ]::TEXT[];

    RETURN query SELECT * FROM test(params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::TEXT'
    ]::TEXT[];
    RETURN query SELECT * FROM test(params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
