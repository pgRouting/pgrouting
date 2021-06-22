\i setup.sql
UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(11) END;

CREATE OR REPLACE FUNCTION edge_cases()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;

-- 0 edge 0 vertex test

PREPARE q1 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id > 18;
RETURN QUERY
SELECT is_empty('q1', 'q1: Graph with 0 edge and 0 vertex');


PREPARE LengauerTarjanDominatorTree_test1 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'q1',1
);

RETURN QUERY
SELECT is_empty('LengauerTarjanDominatorTree_test1', 'LengauerTarjanDominatorTree_test1: Graph with 0 edge and 0 vertex');


-- single vertex test

PREPARE q2 AS
SELECT id, source, 1 AS target, cost, reverse_cost
FROM edge_table
WHERE id = 1;

RETURN QUERY
SELECT set_eq('q2', $$VALUES (1, 1, 1, 1, 1)$$, 'q2: Graph with only vertex 1');

PREPARE LengauerTarjanDominatorTree_test2 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'q2',1
);
RETURN QUERY
SELECT set_eq('LengauerTarjanDominatorTree_test2', $$VALUES (1, 1, 0)$$, 'LengauerTarjanDominatorTree_test2: single vertex');




-- 2 vertices test (connected)

PREPARE q3 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id = 1;

RETURN QUERY
SELECT set_eq('q3', $$VALUES (1, 1, 2, 1, 1)$$, 'q3: Graph with two connected vertices 1 and 2');

PREPARE LengauerTarjanDominatorTree_test3 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
    'q3',1
);

RETURN QUERY
SELECT set_eq('LengauerTarjanDominatorTree_test3', $$VALUES (1, 1, 0),(2, 2, 1)$$, 'LengauerTarjanDominatorTree_test3: two vertices test');



-- 3 vertices test

PREPARE q4 AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table
WHERE id <= 2;

RETURN QUERY
SELECT set_eq('q4', $$VALUES (1, 1, 2, 1, 1), (2, 2, 3, -1, 1)$$, 'q4: Graph with three vertices 1, 2 and 3');


PREPARE LengauerTarjanDominatorTree_test4 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'q4',1
);

RETURN QUERY
SELECT set_eq('LengauerTarjanDominatorTree_test4', $$VALUES (1, 1, 0),(2, 2, 1),(3, 3, 0)$$, 'LengauerTarjanDominatorTree_test3:three vertices test');


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
    'q5: Graph with four vertices 1, 2, 3 and 4'
);

PREPARE LengauerTarjanDominatorTree_test5 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'q5',1
);

RETURN QUERY
SELECT set_eq('LengauerTarjanDominatorTree_test5', $$VALUES (1, 1, 0),(2, 2, 1),(3, 3, 0), (4, 4, 0)$$, 'LengauerTarjanDominatorTree_test5: four vertices test root as 1');


PREPARE LengauerTarjanDominatorTree_test6 AS
SELECT *
FROM pgr_lengauerTarjanDominatorTree(
'q5',4
);

RETURN QUERY
SELECT set_eq('LengauerTarjanDominatorTree_test6', $$VALUES (1, 1, 2),(2,2,3),(3, 3, 4),(4, 4, 0)$$, 'LengauerTarjanDominatorTree_test6:four vertices test root as 4');

END;
$BODY$
LANGUAGE plpgsql;

SELECT edge_cases();

SELECT * FROM finish();
ROLLBACK;
