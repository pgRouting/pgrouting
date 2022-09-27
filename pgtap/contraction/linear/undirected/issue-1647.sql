BEGIN;

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);

SELECT CASE WHEN NOT min_version('3.1.1') THEN plan(1) ELSE plan(4) END;

CREATE OR REPLACE FUNCTION issue_check()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.1.1') THEN
  RETURN QUERY SELECT skip(1,'Issue fixed on 3.1.1');
  RETURN;
END IF;

PREPARE allgraph AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    $$SELECT * FROM edge_table$$,
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

PREPARE minigraph AS
SELECT type, id, contracted_vertices, source, target, cost
FROM pgr_contraction(
    $$SELECT * FROM edge_table WHERE source IN(1,2) OR target IN(1,2)$$,
    ARRAY[2]::INTEGER[], 1, ARRAY[]::INTEGER[], false);

RETURN QUERY
SELECT lives_ok('allgraph', 'allgraph QUERY 1: Graph with no loop cycle');
RETURN QUERY
SELECT lives_ok('minigraph', 'minigraph QUERY 1: Graph with no loop cycle');

INSERT INTO edge_table (source, target, cost, reverse_cost) VALUES
(1, 1, 1, 1);

RETURN QUERY
SELECT lives_ok('allgraph', 'allgraph QUERY 1: Graph with no loop cycle');
RETURN QUERY
SELECT lives_ok('minigraph', 'minigraph QUERY 1: Graph with no loop cycle');

END;
$BODY$
LANGUAGE plpgsql;

SELECT issue_check();
SELECT finish();
DELETE FROM edge_table WHERE id > 18;
ROLLBACK;

