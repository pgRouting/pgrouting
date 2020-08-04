\i setup.sql

SELECT plan(8);


CREATE TABLE sample_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO sample_table (source, target, cost, reverse_cost) VALUES
    (3, 6, 20, 15),
    (3, 8, 10, -10),
    (6, 8, -1, 12);

PREPARE q0 AS
SELECT id, source, target, cost, reverse_cost FROM sample_table
ORDER BY id;


PREPARE q1 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    3
);

SELECT set_eq('q1',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 1, 3, 6, 1, 20, 20),
        (3, 1, 3, 8, 2, 10, 10)
    $$,
    'q1: Directed Graph with Root vid 3'
);


PREPARE q2 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    6
);

SELECT set_eq('q2',
    $$VALUES
        (1, 0, 6, 6, -1, 0, 0),
        (2, 1, 6, 3, 1, 15, 15),
        (3, 2, 6, 8, 2, 10, 25)
    $$,
    'q2: Directed Graph with Root vid 6'
);


PREPARE q3 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    6, max_depth => 1
);

SELECT set_eq('q3',
    $$VALUES
        (1, 0, 6, 6, -1, 0, 0),
        (2, 1, 6, 3, 1, 15, 15)
    $$,
    'q3: Directed Graph with Root vid 6 and max_depth 1'
);


PREPARE q4 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    2
);

SELECT set_eq('q4',
    $$VALUES (1, 0, 2, 2, -1, 0, 0)$$,
    '4: Vertex not present in graph'
);


PREPARE q5 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    3, directed => false
);

SELECT set_eq('q5',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 1, 3, 6, 1, 20, 20),
        (3, 2, 3, 8, 3, 12, 32)
    $$,
    'q5: Undirected Graph with Root vid 3'
);


PREPARE q6 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    6, directed => false
);

SELECT set_eq('q6',
    $$VALUES
        (1, 0, 6, 6, -1, 0, 0),
        (2, 1, 6, 3, 1, 20, 20),
        (3, 2, 6, 8, 2, 10, 30)
    $$,
    'q6: Undirected Graph with Root vid 6'
);


PREPARE q7 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    6, directed => false, max_depth => 1
);

SELECT set_eq('q7',
    $$VALUES
        (1, 0, 6, 6, -1, 0, 0),
        (2, 1, 6, 3, 1, 20, 20),
        (3, 1, 6, 8, 3, 12, 12)
    $$,
    'q7: Undirected Graph with Root vid 6 and max_depth 1'
);


PREPARE q8 AS
SELECT * FROM pgr_depthFirstSearch (
    'q0',
    ARRAY[6, 3, 6]
);

SELECT set_eq('q8',
    $$VALUES
        (1, 0, 3, 3, -1, 0, 0),
        (2, 1, 3, 6, 1, 20, 20),
        (3, 1, 3, 8, 2, 10, 10),
        (4, 0, 6, 6, -1, 0, 0),
        (5, 1, 6, 3, 1, 15, 15),
        (6, 2, 6, 8, 2, 10, 25)
    $$,
    'q8: Directed Graph with multiple Root vids'
);


SELECT * FROM finish();
ROLLBACK;
