--\i setup.sql

SELECT plan(142);

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

-- Initial tables are good to work

SELECT isnt_empty($$SELECT id, source, target, cost, reverse_cost FROM edge_table$$);
SELECT isnt_empty($$SELECT id FROM edge_table_vertices_pgr$$);

-- vertex id values that dont exist
SELECT is_empty($$SELECT id FROM edge_table_vertices_pgr WHERE id > 18$$);
SELECT is_empty($$SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE source > 18$$);
SELECT is_empty($$SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE target > 18$$);
SELECT is_empty($$SELECT id FROM edge_table WHERE id > 18$$);

CREATE TABLE expected AS
WITH
a AS (SELECT start_vid, d_w_1, u_w_1, d_n_1, u_n_1, d_w_2, u_w_2, d_n_2, u_n_2
  FROM (VALUES
    (1::BIGINT,
        ARRAY[2]::BIGINT[],   ARRAY[2]::BIGINT[],    ARRAY[2]::BIGINT[],   ARRAY[2]::BIGINT[],
        ARRAY[2,5]::BIGINT[], ARRAY[2,3]::BIGINT[],  ARRAY[2,5]::BIGINT[], ARRAY[2, 5]::BIGINT[]),
    (2,  ARRAY[ 1], ARRAY[ 1], ARRAY[ 5], ARRAY[  1], ARRAY[1, 5], ARRAY[1, 3],  ARRAY[5, 6],   ARRAY[1, 5]),
    (3,  ARRAY[ 2], ARRAY[ 2], ARRAY[ 6], ARRAY[  6], ARRAY[2, 6], ARRAY[2, 4],  ARRAY[6, 9],   ARRAY[5, 6]),
    (4,  ARRAY[ 3], ARRAY[ 3], ARRAY[ 9], ARRAY[  9], ARRAY[3, 9], ARRAY[3, 9],  ARRAY[9, 12],  ARRAY[6, 9]),
    (5,  ARRAY[ 2], ARRAY[ 2], ARRAY[ 6], ARRAY[  2], ARRAY[2, 8], ARRAY[2, 8],  ARRAY[6, 10],  ARRAY[2, 8]),
    (6,  ARRAY[ 5], ARRAY[ 3], ARRAY[ 9], ARRAY[  3], ARRAY[5, 9], ARRAY[3, 5],  ARRAY[9, 11],  ARRAY[3, 5]),
    (7,  ARRAY[ 8], ARRAY[ 8], ARRAY[ 8], ARRAY[  8], ARRAY[5, 8], ARRAY[5, 8],  ARRAY[5, 8],   ARRAY[5, 8]),
    (8,  ARRAY[ 7], ARRAY[ 7], ARRAY[ 5], ARRAY[  7], ARRAY[5, 7], ARRAY[5, 7],  ARRAY[5, 6],   ARRAY[5, 7]),
    (9,  ARRAY[ 6], ARRAY[ 6], ARRAY[12], ARRAY[  6], ARRAY[6,12], ARRAY[6, 12], ARRAY[12],     ARRAY[6, 12]),
    (10, ARRAY[ 5], ARRAY[ 5], ARRAY[11], ARRAY[  5], ARRAY[5,11], ARRAY[5, 11], ARRAY[11, 13], ARRAY[5, 11]),
    (11, ARRAY[12], ARRAY[ 6], ARRAY[12], ARRAY[  6], ARRAY[9,12], ARRAY[6, 10], ARRAY[12],     ARRAY[6, 10]),
    (12, ARRAY[ 9], ARRAY[11], NULL,      ARRAY[ 11], ARRAY[6, 9], ARRAY[9, 11], NULL,          ARRAY[9, 11]),
    (13, ARRAY[10], ARRAY[10], NULL,      ARRAY[ 10], ARRAY[5,10], ARRAY[5, 10], NULL,          ARRAY[5, 10]),
    (14, ARRAY[15], ARRAY[15], ARRAY[15], ARRAY[ 15], ARRAY[15],   ARRAY[15],    ARRAY[15],     ARRAY[15]),
    (15, ARRAY[14], ARRAY[14], NULL,      ARRAY[ 14], ARRAY[14],   ARRAY[14],    NULL,          ARRAY[14]),
    (16, ARRAY[17], ARRAY[17], ARRAY[17], ARRAY[ 17], ARRAY[17],   ARRAY[17],    ARRAY[17],     ARRAY[17]),
    (17, ARRAY[16], ARRAY[16], NULL,      ARRAY[ 16], ARRAY[16],   ARRAY[16],    NULL,          ARRAY[16])
  ) t(start_vid, d_w_1, u_w_1, d_n_1, u_n_1, d_w_2, u_w_2, d_n_2, u_n_2))
SELECT * from a;


CREATE OR REPLACE FUNCTION check_expected(edges_q TEXT, expected_q TEXT, optionals TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijstraNear_query TEXT;
expected_query TEXT;
BEGIN

    FOR id IN 1..17 LOOP
        dijstraNear_query := format($$
            SELECT array_agg(distinct end_vid)
            FROM pgr_dijkstraNearCost(
                %1$L,
                %2$s,
                (SELECT array_agg(id) FROM edge_table_vertices_pgr where id != %2$s),
                %3$s)
            $$, edges_q, id, optionals);
        expected_query := format($$
            SELECT scnd FROM (%1$s) a WHERE frst = %2$s
            $$, expected_q, id);

        -- raise notice '%', dijstraNear_query;
        -- raise notice '%', expected_query;
        RETURN QUERY
        SELECT set_eq(dijstraNear_query, expected_query, id::TEXT || ' ' || optionals);
    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE;

SELECT * from check_expected(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    $$SELECT start_vid AS frst, d_w_1 AS scnd FROM expected$$,
    'directed => true, cap => 1'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    $$SELECT start_vid AS frst, u_w_1 AS scnd FROM expected$$,
    'directed => false, cap => 1'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, d_n_1 AS scnd FROM expected$$,
    'directed => true, cap => 1'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, u_n_1 AS scnd FROM expected$$,
    'directed => false, cap => 1'
);


SELECT * from check_expected(
    $$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, d_w_2 AS scnd FROM expected$$,
    'directed => true, cap => 2'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, u_w_2 AS scnd FROM expected$$,
    'directed => false, cap => 2'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, d_n_2 AS scnd FROM expected$$,
    'directed => true, cap => 2'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, u_n_2 AS scnd FROM expected$$,
    'directed => false, cap => 2'
);

DROP TABLE IF EXISTS expected;
SELECT finish();
