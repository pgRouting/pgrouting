\i setup.sql

SELECT plan(34);

-- testing from an existing starting vertex to an non-existing destination in directed graph
-- expecting results: empty set
PREPARE q1 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    2, 3
);

-- testing from an existing starting vertex to an non-existing destination in undirected graph
-- expecting results: empty set
PREPARE q2 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    2, 3,
    FALSE
);

-- testing from an existing starting vertex to an non-existing destination in directed graph
-- expecting results: empty set
PREPARE q3 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    2, 3
);

-- testing from an existing starting vertex to an non-existing destination in undirected graph
-- expecting results: empty set
PREPARE q4 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    2, 3,
    FALSE
);

-- testing from an non-existing starting vertex to an existing destination in directed graph
-- expecting results: empty set
PREPARE q5 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    6, 8
);

-- testing from an non-existing starting vertex to an existing destination in undirected graph
-- expecting results: empty set
PREPARE q6 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions',
    6, 8,
    FALSE
);

-- testing from an non-existing starting vertex to an existing destination in directed graph
-- expecting results: empty set
PREPARE q7 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    6, 8
);

-- testing from an non-existing starting vertex to an existing destination in undirected graph
-- expecting results: empty set
PREPARE q8 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    6, 8,
    FALSE
);

-- testing from an non-existing starting vertex to a non-existing destination in directed graph
-- expecting results: empty set
PREPARE q9 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 3
);

PREPARE q10 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 4
);

PREPARE q11 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 6
);

PREPARE q12 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 7
);

PREPARE q13 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 9
);

PREPARE q14 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 10
);

PREPARE q15 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 11
);

PREPARE q16 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 12
);

PREPARE q17 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 13
);

PREPARE q18 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 14
);

PREPARE q19 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 15
);

PREPARE q20 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 16
);

PREPARE q21 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 17
);

-- testing from an non-existing starting vertex to a non-existing destination in undirected graph
-- expecting results: empty set
PREPARE q22 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 3,
    FALSE
);

PREPARE q23 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 4,
    FALSE
);

PREPARE q24 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 6,
    FALSE
);

PREPARE q25 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 7,
    FALSE
);

PREPARE q26 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 9,
    FALSE
);

PREPARE q27 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 10,
    FALSE
);

PREPARE q28 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 11,
    FALSE
);

PREPARE q29 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 12,
    FALSE
);

PREPARE q30 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 13,
    FALSE
);

PREPARE q31 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 14,
    FALSE
);

PREPARE q32 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 15,
    FALSE
);

PREPARE q33 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 16,
    FALSE
);

PREPARE q34 AS
SELECT * FROM pgr_dijkstraTRSP(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7',
    'SELECT * FROM restrictions where id > 10',
    1, 17,
    FALSE
);

SELECT is_empty('q1');
SELECT is_empty('q2');
SELECT is_empty('q3');
SELECT is_empty('q4');
SELECT is_empty('q5');
SELECT is_empty('q6');
SELECT is_empty('q7');
SELECT is_empty('q8');
SELECT is_empty('q9');
SELECT is_empty('q10');
SELECT is_empty('q11');
SELECT is_empty('q12');
SELECT is_empty('q13');
SELECT is_empty('q14');
SELECT is_empty('q15');
SELECT is_empty('q16');
SELECT is_empty('q17');
SELECT is_empty('q18');
SELECT is_empty('q19');
SELECT is_empty('q20');
SELECT is_empty('q21');
SELECT is_empty('q22');
SELECT is_empty('q23');
SELECT is_empty('q24');
SELECT is_empty('q25');
SELECT is_empty('q26');
SELECT is_empty('q27');
SELECT is_empty('q28');
SELECT is_empty('q29');
SELECT is_empty('q30');
SELECT is_empty('q31');
SELECT is_empty('q32');
SELECT is_empty('q33');
SELECT is_empty('q34');

-- q3
SELECT * FROM finish();
ROLLBACK;
