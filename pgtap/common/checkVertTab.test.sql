
\i setup.sql

SELECT plan(12);


SET client_min_messages  to WARNING;

SELECT has_column( 'edge_table_vertices_pgr', 'cnt', '1' );
SELECT has_column( 'edge_table_vertices_pgr', 'chk', '2' );
SELECT results_eq( 'SELECT  3, * FROM  _pgr_checkVertTab(''edge_table_vertices_pgr'', ''{"id","cnt","chk"}''::text[]) ',
    'SELECT 3, ''public''::TEXT,''edge_table_vertices_pgr''::TEXT');
SELECT has_column( 'edge_table_vertices_pgr', 'cnt', '4' );
SELECT has_column( 'edge_table_vertices_pgr', 'chk', '5' );

SELECT hasnt_column( 'edge_table', 'cnt', '6' );
SELECT hasnt_column( 'edge_table', 'chk', '7' );
SELECT has_column( 'edge_table', 'id', '8' );
SELECT results_eq( 'SELECT  9, * FROM  _pgr_checkVertTab(''edge_table'', ''{"id","cnt","chk"}''::text[]) ',
    'SELECT 9, ''public''::TEXT,''edge_table''::TEXT');
SELECT has_column( 'edge_table', 'cnt', '10' );
SELECT has_column( 'edge_table', 'chk', '11' );

SELECT throws_ok('SELECT  * FROM  _pgr_checkVertTab(''no_table'', ''{"id","cnt","chk"}''::text[])',
    'P0001', 'raise_exception', '12');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

