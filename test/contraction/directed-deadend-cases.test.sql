
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
(1, 2, 1, 0, 1),

(1, 2, 0, 1, 2),

(1, 2, 1, 1, 3),

(1, 2, 0, 1, 4),
(1, 2, 0, 1, 4),
(1, 3, 0, 1, 4),

(1, 2, 1, 1, 5),
(1, 2, 1, 1, 5);

SELECT * FROM pgr_contractGraph(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 1)$$,
    ARRAY[1]::integer[]);

SELECT * FROM pgr_contractGraph(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 2)$$,
    ARRAY[1]::integer[]);

SELECT * FROM pgr_contractGraph(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 3)$$,
    ARRAY[1]::integer[]);

SELECT * FROM pgr_contractGraph(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 4)$$,
    ARRAY[1]::integer[]);

SELECT * FROM pgr_contractGraph(
    $$SELECT * FROM test_deadend WHERE dead_case IN (0, 5)$$,
    ARRAY[1]::integer[]);

