
CREATE OR REPLACE FUNCTION general_no_crash(fn_name TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    PREPARE edges_q AS
    SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2  FROM edges;

    PREPARE null_ret AS
    SELECT id FROM vertices  WHERE id IN (-1);

    PREPARE null_ret_arr AS
    SELECT array_agg(id) FROM vertices  WHERE id IN (-1);

    RETURN QUERY
    SELECT isnt_empty('edges_q', 'Should be not empty to tests be meaningful');
    RETURN QUERY
    SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');
    RETURN QUERY
    SELECT set_eq('null_ret_arr', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');

    -- one to one
    params = ARRAY['$$edges_q$$','5::BIGINT', '6::BIGINT']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT id FROM vertices  WHERE id IN (-1))',
    '(SELECT id FROM vertices  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    -- one to many
    params = ARRAY['$$edges_q$$','5', 'ARRAY[6,7]::BIGINT[]']::TEXT[];
    subs = ARRAY[
    'NULL',
   '(SELECT id FROM vertices  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    -- many to one
    params = ARRAY['$$edges_q$$', 'ARRAY[6,7]::BIGINT[]', '5']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))',
    '(SELECT id FROM vertices  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    -- many to many
    params = ARRAY['$$edges_q$$','ARRAY[5]::BIGINT[]', 'ARRAY[6,7]::BIGINT[]']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);
   subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    -- many to many
    params = ARRAY['$$edges_q$$','ARRAY[5]::BIGINT[]', 'ARRAY[6,7]::BIGINT[]']::TEXT[];
    subs = ARRAY[
    'NULL',
    '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))',
    '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::BIGINT[]',
    'NULL::BIGINT[]'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    IF NOT min_version('3.2.0') THEN
      RETURN QUERY
      SELECT skip(1, 'Combinations signature added on version 3.2.0');
      RETURN;
    END IF;

    PREPARE combinations_q AS
    SELECT source, target  FROM combinations;

    PREPARE null_combinations AS
    SELECT source, target FROM combinations WHERE source IN (-1);

    RETURN QUERY
    SELECT isnt_empty('combinations_q', 'Should be not empty to tests be meaningful');

    RETURN QUERY
    SELECT is_empty('null_combinations', 'Should be empty to tests be meaningful');

    -- Combinations SQL
    params = ARRAY['$$edges_q$$', '$$combinations_q$$']::TEXT[];
    subs = ARRAY[
    'NULL',
    '$$(SELECT source, target FROM combinations  WHERE source IN (-1))$$'
    ]::TEXT[];

    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

    subs = ARRAY[
    'NULL',
    'NULL::TEXT'
    ]::TEXT[];
    RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;

