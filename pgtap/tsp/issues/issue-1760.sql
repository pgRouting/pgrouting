
\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(11);

DROP TABLE IF EXISTS tsp_issue_numb;
CREATE TABLE tsp_issue_numb AS
SELECT start_vid, end_vid, agg_cost
FROM (VALUES
    (-2,      -1, 0.005665599374822598),
    (-1,      -2, 0.005665599374822598))
AS t (start_vid, end_vid, agg_cost);

SELECT * FROM tsp_issue_numb;

PREPARE test1 AS
SELECT seq, node
      FROM pgr_TSP(
        $matrix$
        SELECT *
        FROM tsp_issue_numb
        $matrix$,
        start_id => -1
      );


PREPARE result1 AS
SELECT seq::INTEGER, node::BIGINT
FROM (VALUES
   (1,   -1),
   (2,   -2),
   (3,   -1))
AS t (seq, node);

SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');
SELECT set_eq('test1', 'result1');

DROP TABLE tsp_issue_numb;
SELECT finish();


