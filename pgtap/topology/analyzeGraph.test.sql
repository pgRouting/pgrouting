\i setup.sql
SET client_min_messages TO WARNING;

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(3);


SELECT pgr_analyzegraph('edge_table', 0.000001);


SELECT is((SELECT count(*)::INTEGER cnt FROM edge_table_vertices_pgr WHERE chk = 1), 1, '1: One vertex has a problem');
SELECT is((SELECT id::INTEGER FROM edge_table_vertices_pgr WHERE chk = 1), 15, '2: vertex #15 has the problem');

PREPARE q1 AS
SELECT cnt, count(*) AS M  FROM edge_table_vertices_pgr GROUP BY cnt ORDER BY cnt;

PREPARE vals AS
VALUES (1, 7), (2,3), (3,5), (4,2);
SELECT set_eq('q1', 'vals', '3: (N,M) M vertices referenced by N edges') ;


SELECT finish();
ROLLBACK;
