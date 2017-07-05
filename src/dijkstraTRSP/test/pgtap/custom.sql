\i setup.sql

SELECT plan(1);

SET client_min_messages TO WARNING;

PREPARE q1 AS
SELECT seq,
       edge
FROM pgr_dijkstraTRSP('SELECT id, source, target, cost, reverse_cost
FROM edge_table', 'SELECT * FROM restrict WHERE id in (1)', 2, 8);


PREPARE q2 AS
SELECT seq, edge FROM ( VALUES
	(1, 4),
	(2, 8),
	(3, 11),
	(4, 12),
	(5, 10),
	(6, 7) )
AS t(seq, edge);

SELECT set_eq('q1', 'q2', 'Testing from source 2 to target 8 with restriction on edges as follows 4->7');

SELECT finish();
ROLLBACK;
