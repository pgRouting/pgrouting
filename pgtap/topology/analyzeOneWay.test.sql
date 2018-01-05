

\i setup.sql
SET client_min_messages TO WARNING;

SELECT plan(2);

SELECT pgr_analyzeOneway('edge_table',
    ARRAY['', 'B', 'TF'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'TF'],
    oneway:='dir');

prepare q1 AS
SELECT ein, count(*) FROM edge_table_vertices_pgr GROUP BY ein ORDER BY ein;
prepare q2 AS
SELECT eout, count(*) FROM edge_table_vertices_pgr GROUP BY eout ORDER BY eout;

PREPARE vals1 AS
VALUES (1,9), (2,4), (3,3), (4,1);
PREPARE vals2 AS
VALUES (1,9), (2,4), (3,3), (4,1);

SELECT set_eq('q1', 'vals1', '3: (N,M) M vertices have N incomming edges') ;
SELECT set_eq('q2', 'vals2', '3: (N,M) M vertices have N outgoing edges') ;

SELECT finish();
ROLLBACK;
