--\i setup.sql

SELECT plan(122);

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);

SELECT is_empty($$SELECT distinct cost FROM edge_table WHERE cost != 1 AND cost != -1$$);
SELECT is_empty($$SELECT distinct reverse_cost FROM edge_table WHERE cost != 1 AND cost != -1$$);

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
a AS (SELECT start_vid, d_w_1, u_w_1, d_n_1, u_n_1
  FROM (VALUES
    (1::BIGINT,
        ARRAY[2]::BIGINT[],   ARRAY[2]::BIGINT[],    ARRAY[2]::BIGINT[],   ARRAY[2]::BIGINT[]),
    (2,  ARRAY[ 1, 5],          ARRAY[ 1, 3, 5],     ARRAY[ 5],     ARRAY[  1, 5]),
    (3,  ARRAY[ 2, 6],          ARRAY[ 2, 4, 6],     ARRAY[ 6],     ARRAY[  6]),
    (4,  ARRAY[ 3, 9],          ARRAY[ 3, 9],        ARRAY[ 9],     ARRAY[  9]),
    (5,  ARRAY[ 2, 6, 8, 10],   ARRAY[ 2, 6, 8, 10], ARRAY[ 6, 10], ARRAY[  2, 6, 8, 10]),
    (6,  ARRAY[ 5, 9, 11],      ARRAY[ 3, 5, 9, 11], ARRAY[ 9, 11], ARRAY[  3, 5, 9, 11]),
    (7,  ARRAY[ 8],             ARRAY[ 8],           ARRAY[ 8],     ARRAY[  8]),
    (8,  ARRAY[ 5, 7],          ARRAY[ 5, 7],        ARRAY[ 5],     ARRAY[  5, 7]),
    (9,  ARRAY[ 4, 6, 12],      ARRAY[ 4, 6, 12],    ARRAY[12],     ARRAY[  4, 6, 12]),
    (10, ARRAY[ 5, 11, 13],     ARRAY[ 5, 11, 13],   ARRAY[11, 13], ARRAY[  5, 11, 13]),
    (11, ARRAY[12],             ARRAY[ 6, 10, 12],   ARRAY[12],     ARRAY[  6, 10, 12]),
    (12, ARRAY[ 9],             ARRAY[9, 11],        NULL,          ARRAY[ 9, 11]),
    (13, ARRAY[10],             ARRAY[10],           NULL,          ARRAY[ 10]),
    (14, ARRAY[15],             ARRAY[15],           ARRAY[15],     ARRAY[ 15]),
    (15, ARRAY[14],             ARRAY[14],           NULL,          ARRAY[ 14]),
    (16, ARRAY[17],             ARRAY[17],           ARRAY[17],     ARRAY[ 17]),
    (17, ARRAY[16],             ARRAY[16],           NULL,          ARRAY[ 16])
  ) t(start_vid, d_w_1, u_w_1, d_n_1, u_n_1))
SELECT * from a;


CREATE OR REPLACE FUNCTION check_expected(edges_q TEXT, expected_q TEXT, optionals TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijstraNear_query TEXT;
inner_query TEXT;
expected_query TEXT;
data record;
dest BIGINT;
id BIGINT;
BEGIN

    FOR data IN EXECUTE expected_q LOOP
        id := data.frst;
        CONTINUE WHEN data.scnd IS NULL;

        FOREACH dest IN ARRAY data.scnd LOOP
            inner_query := format($$
                WITH
                edges AS (%1$s),
                first_edge AS (SELECT *, (source = %2$s AND target = %3$s) OR (source = %3$s AND target = %2$s) AS order_1 FROM edges)
                SELECT * FROM first_edge ORDER BY order_1 DESC
                $$, edges_q, id, dest);
            dijstraNear_query := format($$
                SELECT distinct end_vid
                FROM pgr_dijkstraNear(
                    %1$L,
                    %2$s,
                    (SELECT array_agg(id) FROM edge_table_vertices_pgr where id != %2$s),
                    %3$s)
                $$, inner_query, id, optionals);
            expected_query := format($$
                SELECT %1$s
                $$, dest);

            RETURN QUERY
            SELECT set_eq(dijstraNear_query, expected_query, id::TEXT || '->' || dest || ' ' || optionals);
            END LOOP;
    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE;

SELECT * from check_expected(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id DESC$$,
    $$SELECT start_vid AS frst, d_w_1 AS scnd FROM expected$$,
    'directed => true, cap => 1'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id DESC$$,
    $$SELECT start_vid AS frst, u_w_1 AS scnd FROM expected$$,
    'directed => false, cap => 1'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost  FROM edge_table ORDER BY id DESC$$,
    $$SELECT start_vid AS frst, d_n_1 AS scnd FROM expected$$,
    'directed => true, cap => 1'
);

SELECT * from check_expected(
    $$SELECT id, source, target, cost  FROM edge_table ORDER BY id DESC$$,
    $$SELECT start_vid AS frst, u_n_1 AS scnd FROM expected$$,
    'directed => false, cap => 1'
);

DROP TABLE IF EXISTS expected;
SELECT finish();
