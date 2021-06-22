\i setup.sql

SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(142) END;

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;



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


CREATE OR REPLACE FUNCTION check_compare(edges_q TEXT, expected_q TEXT, optionals_n TEXT, optionals_d TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijkstraNear_query TEXT;
dijkstra_query TEXT;
BEGIN
    FOR id IN 1..17 LOOP
        dijkstraNear_query := format($$
            SELECT path_seq, start_vid, end_vid, node, edge, cost, agg_cost
            FROM pgr_dijkstraNear(
                %1$L,
                %2$s,
                (SELECT array_agg(id) FROM edge_table_vertices_pgr where id != %2$s),
                %3$s);
            $$, edges_q, id, optionals_n);
        dijkstra_query := format($$
            SELECT path_seq, start_vid, end_vid, node, edge, cost, agg_cost
            FROM pgr_dijkstra(
                %1$L,
                ARRAY[%2$s]::BIGINT[],
                (SELECT scnd FROM (%3$s) a  WHERE frst = %2$s),
                %4$s
                );
            $$, edges_q, id, expected_q, optionals_d);

        RETURN QUERY
        SELECT set_eq(dijkstraNear_query, dijkstra_query, id::TEXT || ' ' || optionals_d);
    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION compare()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

RETURN QUERY
SELECT isnt_empty($$SELECT id, source, target, cost, reverse_cost FROM edge_table$$);
RETURN QUERY
SELECT isnt_empty($$SELECT id FROM edge_table_vertices_pgr$$);

-- vertex id values that dont exist
RETURN QUERY
SELECT is_empty($$SELECT id FROM edge_table_vertices_pgr WHERE id > 18$$);
RETURN QUERY
SELECT is_empty($$SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE source > 18$$);
RETURN QUERY
SELECT is_empty($$SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE target > 18$$);
RETURN QUERY
SELECT is_empty($$SELECT id FROM edge_table WHERE id > 18$$);

RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    $$SELECT start_vid AS frst, d_w_1 AS scnd FROM expected$$,
    'directed => true, cap => 1', 'directed => true'
);

RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    $$SELECT start_vid AS frst, u_w_1 AS scnd FROM expected$$,
    'directed => false, cap => 1', 'directed => false'
);

RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, d_n_1 AS scnd FROM expected$$,
    'directed => true, cap => 1', 'directed => true'
);

RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, u_n_1 AS scnd FROM expected$$,
    'directed => false, cap => 1', 'directed => false'
);


RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, d_w_2 AS scnd FROM expected$$,
    'directed => true, cap => 2', 'directed => true'
);

RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, u_w_2 AS scnd FROM expected$$,
    'directed => false, cap => 2', 'directed => false'
);

RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, d_n_2 AS scnd FROM expected$$,
    'directed => true, cap => 2', 'directed => true'
);

RETURN QUERY
SELECT * from check_compare(
    $$SELECT id, source, target, cost  FROM edge_table$$,
    $$SELECT start_vid AS frst, u_n_2 AS scnd FROM expected$$,
    'directed => false, cap => 2', 'directed => false'
);

END;
$BODY$
LANGUAGE plpgsql;


SELECT compare();
SELECT finish();
ROLLBACK;
