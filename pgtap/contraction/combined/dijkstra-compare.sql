
\i setup.sql

SELECT plan(334);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

--step 1: Initial tables

SELECT isnt_empty($$SELECT id, source, target, cost, reverse_cost FROM edge_table$$);
SELECT isnt_empty($$SELECT id FROM edge_table_vertices_pgr$$);


-- add extra columns to the edges and vertices table
ALTER TABLE edge_table
ADD is_contracted BOOLEAN DEFAULT false,
ADD contracted_vertices integer[];

ALTER TABLE edge_table_vertices_pgr
ADD is_contracted BOOLEAN DEFAULT false,
ADD contracted_vertices integer[];

SELECT has_column('edge_table', 'is_contracted');
SELECT has_column('edge_table_vertices_pgr', 'is_contracted');
SELECT has_column('edge_table', 'contracted_vertices');
SELECT has_column('edge_table_vertices_pgr', 'contracted_vertices');

SELECT * INTO contraction_info FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1,2]::integer[], 1, ARRAY[]::BIGINT[], false);

PREPARE c_info AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('v', 5, ARRAY[7, 8], -1, -1, '-1.000'),
    ('v', 15, ARRAY[14], -1, -1, '-1.000'),
    ('v', 17, ARRAY[16], -1, -1, '-1.000'),
    ('e', -3, ARRAY[10, 13], 5, 11, '2.244'),
    ('e', -4, ARRAY[12], 9, 11, '2.394'),
    ('e', -1, ARRAY[1, 2], 3, 5, '2.020'),
    ('e', -2, ARRAY[4], 3, 9, '2.265')
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq($$SELECT type, id, contracted_vertices, source, target, round(cost::numeric,3)::TEXT FROM contraction_info$$, 'c_info');

-- add the new edges
INSERT INTO edge_table(source, target, cost, reverse_cost, contracted_vertices, is_contracted)
SELECT source, target, cost, -1, contracted_vertices, true
FROM contraction_info
WHERE type = 'e';

-- Indicate vertices that were contracted
UPDATE edge_table_vertices_pgr
SET is_contracted = true
WHERE id IN (SELECT  unnest(contracted_vertices) FROM  contraction_info);

-- add the contracted vertices on the vertices table
UPDATE edge_table_vertices_pgr
SET contracted_vertices = contraction_info.contracted_vertices
FROM contraction_info
WHERE type = 'v' AND edge_table_vertices_pgr.id = contraction_info.id;

SELECT id
    FROM edge_table
    WHERE is_contracted;

SELECT set_eq($$SELECT id
    FROM edge_table
    WHERE NOT is_contracted$$,
    $$SELECT unnest(ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18])$$
);

SELECT set_eq($$SELECT id
    FROM edge_table
    WHERE is_contracted$$,
    $$SELECT unnest(ARRAY[19,20, 21, 22])$$
);

-- the contracted graph
PREPARE c_graph AS
SELECT source, target, round(cost::numeric,3)::TEXT AS cost, round(reverse_cost::numeric,3)::TEXT AS reverse_cost FROM edge_table
WHERE
    EXISTS (SELECT id FROM edge_table_vertices_pgr AS v WHERE NOT is_contracted AND v.id = edge_table.source)
    AND
    EXISTS (SELECT id FROM edge_table_vertices_pgr AS v WHERE NOT is_contracted AND v.id = edge_table.target);

PREPARE c_expected_graph AS
SELECT source, target, cost, reverse_cost
FROM (VALUES
    (3,      6, '1.025',       '-0.975'),
    (5,      6, '1.064',        '1.064'),
    (6,      9, '1.081',        '1.081'),
    (6,     11, '1.121',       '-0.879'),
    (5,     11, '2.244',        '-1.000'),
    (3,      9, '2.265',        '-1.000'),
    (9,     11, '2.394',        '-1.000'),
    (3,      5, '2.020',        '-1.000'))
AS t(source, target, cost, reverse_cost);

SELECT set_eq('c_graph', 'c_expected_graph');

CREATE OR REPLACE FUNCTION compare_dijkstra(
    BIGINT, BIGINT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
    graph_q TEXT;
BEGIN
    graph_q := format($query$
            WITH
            contracted_section AS (
                SELECT unnest(contracted_vertices) AS id
                FROM edge_table_vertices_pgr
                WHERE ARRAY[%1$s, %2$s] && contracted_vertices
                UNION
                SELECT unnest(contracted_vertices) AS id
                FROM edge_table
                WHERE ARRAY[%1$s, %2$s] && contracted_vertices
            ),
            vertices AS (
                SELECT id FROM edge_table_vertices_pgr
                WHERE NOT is_contracted OR id IN (SELECT id FROM contracted_section)
            )
            SELECT id, source, target, cost, reverse_cost FROM edge_table
            WHERE
            EXISTS (SELECT id FROM vertices WHERE vertices.id = edge_table.source)
            AND
            EXISTS (SELECT id FROM vertices WHERE vertices.id = edge_table.target)
            $query$, $1::BIGINT, $2::BIGINT);
    -- set client_min_messages TO notice;
    -- raise notice '%', graph_q;
    CREATE TEMP TABLE nodes_on_graph AS
    WITH dijkstra_r AS (
        SELECT edge, node FROM pgr_dijkstra(graph_q, $1::BIGINT, $2::BIGINT, false))
    SELECT node FROM dijkstra_r
    UNION
    SELECT unnest(contracted_vertices)
    FROM dijkstra_r JOIN edge_table ON (edge = id) WHERE is_contracted = true;

    CREATE TEMP TABLE dijkstra_contracted AS
    SELECT * FROM pgr_dijkstra($$SELECT id, source, target, cost, reverse_cost
        FROM edge_table
        WHERE
            contracted_vertices IS NULL
            AND source IN (SELECT * FROM nodes_on_graph)
            AND target IN (SELECT * FROM nodes_on_graph)$$, $1::BIGINT, $2::BIGINT, false);

    CREATE TEMP TABLE dijkstra_normal AS
    SELECT * FROM pgr_dijkstra($$SELECT id, source, target, cost, reverse_cost
        FROM edge_table WHERE id < 19$$, $1::BIGINT, $2::BIGINT, false);
    RETURN QUERY
    SELECT set_eq(
        $$SELECT * FROM dijkstra_contracted$$,
        $$SELECT * FROM dijkstra_normal$$,
        'From ' || $1 || ' to ' || $2 );

    DROP TABLE dijkstra_contracted;
    DROP TABLE dijkstra_normal;
    DROP TABLE nodes_on_graph;
END
$BODY$
LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION compare_dijkstra_all()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
    i INTEGER;
    j INTEGER;

BEGIN
    FOR i IN 1..18 LOOP
        FOR j IN 1..18 LOOP
            RETURN QUERY
            SELECT compare_dijkstra(i, j);
        END LOOP;
    END LOOP;
END
$BODY$
LANGUAGE plpgsql;

SELECT compare_dijkstra_all();


