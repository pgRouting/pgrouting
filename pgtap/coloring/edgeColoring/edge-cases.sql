\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.3.0') THEN plan(1) ELSE plan(27) END;

CREATE OR REPLACE FUNCTION edge_cases()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.3.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.3.0');
  RETURN;
END IF;

-- 0 edge, 0 vertex test

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;

RETURN QUERY
SELECT is_empty('q1', 'Graph with 0 edge and 0 vertex');

PREPARE edgeColoring1 AS
SELECT * FROM pgr_edgeColoring('q1');

RETURN QUERY
SELECT is_empty('edgeColoring1', 'Graph with 0 edge and 0 vertex -> Empty row is returned');


-- 1 vertex test

PREPARE q2 AS
SELECT id, source, 2 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 2;

PREPARE edgeColoring2 AS
SELECT * FROM pgr_edgeColoring('q2');

RETURN QUERY
SELECT is_empty('edgeColoring2', 'One vertex graph can not be edgeColored -> Empty row is returned');


-- 2 vertices test (connected)

PREPARE q3 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 7;

RETURN QUERY
SELECT set_eq('q3', $$VALUES (7, 8, 5, 1, 1)$$, 'Graph with two connected vertices 8 and 5');

PREPARE edgeColoring3 AS
SELECT * FROM pgr_edgeColoring('q3');

RETURN QUERY
SELECT set_eq('edgeColoring3', $$VALUES (7, 1)$$, 'Edge is colored with color 1');


-- linear tests

-- 3 vertices test

PREPARE q4 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 2;

RETURN QUERY
SELECT set_eq('q4', $$VALUES (1, 1, 2, 1, 1), (2, 2, 3, -1, 1)$$, 'Graph with three vertices 1, 2 and 3');

PREPARE edgeColoring4 AS
SELECT * FROM pgr_edgeColoring('q4');

RETURN QUERY
SELECT set_eq('edgeColoring4', $$VALUES (1, 1), (2, 2)$$, 'Two colors are required');


-- 4 vertices test

PREPARE q5 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 3;

RETURN QUERY
SELECT set_eq('q5',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 3, -1, 1),
        (3, 3, 4, -1, 1)
    $$,
    'Graph with four vertices 1, 2, 3 and 4'
);

PREPARE edgeColoring5 AS
SELECT * FROM pgr_edgeColoring('q5');

RETURN QUERY
SELECT set_eq('edgeColoring5', $$VALUES (1, 1), (2, 2), (3, 3)$$, 'Three colors are required');


-- even length cycle test

-- 4 vertices length

PREPARE q6 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id IN (8, 10, 11, 12);

RETURN QUERY
SELECT set_eq('q6',
    $$VALUES
        (8, 5, 6, 1, 1),
        (10, 5, 10, 1, 1),
        (11, 6, 11, 1, -1),
        (12, 10, 11, 1, -1)
    $$,
    'Graph with four vertices 5, 6, 10 and 11 (cyclic)'
);

PREPARE edgeColoring6 AS
SELECT * FROM pgr_edgeColoring('q6');

RETURN QUERY
SELECT set_eq('edgeColoring6', $$VALUES (8, 1), (10, 3), (11, 3), (12, 2)$$, 'Three colors are required');


-- odd length cycle test

-- 3 vertices cyclic

CREATE TABLE three_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO three_vertices_table (source, target, cost, reverse_cost) VALUES
    (3, 6, 20, 15),
    (3, 8, 10, -10),
    (6, 8, -1, 12);

PREPARE q7 AS
SELECT id, source, target, cost, reverse_cost
FROM three_vertices_table;

RETURN QUERY
SELECT set_eq('q7',
    $$VALUES
        (1, 3, 6, 20, 15),
        (2, 3, 8, 10, -10),
        (3, 6, 8, -1, 12)
    $$,
    'Cyclic Graph with three vertices 3, 6 and 8'
);

PREPARE edgeColoring7 AS
SELECT * FROM pgr_edgeColoring('q7');

RETURN QUERY
SELECT set_eq('edgeColoring7', $$VALUES (1, 1), (2, 2), (3, 3)$$, 'Three colors are required');


-- 5 vertices cyclic

CREATE TABLE five_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO five_vertices_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (2, 3, 1, 1),
    (3, 4, 1, -1),
    (4, 5, 1, 1),
    (5, 1, 1, -1);
    
PREPARE q8 AS
SELECT id, source, target, cost, reverse_cost
FROM five_vertices_table;

RETURN QUERY
SELECT set_eq('q8',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 3, 1, 1),
        (3, 3, 4, 1, -1),
        (4, 4, 5, 1, 1),
        (5, 5, 1, 1, -1)
    $$,
    'Cyclic Graph with five vertices 1, 2, 3, 4 and 5'
);

PREPARE edgeColoring8 AS
SELECT * FROM pgr_edgeColoring('q8');

RETURN QUERY
SELECT set_eq('edgeColoring8', $$VALUES (1, 1), (2, 2), (3, 3), (4, 1), (5, 2)$$, 'Three colors are required');


-- self loop test

-- 1 vertex self loop

CREATE TABLE one_vertex_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO one_vertex_table (source, target, cost, reverse_cost) VALUES
    (1, 1, 1, 1);

PREPARE q9 AS
SELECT id, source, target, cost, reverse_cost
FROM one_vertex_table;

RETURN QUERY
SELECT set_eq('q9',
    $$VALUES
        (1, 1, 1, 1, 1)
    $$,
    'Self loop Graph with one vertex 1'
);

PREPARE edgeColoring9 AS
SELECT * FROM pgr_edgeColoring('q9');

RETURN QUERY
SELECT is_empty('edgeColoring9', 'One vertex self-loop graph can not be edgeColored -> Empty row is returned');


-- 2 vertex self loop

CREATE TABLE two_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO two_vertices_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (2, 2, 1, 1);

PREPARE q10 AS
SELECT id, source, target, cost, reverse_cost
FROM two_vertices_table;

RETURN QUERY
SELECT set_eq('q10',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 2, 1, 1)
    $$,
    'Self loop Graph with two vertices 1 and 2'
);

PREPARE edgeColoring10 AS
SELECT * FROM pgr_edgeColoring('q10');

RETURN QUERY
SELECT set_eq('edgeColoring10', $$VALUES (1, 1)$$, 'One color is required');


-- 7 vertices tree

CREATE TABLE seven_vertices_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO seven_vertices_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (1, 4, 1, 1),
    (2, 3, 1, 1),
    (2, 5, 1, 1),
    (4, 6, 1, 1),
    (4, 7, 1, 1);

PREPARE q11 AS
SELECT id, source, target, cost, reverse_cost
FROM seven_vertices_table;

RETURN QUERY
SELECT set_eq('q11',
    $$VALUES
    (1, 1, 2, 1, 1),
    (2, 1, 4, 1, 1),
    (3, 2, 3, 1, 1),
    (4, 2, 5, 1, 1),
    (5, 4, 6, 1, 1),
    (6, 4, 7, 1, 1)
    $$,
    'A tree Graph with seven vertices 1, 2, 3, 4, 5, 6 and 7'
);

PREPARE edgeColoring11 AS
SELECT * FROM pgr_edgeColoring('q11');

RETURN QUERY
SELECT set_eq('edgeColoring11', $$VALUES (1, 1), (2, 2), (3, 3), (4, 2), (5, 3), (6, 1)$$, 'Three colors are required');


-- 3 vertices multiple edge

CREATE TABLE multiple_edge_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO multiple_edge_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (2, 3, 1, 1),
    (3, 2, 1, 1);

PREPARE q12 AS
SELECT id, source, target, cost, reverse_cost
FROM multiple_edge_table;

RETURN QUERY
SELECT set_eq('q12',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 3, 1, 1),
        (3, 3, 2, 1, 1)
    $$,
    'Multiple edge Graph with three vertices 1, 2 and 3'
);

PREPARE edgeColoring12 AS
SELECT * FROM pgr_edgeColoring('q12');

RETURN QUERY
SELECT set_eq('edgeColoring12', $$VALUES (1, 1), (2, 2)$$, 'Two colors are required');


-- 2 vertices multiple edge

CREATE TABLE two_vertices_multiple_edge_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO two_vertices_multiple_edge_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (2, 1, 1, 1);

PREPARE q13 AS
SELECT id, source, target, cost, reverse_cost
FROM two_vertices_multiple_edge_table;

RETURN QUERY
SELECT set_eq('q13',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 2, 1, 1, 1)
    $$,
    'Multiple edge Graph with two vertices 1 and 2'
);

PREPARE edgeColoring13 AS
SELECT * FROM pgr_edgeColoring('q13');

RETURN QUERY
SELECT set_eq('edgeColoring13', $$VALUES (1, 1)$$, 'One color is required');


-- 4 vertices disconnected graph

CREATE TABLE disconnected_graph_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO disconnected_graph_table (source, target, cost, reverse_cost) VALUES
    (1, 2, 1, 1),
    (3, 4, 1, 1);

PREPARE q14 AS
SELECT id, source, target, cost, reverse_cost
FROM disconnected_graph_table;

RETURN QUERY
SELECT set_eq('q14',
    $$VALUES
        (1, 1, 2, 1, 1),
        (2, 3, 4, 1, 1)
    $$,
    'Disconnected Graph with four vertices 1, 2, 3 and 4'
);

PREPARE edgeColoring14 AS
SELECT * FROM pgr_edgeColoring('q14');

RETURN QUERY
SELECT set_eq('edgeColoring14', $$VALUES (1, 1), (2, 1)$$, 'One color is required');


END;
$BODY$
LANGUAGE plpgsql;

SELECT edge_cases();


SELECT * FROM finish();
ROLLBACK;
