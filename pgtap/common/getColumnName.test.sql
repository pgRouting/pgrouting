
\i setup.sql

SELECT plan(32);

SET client_min_messages  to WARNING;
SET search_path TO "$user", public;

CREATE schema s1;
CREATE TABLE streets (id SERIAL PRIMARY KEY,source INTEGER,target INTEGER);
CREATE TABLE "Streets" (GID SERIAL PRIMARY KEY,SOURCE INTEGER,TARGET INTEGER);
CREATE TABLE s1.streets ("ID" SERIAL PRIMARY KEY,sour INTEGER,targ INTEGER);
CREATE TABLE s1."Streets" ("ID" SERIAL PRIMARY KEY,"SOURCE" INTEGER,"Target" INTEGER);

SELECT  results_eq('SELECT _pgr_getColumnName (''streets'',''id''), 1', 'SELECT ''id''::TEXT, 1');
SELECT  results_eq('SELECT _pgr_getColumnName (''streets'',''ID''), 2', 'SELECT ''id''::TEXT,2');
SELECT  results_eq('SELECT _pgr_getColumnName (''Streets'',''gid''), 3', 'SELECT ''gid''::TEXT,3');
SELECT  results_eq('SELECT _pgr_getColumnName (''Streets'',''GID''), 4', 'SELECT ''gid''::TEXT,4');
SELECT  results_eq('SELECT _pgr_getColumnName (''Streets'',''ID''), 5', 'SELECT NULL::TEXT,5');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1.streets'',''ID''), 6', 'SELECT ''ID''::TEXT,6');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1.streets'',''id''), 7', 'SELECT NULL::TEXT,7');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1.Streets'',''id''), 8', 'SELECT NULL::TEXT,8');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1.Streets'',''ID''), 9', 'SELECT ''ID''::TEXT,9');

SET search_path TO  s1,public;
SELECT  results_eq('SELECT _pgr_getColumnName (''public.streets'',''ID''), 10', 'SELECT ''id''::TEXT,10');
SELECT  results_eq('SELECT _pgr_getColumnName (''public.Streets'',''GID''), 11', 'SELECT ''gid''::TEXT,11');

SELECT  results_eq('SELECT _pgr_getColumnName (''streets'',''ID''), 12', 'SELECT ''ID''::TEXT,12');
SELECT  results_eq('SELECT _pgr_getColumnName (''streets'',''id''), 13', 'SELECT NULL::TEXT,13');
SELECT  results_eq('SELECT _pgr_getColumnName (''Streets'',''id''), 14', 'SELECT NULL::TEXT,14');
SELECT  results_eq('SELECT _pgr_getColumnName (''Streets'',''ID''), 15', 'SELECT ''ID''::TEXT,15');

SET search_path TO "$user", public;
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''streets'',''id''), 16', 'SELECT ''id''::TEXT,16');
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''streets'',''ID''), 17', 'SELECT ''id''::TEXT,17');
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''Streets'',''gid''), 18', 'SELECT ''gid''::TEXT,18');
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''Streets'',''GID''), 19', 'SELECT ''gid''::TEXT,19');
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''Streets'',''ID''), 20', 'SELECT NULL::TEXT,20');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''streets'',''ID''), 21', 'SELECT ''ID''::TEXT,21');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''streets'',''id''), 22', 'SELECT NULL::TEXT,22');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''Streets'',''id''), 23', 'SELECT NULL::TEXT,23');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''Streets'',''ID''), 24', 'SELECT ''ID''::TEXT,24');

SET search_path TO  s1,public;
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''streets'',''ID''), 25', 'SELECT ''id''::TEXT,25');
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''Streets'',''GID''), 26', 'SELECT ''gid''::TEXT,26');

SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''streets'',''ID''), 27', 'SELECT ''ID''::TEXT,27');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''streets'',''id''), 28', 'SELECT NULL::TEXT,28');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''Streets'',''id''), 29', 'SELECT NULL::TEXT,29');
SELECT  results_eq('SELECT _pgr_getColumnName (''s1'',''Streets'',''ID''), 30', 'SELECT ''ID''::TEXT,30');

-- becuase this dont test for schema or TABLE names the expected results are NULL
SELECT  results_eq('SELECT _pgr_getColumnName (''publIc'',''Streets'',''gid''), 31', 'SELECT NULL::TEXT,31');
SELECT  results_eq('SELECT _pgr_getColumnName (''public'',''StrEets'',''gid''), 32', 'SELECT NULL::TEXT,32');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

