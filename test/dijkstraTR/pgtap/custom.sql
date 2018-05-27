\i setup.sql

SELECT plan(7);

SET client_min_messages TO WARNING;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT todo_start();


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
	(6, 7),
	(7, -1) )
AS t(seq, edge);

SELECT set_eq('q1', 'q2', 'Testing from source 2 to target 8 with restriction on edges as follows 4 -> 7');



PREPARE q3 AS
SELECT seq,
       edge
FROM pgr_dijkstraTRSP('SELECT id, source, target, cost, reverse_cost
FROM edge_table', 'SELECT * FROM restrict WHERE id in (2)', 8, 11);

PREPARE q4 AS
SELECT seq, edge FROM ( VALUES
	(1, 7),
	(2, 10),
	(3, 12),
	(4, -1) )
AS t(seq, edge);

SELECT set_eq('q3', 'q4', 'Testing from source 8 to target 11 with restriction on edges as follows 7 -> 8 -> 11');



PREPARE q5 AS
SELECT seq,
       edge
FROM pgr_dijkstraTRSP('SELECT id, source, target, cost, reverse_cost
FROM edge_table', 'SELECT * FROM restrict WHERE id in (2, 3)', 7, 3);

PREPARE q6 AS
SELECT seq, edge FROM ( VALUES
	(1, 6),
	(2, 7),
	(3, 8),
	(4, 9),
	(5, 16),
	(6, 3),
	(7, -1) )
AS t(seq, edge);

SELECT set_eq('q5', 'q6', 'Testing from source 7 to target 3 with restriction on edges as follows 7 -> 8 -> 11 and 7 -> 8 -> 5');



PREPARE q7 AS
SELECT seq,
       edge
FROM pgr_dijkstraTRSP('SELECT id, source, target, cost, reverse_cost
FROM edge_table', 'SELECT * FROM restrict WHERE id in (4)', 7, 1);

PREPARE q8 AS
SELECT seq, edge FROM ( VALUES
	(1, 6),
	(2, 7),
	(3, 8),
	(4, 5),
	(5, 2),
	(6, 1),
	(7, -1) )
AS t(seq, edge);

SELECT set_eq('q7', 'q8', 'Testing from source 7 to target 1 with restriction on edges as follows 7 -> 4');



PREPARE q9 AS
SELECT seq,
       edge
FROM pgr_dijkstraTRSP('SELECT id, source, target, cost, reverse_cost
FROM edge_table', 'SELECT * FROM restrict WHERE id in (4, 5)', 8, 4);

PREPARE q10 AS
SELECT seq, edge FROM ( VALUES
	(1, 7),
	(2, 10),
	(3, 12),
	(4, 13),
	(5, 15),
	(6, 16),
	(7, -1) )
AS t(seq, edge);

SELECT set_eq('q9', 'q10', 'Testing from source 8 to target 4 with restriction on edges as follows 7 -> 8 and 7 -> 4');



PREPARE q11 AS
SELECT seq,
       edge
FROM pgr_dijkstraTRSP('SELECT id, source, target, cost, reverse_cost
FROM edge_table', 'SELECT * FROM restrict WHERE id in (4, 5)', 7, 1);

PREPARE q12 AS
SELECT seq, edge FROM ( VALUES
	(1, 6),
	(2, 7),
	(3, 10),
	(4, 12),
	(5, 13),
	(6, 15),
	(7, 9),
	(8, 5),
	(9, 2),
	(10, 1),
	(11, -1) )
AS t(seq, edge);

SELECT set_eq('q11', 'q12', 'Testing from source 7 to target 1 with restriction on edges as follows 7 -> 8 and 7 -> 4');



PREPARE q13 AS
SELECT seq,
       edge
FROM pgr_dijkstraTRSP('SELECT id, source, target, cost, reverse_cost
FROM edge_table', 'SELECT * FROM restrict WHERE id in (6, 7)', 12, 7);

PREPARE q14 AS
SELECT seq, edge FROM ( VALUES
	(1, 15),
	(2, 9),
	(3, 5),
	(4, 2),
	(5, 4),
	(6, 7),
	(7, 6),
	(8, -1) )
AS t(seq, edge);

SELECT set_eq('q13', 'q14', 'Testing from source 12 to target 7 with restriction on edges as follows 9 -> 11 and 9 -> 8');


SELECT todo_end();

SELECT finish();
ROLLBACK;
