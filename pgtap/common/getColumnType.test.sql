
\i setup.sql

SELECT plan(21);


set client_min_messages  to warning;

CREATE SCHEMA s1;
CREATE TABLE streets (id SERIAL PRIMARY KEY, source DOUBLE PRECISION, target REAL);
CREATE TABLE "Streets" (GID bigSERIAL PRIMARY KEY, SOURCE TEXT, TARGET INTEGER);
CREATE TABLE s1.streets ("ID" SERIAL PRIMARY KEY, sour DOUBLE PRECISION, targ REAL);
CREATE TABLE s1."Streets" ("ID" SERIAL PRIMARY KEY, "SOURCE" INTEGER, "Target" INTEGER);

set client_min_messages  to notice;
SELECT  results_eq('SELECT _pgr_getColumnType (''streets'', ''id''),  1', 'SELECT ''integer''::TEXT,  1');
SELECT  results_eq('SELECT _pgr_getColumnType (''streets'', ''source''),  2', 'SELECT ''double precision''::TEXT,  2');
SELECT  results_eq('SELECT _pgr_getColumnType (''streets'', ''target''),  3', 'SELECT ''real''::TEXT,  3');
SELECT  results_eq('SELECT _pgr_getColumnType (''Streets'', ''GID''),  4', 'SELECT ''bigint''::TEXT,  4');
SELECT  results_eq('SELECT _pgr_getColumnType (''Streets'', ''SOURCE''),  5', 'SELECT ''text''::TEXT,  5');
SELECT  results_eq('SELECT _pgr_getColumnType (''Streets'', ''TARGET''),  6', 'SELECT ''integer''::TEXT,  6');

SELECT  results_eq('SELECT _pgr_getColumnType (''s1.streets'', ''ID''),  7', 'SELECT ''integer''::TEXT, 7');
SELECT  results_eq('SELECT _pgr_getColumnType (''s1.streets'', ''sour''),  8',  'SELECT ''double precision''::TEXT,  8');
SELECT  results_eq('SELECT _pgr_getColumnType (''s1.streets'', ''targ''),  9',  'SELECT ''real''::TEXT,  9');
SELECT  results_eq('SELECT _pgr_getColumnType (''s1.Streets'', ''ID''),  10', 'SELECT ''integer''::TEXT,  10');
SELECT  results_eq('SELECT _pgr_getColumnType (''s1.Streets'', ''SOURCE''),  11', 'SELECT ''integer''::TEXT,  11');
SELECT  results_eq('SELECT _pgr_getColumnType (''s1.Streets'', ''Target''),  12', 'SELECT ''integer''::TEXT,  12');

SET search_path TO  s1, public;
SELECT  results_eq('SELECT _pgr_getColumnType (''public.streets'', ''ID''),  13', 'SELECT ''integer''::TEXT, 13');
SELECT  results_eq('SELECT _pgr_getColumnType (''public.Streets'', ''GID''),  14', 'SELECT ''bigint''::TEXT, 14');

SELECT  results_eq('SELECT _pgr_getColumnType (''streets'', ''ID''),  15',  'SELECT ''integer''::TEXT, 15');
SELECT  results_eq('SELECT _pgr_getColumnType (''Streets'', ''ID''),  16',  'SELECT ''integer''::TEXT, 16');

SET search_path TO "$user",  public;
--NULL when TABLE or column doesnt exist
SELECT  results_eq('SELECT _pgr_getColumnType (''Streets'', ''data'', 0),  17',  'SELECT NULL::TEXT,  17');
SELECT  results_eq('SELECT _pgr_getColumnType (''strats'', ''id'', 0),  18',  'SELECT NULL::TEXT,  18');

-- becuase this dont test for schema or TABLE names the expected results are NULL
SELECT  results_eq('SELECT _pgr_getColumnType (''publIc'', ''Streets'', ''gid''),  19', 'SELECT NULL::TEXT,  19');
SELECT  results_eq('SELECT _pgr_getColumnType (''public'', ''StrEets'', ''gid''),  20',  'SELECT  NULL::TEXT,  20');
SELECT  results_eq('SELECT _pgr_getColumnType (''public'', ''Streets'', ''data''),  21',  'SELECT  NULL::TEXT,  21');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;


