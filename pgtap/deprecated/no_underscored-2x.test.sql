
\i setup.sql


SELECT plan(18);

set client_min_messages to WARNING;
SELECT  results_eq('SELECT  1, sname, tname  FROM pgr_getTableName(''EDGe_table'')',
    'SELECT 1, ''public''::TEXT, ''edge_table''::TEXT ');
SELECT  results_eq('SELECT  2, sname, tname FROM pgr_getTableName(''EDes2'')',
    'SELECT 2, ''public''::TEXT, NULL::TEXT ');


select todo(1);
SELECT  results_eq('SELECT  3,  pgr_getColumnName(''EDGe_table'', ''SOuRce'')' ,
    'SELECT 3, ''source''::TEXT ');
SELECT  results_eq('SELECT  4,  pgr_getColumnName(''EDes2'', ''SOuRce'') ',
    'SELECT 4,  NULL::TEXT ');
SELECT  results_eq('SELECT  5,  pgr_getColumnName(''EDes2'', ''SOuce'') ',
    'SELECT 5,  NULL::TEXT ');
SELECT  results_eq('SELECT  6,  pgr_getColumnName(''EDes2'', ''SOuce'') ',
    'SELECT 6,  NULL::TEXT ');


select todo(1);
SELECT  results_eq('SELECT  7, pgr_isColumnInTable(''EDGe_table'', ''SOuRce'') ',
    'SELECT 7, true ');
SELECT  results_eq('SELECT  8, pgr_isColumnInTable(''EDes2'', ''SOuRce'')',
    'SELECT 8,  false ');
SELECT  results_eq('SELECT  9, pgr_isColumnInTable(''EDes2'', ''SOuce'')',
    'SELECT 9,  false ');
SELECT  results_eq('SELECT  10, pgr_isColumnInTable(''EDes2'', ''SOuce'')',
    'SELECT 10,  false ');


SELECT  results_eq('SELECT  11, pgr_isColumnIndexed(''EDGe_table'', ''id'') ',
    'SELECT 11, true ');
SELECT  results_eq('SELECT  12, pgr_isColumnIndexed(''EDGe_table'', ''X1'') ',
    'SELECT 12,  false ');


SELECT  results_eq('SELECT  13, pgr_versionless(''2.1.0foobar23'', ''2.1'') ',
    'SELECT 13,  true ');
SELECT  results_eq('SELECT  14, pgr_versionless(''2.1.0foobar23'', ''2.1-rc1'') ',
    'SELECT 14,  true ');
SELECT  results_eq('SELECT  15, pgr_versionless(''2.1.0foobar23'', ''2.1-beta'') ',
    'SELECT 15,  true ');


select todo(1);
SELECT  results_eq('SELECT  16, pgr_quote_ident(''idname.text'') ',
    'SELECT 16,  ''idname.text''::TEXT ');

SELECT  results_eq('SELECT  17, pgr_startPoint(the_geom)::TEXT  FROM edge_table where id = 1 ',
    'SELECT 17, ''010100000000000000000000400000000000000000''::TEXT ');

SELECT  results_eq('SELECT  18, pgr_endPoint(the_geom)::TEXT  FROM edge_table where id = 1 ',
    'SELECT 18, ''01010000000000000000000040000000000000F03F''::TEXT ');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

