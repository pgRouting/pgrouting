
\i setup.sql

SELECT plan(8);



--Examples for queries marked AS ``directed`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------
PREPARE q1 AS
WITH
union_data AS (
    (SELECT 1 AS path_id,* FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            ARRAY[1], ARRAY[3]))

    UNION

    (SELECT 2 AS path_id,* FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            ARRAY[3], ARRAY[11]
))),
data AS (SELECT * FROM union_data ORDER BY path_id, path_seq)

SELECT ((row_number() over())) AS seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
FROM data ORDER BY path_id, path_seq;

PREPARE q2 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[1, 3, 11]
    );

PREPARE q3 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[1, 3, 11],
        directed:=TRUE
    );

SELECT set_eq('q1', 'q2', '1: union of dijkstra is the same AS pgr_dijkstraVia');
SELECT set_eq('q2', 'q3', '2: with directed:=TRUE is the same AS no flag');



-- Examples for queries marked AS ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
PREPARE q11 AS
WITH
union_data AS (
    (SELECT 1 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            ARRAY[1], ARRAY[3],
            FALSE))

    UNION

    (SELECT 2 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            ARRAY[3], ARRAY[11],
            FALSE))),
data AS (SELECT * FROM union_data ORDER BY path_id, path_seq)

SELECT ((row_number() over())) AS seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
FROM data ORDER BY path_id, path_seq;


PREPARE q12 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[1, 3, 11],
        directed:=FALSE
    );

SELECT set_eq('q11','q12', '3: union of dijkstra is the same AS pgr_dijkstraVia');


-- Examples for queries marked AS ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
PREPARE q21 AS
WITH
union_data AS (
    (SELECT 1 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            ARRAY[1], ARRAY[3]))
    UNION

    (SELECT 2 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            ARRAY[3], ARRAY[11]))),
data AS (SELECT * FROM union_data ORDER BY path_id, path_seq)

SELECT ((row_number() over())) AS seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
FROM data ORDER BY path_id, path_seq;



PREPARE q22 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost FROM edge_table',
        array[1, 3, 11]
    );

PREPARE q23 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost FROM edge_table',
        array[1, 3, 11],
        directed:=TRUE
    );


SELECT set_eq('q21','q22', '4: union of dijkstra is the same AS pgr_dijkstraVia');
SELECT set_eq('q22','q23', '5: with directed:=TRUE is the same AS no flag');


-- Examples for queries marked AS ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
PREPARE q31 AS
WITH
union_data AS (
    (SELECT 1 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            ARRAY[1], ARRAY[3],
            FALSE))

    UNION

    (SELECT 2 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            ARRAY[3], ARRAY[11],
            directed:=FALSE))),
data AS (SELECT * FROM union_data ORDER BY path_id, path_seq)

SELECT ((row_number() over())) AS seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
FROM data ORDER BY path_id, path_seq;


PREPARE q32 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost
    FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost FROM edge_table',
        array[1, 3, 11],
        directed:=FALSE
    );

SELECT set_eq('q31','q32', '6: union of dijkstra is the same AS pgr_dijkstraVia');

-- Equvalences between signatures
------------------------------
----------------------------------------------------------------------------------------
PREPARE q41 AS
WITH
union_data AS (
    (SELECT 1 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[1], array[3]))
    UNION
    (SELECT 2 AS path_id, * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[3], array[11] ))),
data AS (SELECT * FROM union_data ORDER BY path_id, path_seq)

SELECT ((row_number() over())) AS seq, path_id, path_seq, start_vid, end_vid, node, edge, cost, agg_cost
FROM data ORDER BY path_id, path_seq;


PREPARE q42 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost
    FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[1, 3, 11]
    );

PREPARE q43 AS
SELECT seq, path_id, path_seq, start_vid, end_vid, node,
CASE WHEN edge = -2 THEN -1 ELSE edge END,
    cost, agg_cost
    FROM pgr_dijkstraVia(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[1, 3, 11],
        directed:=TRUE
    );

SELECT set_eq('q41','q42', '7: union of dijkstra is the same AS pgr_dijkstraVia');
SELECT set_eq('q42','q43', '8: with directed:=TRUE is the same AS no flag');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

