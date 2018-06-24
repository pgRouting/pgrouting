\i setup.sql

SELECT plan(9);

SET client_min_messages TO WARNING;


-------------------------------------------------------------
-- queries that return r1

PREPARE r1 AS
SELECT path_seq, node::BIGINT, edge::BIGINT, agg_cost FROM ( VALUES
   (1,      2,   4,   0),
   (2,      5,   8,   1),
   (3,      6,   9,   2),
   (4,      9,  15,   3),
   (5,     12,  -1,   4))
AS t(path_seq, node, edge, agg_cost);

PREPARE q1 AS
SELECT path_seq, node, edge, agg_cost
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id < 3',
    2, 12,
    1
);


PREPARE q2 AS
SELECT  path_seq, node, edge, agg_cost
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id < 3',
    2, 12,
    1,
    stop_on_first:=false
);

SELECT set_eq('q1', 'r1', 'Should return r1 result (one cycle)');
SELECT set_eq('q2', 'r1', 'Should return r1 result (one cycle, Dont stop_on_first)');

-------------------------------------------------------------
-- queries that return r2

PREPARE r2 AS
SELECT path_seq, node::BIGINT, edge::BIGINT, agg_cost FROM ( VALUES
   (1,      2,   4,   0),
   (2,      5,   8,   1),
   (3,      6,   9,   2),
   (4,      9,  15,   3),
   (5,     12,  -1,   4),
   (1,      2,   4,   0),
   (2,      5,  10,   1),
   (3,     10,  12,   2),
   (4,     11,  13,   3),
   (5,     12,  -1,   4))
AS t(path_seq, node, edge, agg_cost);

PREPARE q3 AS
SELECT path_seq, node, edge, agg_cost
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id < 3',
    2, 12,
    3,
    stop_on_first:=false
);

SELECT set_eq('q3', 'r2', 'Should return r2 result (3 cycle, Dont stop_on_first)');

-------------------------------------------------------------
-- queries that return r3

PREPARE r3 AS
SELECT path_seq, node::BIGINT, edge::BIGINT FROM ( VALUES
   (1,    2,    4),
   (3,    6,    9))
AS t(path_seq, node, edge);


PREPARE q4 AS
SELECT path_seq, node, edge
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    1
) WHERE agg_cost = FLOAT8 '+infinity';

PREPARE q7 AS
SELECT path_seq, node, edge
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    1,
    heap_paths:=true
)  WHERE agg_cost = FLOAT8 '+infinity';

SELECT set_eq('q4', 'r3', 'Should return r3 result (1 cycle)');
SELECT set_eq('q7', 'r3', 'Should return r3 result (1 cycle, Heap paths)');

-------------------------------------------------------------
-- queries that return r4

PREPARE r4 AS
SELECT path_seq, node::BIGINT, edge::BIGINT FROM ( VALUES
   (2,    5,    10))
AS t(path_seq, node, edge);

PREPARE q5 AS
SELECT path_seq, node, edge
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    2,
    stop_on_first:=false
) WHERE agg_cost = FLOAT8 '+infinity';



PREPARE q6 AS
SELECT path_seq, node, edge
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    3,
    stop_on_first:=false
) WHERE agg_cost = FLOAT8 '+infinity';

SELECT set_eq('q5', 'r4', 'q5 Should return r4 result (2 cycle, Dont stop_on_first)');
SELECT set_eq('q6', 'r4', 'q6 Should return r4 result (2 cycle, Dont stop_on_first)');


----------------------------------------
-- queries that return r5

PREPARE r5 AS
SELECT path_seq, node::BIGINT, edge::BIGINT FROM ( VALUES
   (1,    2,    4),
   (1,    2,    4),
   (2,    5,   10),
   (2,    5,    8),
   (3,    6,    9))
AS t(path_seq, node, edge);

PREPARE q8 AS
SELECT path_seq, node, edge
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    2,
    heap_paths:=true,
    stop_on_first:=false
) WHERE agg_cost = FLOAT8 '+infinity';

PREPARE q9 AS
SELECT path_seq, node, edge
FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12,
    3,
    heap_paths:=true,
    stop_on_first:=false
) WHERE agg_cost = FLOAT8 '+infinity';

SELECT set_eq('q8', 'r5', 'Should return r4 result (2 cycle, Dont stop_on_first, heap paths)');
SELECT set_eq('q9', 'r5', 'Should return r4 result (3 cycle, Dont stop_on_first, heap paths)');


SELECT finish();
ROLLBACK;
