\i setup.sql

SELECT plan(6);

CREATE TABLE test_deadend (
    id SERIAL,
    source BIGINT,
    target BIGINT,
    cost BIGINT default 1,
    reverse_cost BIGINT default 1,
    dead_case INTEGER
);

-- 0 for all cases
INSERT INTO test_deadend(source, target, dead_case)
VALUES
(2, 3, 0),
(2, 4, 0),
(3, 4, 0);

INSERT INTO test_deadend(source, target, cost, reverse_cost, dead_case)
VALUES
(1, 2, 1, -1, 1),

(1, 2, -1, 1, 2),

(1, 2, 1, 1, 3),

(1, 2, -1, 1, 4),
(1, 2, -1, 1, 4),
(1, 3, -1, 1, 4),

(2, 1, 1, -1, 41),
(2, 1, 1, -1, 41),
(3, 1, 1, -1, 41),

(1, 2, 1, 1, 5),
(1, 2, 1, 1, 5);

--
prepare q1 AS
SELECT * FROM pgr_contraction(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 1)$$,
    ARRAY[1]::integer[], directed := false);

prepare sol_2_1 AS
SELECT
    'v'::CHAR AS type,
    2::BIGINT AS id,
    ARRAY[1]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost;

SELECT set_eq('q1', 'sol_2_1');

--
prepare q2 AS
SELECT * FROM pgr_contraction(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 2)$$,
    ARRAY[1]::integer[], directed := false);

SELECT set_eq('q2', 'sol_2_1');

--
prepare q3 AS
SELECT * FROM pgr_contraction(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 3)$$,
    ARRAY[1]::integer[]);
SELECT set_eq('q3', 'sol_2_1');

--
prepare q4 AS
SELECT * FROM pgr_contraction(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 4)$$,
    ARRAY[1]::integer[], directed := false);
SELECT is_empty('q4');

--
PREPARE q41 AS
SELECT * FROM pgr_contraction(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 41)$$,
    ARRAY[1]::integer[], directed := false);
SELECT is_empty('q41');

--
PREPARE q5 AS
SELECT * FROM pgr_contraction(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 5)$$,
    ARRAY[1]::integer[], directed := false);

SELECT set_eq('q5', 'sol_2_1');


SELECT finish();

