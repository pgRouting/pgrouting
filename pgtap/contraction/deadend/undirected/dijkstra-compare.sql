\i setup.sql

SELECT plan(333);

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
    ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true);

PREPARE c_info AS
SELECT type, id, contracted_vertices, source, target, cost
FROM (VALUES
    ('v'::CHAR, 2::BIGINT, ARRAY[1]::BIGINT[], -1::BIGINT, -1::BIGINT, -1::FLOAT),
    ('v', 5, ARRAY[7,8], -1, -1, -1),
    ('v', 10, ARRAY[13], -1, -1, -1),
    ('v', 15, ARRAY[14], -1, -1, -1),
    ('v', 17, ARRAY[16], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );

SELECT set_eq($$SELECT * FROM contraction_info$$, 'c_info');


UPDATE edge_table_vertices_pgr
    SET contracted_vertices = result.contracted_vertices
FROM contraction_info AS result
WHERE result.type = 'v' AND result.id = edge_table_vertices_pgr.id;

UPDATE edge_table_vertices_pgr
    SET is_contracted = true
FROM (SELECT unnest(contracted_vertices) AS id FROM contraction_info WHERE type = 'v') AS result
WHERE result.id = edge_table_vertices_pgr.id;

SELECT set_eq($$SELECT id
    FROM edge_table_vertices_pgr
    WHERE NOT is_contracted$$,
    $$SELECT unnest(ARRAY[2,3,4,5,6,9,10,11,12,15,17])$$
);

SELECT set_eq($$SELECT id
    FROM edge_table_vertices_pgr
    WHERE is_contracted$$,
    $$SELECT unnest(ARRAY[1,7,8,13,14,16])$$
);


CREATE OR REPLACE FUNCTION compare_dijkstra(
    BIGINT, BIGINT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
    CREATE TEMP TABLE dijkstra_contracted AS
    SELECT * FROM pgr_dijkstra(
        format($query$
            WITH
            contracted_section AS (
                SELECT unnest(contracted_vertices) AS id
                FROM edge_table_vertices_pgr
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
            $query$, $1::BIGINT, $2::BIGINT),
        $1::BIGINT, $2::BIGINT, directed := false);

    CREATE TEMP TABLE dijkstra_normal AS
    SELECT * FROM pgr_dijkstra($$SELECT id, source, target, cost, reverse_cost FROM edge_table$$, $1::BIGINT, $2::BIGINT, directed := false);

    RETURN QUERY
    SELECT set_eq(
        $$SELECT * FROM dijkstra_contracted$$,
        $$SELECT * FROM dijkstra_normal$$);

    DROP TABLE dijkstra_contracted;
    DROP TABLE dijkstra_normal;
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
