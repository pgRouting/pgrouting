
\echo -- q00
SELECT id, source, target, cost, reverse_cost FROM edge_table;
\echo -- q01
\echo -- q1
ALTER TABLE edge_table_vertices_pgr ADD is_contracted BOOLEAN DEFAULT false;
ALTER TABLE edge_table_vertices_pgr ADD contracted_vertices BIGINT[];
ALTER TABLE edge_table ADD is_new BOOLEAN DEFAULT false;
ALTER TABLE edge_table ADD contracted_vertices BIGINT[];

\echo -- q2
SELECT * FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    array[1,2], directed:=false);

\echo -- q3
SELECT * INTO contraction_results
FROM pgr_contraction(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    array[1,2], directed:=false);

\echo -- q4
UPDATE edge_table_vertices_pgr
SET is_contracted = true
WHERE id IN (SELECT  unnest(contracted_vertices) FROM  contraction_results);

\echo -- q5
SELECT id, is_contracted
FROM edge_table_vertices_pgr
ORDER BY id;

\echo -- q6
UPDATE edge_table_vertices_pgr
SET contracted_vertices = contraction_results.contracted_vertices
FROM contraction_results
WHERE type = 'v' AND edge_table_vertices_pgr.id = contraction_results.id;

\echo -- q7
SELECT id, contracted_vertices, is_contracted
FROM edge_table_vertices_pgr
ORDER BY id;

\echo -- q8
INSERT INTO edge_table(source, target, cost, reverse_cost, contracted_vertices, is_new)
SELECT source, target, cost, -1, contracted_vertices, true
FROM contraction_results
WHERE type = 'e';

\echo -- q9
SELECT id, source, target, cost, reverse_cost, contracted_vertices, is_new
FROM edge_table
ORDER BY id;

\echo -- q10
SELECT id
FROM edge_table_vertices_pgr
WHERE is_contracted = false
ORDER BY id;

\echo -- q11
WITH
vertices_in_graph AS (
    SELECT id
    FROM edge_table_vertices_pgr
    WHERE is_contracted = false
)
SELECT id, source, target, cost, reverse_cost, contracted_vertices
FROM edge_table
WHERE source IN (SELECT * FROM vertices_in_graph)
AND target IN (SELECT * FROM vertices_in_graph);


\echo -- case1
CREATE OR REPLACE FUNCTION my_dijkstra(
    departure BIGINT, destination BIGINT,
    OUT seq INTEGER, OUT path_seq INTEGER,
    OUT node BIGINT, OUT edge BIGINT,
    OUT cost FLOAT, OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT * FROM pgr_dijkstra(
    $contractedGraph$
    WITH
    vertices_in_graph AS (
        SELECT id  FROM edge_table_vertices_pgr WHERE is_contracted = false)
    SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE source IN (SELECT * FROM vertices_in_graph)
    AND target IN (SELECT * FROM vertices_in_graph)
    $contractedGraph$,
    departure, destination, false);
$BODY$
LANGUAGE SQL VOLATILE;

\echo -- use1
SELECT * FROM my_dijkstra(3, 11);

\echo -- case2
SELECT id
FROM edge_table
WHERE ARRAY[4]::BIGINT[] <@ contracted_vertices OR ARRAY[12]::BIGINT[] <@ contracted_vertices;

CREATE OR REPLACE FUNCTION my_dijkstra(
    departure BIGINT, destination BIGINT,
    OUT seq INTEGER, OUT path_seq INTEGER,
    OUT node BIGINT, OUT edge BIGINT,
    OUT cost FLOAT, OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT * FROM pgr_dijkstra(
    $$
    WITH
    edges_to_expand AS (
        SELECT id
        FROM edge_table
        WHERE ARRAY[$$ || departure || $$]::BIGINT[] <@ contracted_vertices
           OR ARRAY[$$ || destination || $$]::BIGINT[] <@ contracted_vertices
    ),

    vertices_in_graph AS (
        SELECT id
        FROM edge_table_vertices_pgr
        WHERE is_contracted = false

        UNION

        SELECT unnest(contracted_vertices)
        FROM edge_table
        WHERE id IN (SELECT id FROM edges_to_expand)
    )

    SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE source IN (SELECT * FROM vertices_in_graph)
    AND target IN (SELECT * FROM vertices_in_graph)
    $$,
    departure, destination, false);
$BODY$
LANGUAGE SQL VOLATILE;

\echo -- use2
SELECT * FROM my_dijkstra(3, 11);
SELECT * FROM my_dijkstra(4, 11);

\echo -- case3
CREATE OR REPLACE FUNCTION my_dijkstra(
    departure BIGINT, destination BIGINT,
    OUT seq INTEGER, OUT path_seq INTEGER,
    OUT node BIGINT, OUT edge BIGINT,
    OUT cost FLOAT, OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT * FROM pgr_dijkstra(
    $$
    WITH
    edges_to_expand AS (
        SELECT id
        FROM edge_table
        WHERE ARRAY[$$ || departure || $$]::BIGINT[] <@ contracted_vertices
           OR ARRAY[$$ || destination || $$]::BIGINT[] <@ contracted_vertices
    ),

    vertices_to_expand AS (
        SELECT id
        FROM edge_table_vertices_pgr
        WHERE ARRAY[$$ || departure || $$]::BIGINT[] <@ contracted_vertices
           OR ARRAY[$$ || destination || $$]::BIGINT[] <@ contracted_vertices
    ),

    vertices_in_graph AS (
        SELECT id
        FROM edge_table_vertices_pgr
        WHERE is_contracted = false

        UNION

        SELECT unnest(contracted_vertices)
        FROM edge_table
        WHERE id IN (SELECT id FROM edges_to_expand)

        UNION

        SELECT unnest(contracted_vertices)
        FROM edge_table_vertices_pgr
        WHERE id IN (SELECT id FROM vertices_to_expand)
    )

    SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE source IN (SELECT * FROM vertices_in_graph)
    AND target IN (SELECT * FROM vertices_in_graph)
    $$,
    departure, destination, false);
$BODY$
LANGUAGE SQL VOLATILE;

\echo -- use3
SELECT * FROM my_dijkstra(3, 11);
SELECT * FROM my_dijkstra(4, 11);
SELECT * FROM my_dijkstra(4, 7);

\echo -- case4

SELECT * FROM  pgr_dijkstra(
    $$
    WITH
    expand_vertices AS (SELECT id, unnest(contracted_vertices) AS vertex FROM edge_table_vertices_pgr),
    expand7 AS (SELECT contracted_vertices FROM edge_table_vertices_pgr
        WHERE id IN (SELECT id FROM expand_vertices WHERE vertex = 7)),
    vertices_in_graph AS (
        SELECT id  FROM edge_table_vertices_pgr WHERE is_contracted = false
        UNION
        SELECT unnest(contracted_vertices) FROM expand7)
    SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE source IN (SELECT * FROM vertices_in_graph)
    AND target IN (SELECT * FROM vertices_in_graph)
    $$,
    3, 7, false);


\echo -- case5q1
-- Edges that need expansion and the vertices to be expanded.
WITH
first_dijkstra AS (
    SELECT * FROM  pgr_dijkstra(
        $$
        WITH
        expand_vertices AS (SELECT id, unnest(contracted_vertices) AS vertex FROM edge_table_vertices_pgr),
        expand7 AS (SELECT contracted_vertices FROM edge_table_vertices_pgr
            WHERE id IN (SELECT id FROM expand_vertices WHERE vertex = 7)),
        vertices_in_graph AS (
            SELECT id  FROM edge_table_vertices_pgr WHERE is_contracted = false
            UNION
            SELECT unnest(contracted_vertices) FROM expand7)
        SELECT id, source, target, cost, reverse_cost
        FROM edge_table
        WHERE source IN (SELECT * FROM vertices_in_graph)
        AND target IN (SELECT * FROM vertices_in_graph)
        $$,
        3, 7, false))

SELECT edge, contracted_vertices
    FROM first_dijkstra JOIN edge_table
    ON (edge = id)
    WHERE is_new = true;

\echo -- case5q2

SELECT * FROM pgr_dijkstra($$
    WITH
    -- This returns the results from case 2
    first_dijkstra AS (
        SELECT * FROM  pgr_dijkstra(
            '
            WITH
            expand_vertices AS (SELECT id, unnest(contracted_vertices) AS vertex FROM edge_table_vertices_pgr),
            expand7 AS (SELECT contracted_vertices FROM edge_table_vertices_pgr
                WHERE id IN (SELECT id FROM expand_vertices WHERE vertex = 7)),
            vertices_in_graph AS (
                SELECT id  FROM edge_table_vertices_pgr WHERE is_contracted = false
                UNION
                SELECT unnest(contracted_vertices) FROM expand7)
            SELECT id, source, target, cost, reverse_cost
            FROM edge_table
            WHERE source IN (SELECT * FROM vertices_in_graph)
            AND target IN (SELECT * FROM vertices_in_graph)
            ',
            3, 7, false)),

    -- edges that need expansion and the vertices to be expanded.
    edges_to_expand AS (
        SELECT edge, contracted_vertices
        FROM first_dijkstra JOIN edge_table
        ON (edge = id)
        WHERE is_new = true),

    vertices_in_graph AS (
        -- the nodes of the contracted solution
        SELECT node FROM first_dijkstra
        UNION
        -- the nodes of the expanding sections
        SELECT unnest(contracted_vertices) FROM edges_to_expand)

    SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE source IN (SELECT * FROM vertices_in_graph)
    AND target IN (SELECT * FROM vertices_in_graph)
    -- not including the expanded edges
    AND id NOT IN (SELECT edge FROM edges_to_expand)
    $$,
    3, 7, false);

\echo -- end
