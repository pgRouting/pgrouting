\i setup.sql

SELECT plan(3);

SET client_min_messages TO WARNING;

-- TESTING queries on issues related to https://github.com/pgRouting/pgrouting/issues/1002


------ https://github.com/pgRouting/pgrouting/issues/1004
PREPARE issue1004_r AS
SELECT *
FROM (VALUES
    ('v', 4, ARRAY[2,3]::BIGINT[], -1, -1, -1)
) AS t(type, id, contracted_vertices, source, target, cost );


PREPARE issue1004_q AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 2 or id = 3$$,
    ARRAY[1]::integer[], 1, ARRAY[7]::BIGINT[], true);

SELECT set_eq('issue1004_r', 'issue1004_q', 'Contraction any one of the forbidden vertices do not belong to graph');

------ https://github.com/pgRouting/pgrouting/issues/1005
PREPARE issue1005_r AS
SELECT *
FROM (VALUES
    ('e', -1, ARRAY[6, 11]::BIGINT[], 3, 12, 3)
) AS t(type, id, contracted_vertices, source, target, cost );

PREPARE issue1005_q AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 5 OR id = 11 OR id = 13$$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

SELECT set_eq('issue1005_r', 'issue1005_q', 'Contraction any one of the forbidden vertices do not belong to graph');

------ https://github.com/pgRouting/pgrouting/issues/1006
PREPARE issue1006_r AS
SELECT *
FROM (VALUES
    ('e', -1, ARRAY[6,9,11,12]::BIGINT[], 5, 4, 3)
) AS t(type, id, contracted_vertices, source, target, cost );


PREPARE issue1006_q AS
SELECT * FROM pgr_contraction(
    $$SELECT id, source, target, cost  FROM edge_table WHERE id IN (8, 9, 11, 13)
    UNION
    SELECT id, target, source, reverse_cost FROM edge_table WHERE id IN (15, 16) AND reverse_cost > 0;
    $$,
    ARRAY[2]::integer[], 1, ARRAY[]::BIGINT[], true);

SELECT set_eq('issue1006_r', 'issue1006_q', 'Contraction any one of the forbidden vertices do not belong to graph');


SELECT finish();
ROLLBACK;
