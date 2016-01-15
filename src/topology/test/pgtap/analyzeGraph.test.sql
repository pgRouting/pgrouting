


\i setup.sql
SET client_min_messages TO WARNING;

SELECT plan(4);


SELECT pgr_analyzegraph('edge_table', 0.000001);


SELECT is((SELECT count(*)::INTEGER cnt FROM edge_table_vertices_pgr WHERE chk = 1), 1, '1: One vertex has a problem');
SELECT is((SELECT id::INTEGER FROM edge_table_vertices_pgr WHERE chk = 1), 15, '2: vertex #15 has the problem');

PREPARE q1 AS
SELECT cnt, count(*) AS M  FROM edge_table_vertices_pgr GROUP BY cnt ORDER BY cnt;

PREPARE vals AS
VALUES (1, 7), (2,3), (3,5), (4,2);
SELECT set_eq('q1', 'vals', '3: (N,M) M vertices referenced by N edges') ;

SELECT * FROM edge_table_vertices_pgr order by id;

SELECT * FROM TODO_START();

PREPARE q3 AS
SELECT pgr_analyzegraph('edge_table', 0.000001);

PREPARE stats AS
VALUES (2,7,1,1,0);

SELECT set_eq('q3','stats', '4: pgr_createVerticesTable: Instead of returning OK, return the expected statistics');

SELECT * FROM TODO_END();

SELECT finish();
ROLLBACK;
