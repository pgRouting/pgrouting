
\i setup.sql

SELECT plan(35);

set client_min_messages  to warning;

SET search_path TO "$user", public;

create schema s1;
create schema s2;
create schema "S2";
create schema s3;
create table streets (id serial PRIMARY KEY,source integer,target integer);
create table "Streets" (ID serial PRIMARY KEY,SOURCE integer,TARGET integer);
create table s1.streets ("ID" serial PRIMARY KEY,sour integer,targ integer);
create table s1."Streets" ("ID" serial PRIMARY KEY,"SOURCE" integer,"Target" integer);
create table s2."Streets" (id serial ,source integer,target integer);
create table "S2".streets (id serial ,source integer,target integer);
create table s3."Streets" (id serial ,source integer,target integer);
create table s3."sTreets" (id serial ,source integer,target integer);
create table s3."StrEets" (id serial ,source integer,target integer);

set client_min_messages to warning;

SELECT  results_eq('SELECT 1, sname, tname from  _pgr_getTableName(''StreEts'')', 'SELECT 1, ''public''::TEXT, ''streets''::TEXT '); --public.streets
SELECT  results_eq('SELECT 2, sname, tname  from _pgr_getTableName(''Streets'')', 'SELECT 2, ''public''::TEXT, ''Streets''::TEXT');  --public.Streets
SELECT  results_eq('SELECT 3, sname, tname  from _pgr_getTableName(''streEts'')', 'SELECT 3, ''public''::TEXT, ''streets''::TEXT');  --public.streets
SELECT  results_eq('SELECT 4, sname, tname  from _pgr_getTableName(''streEts1'')', 'SELECT 4, ''public''::TEXT, NULL::TEXT  ');     --public.<null>

SELECT  results_eq('SELECT 5, sname, tname  from _pgr_getTableName(''s1.StreEts'')', 'SELECT 5, ''s1''::TEXT, ''streets''::TEXT');         --s1.streets
SELECT  results_eq('SELECT 6, sname, tname  from _pgr_getTableName(''s1.Streets'')', 'SELECT 6, ''s1''::TEXT, ''Streets''::TEXT');         --s1.Streets
SELECT  results_eq('SELECT 7, sname, tname  from _pgr_getTableName(''S1.streEts'')', 'SELECT 7, ''s1''::TEXT, ''streets''::TEXT');         --s1.streets
SELECT  results_eq('SELECT 8, sname, tname from _pgr_getTableName(''S1.streEts1'')', 'SELECT 8, ''s1''::TEXT, NULL::TEXT::TEXT');         --s1.<null>

SELECT  results_eq('SELECT 9, sname, tname from _pgr_getTableName(''s2.StreEts'')', 'SELECT 9, ''s2''::TEXT, NULL::TEXT::TEXT');         --s2.<null>
SELECT  results_eq('SELECT 10, sname, tname  from _pgr_getTableName(''s2.Streets'')', 'SELECT 10, ''s2''::TEXT, ''Streets''::TEXT');         --s2.Streets
SELECT  results_eq('SELECT 11, sname, tname from _pgr_getTableName(''S2.streEts'')', 'SELECT 11, ''S2''::TEXT, ''streets''::TEXT');         --S2.streets


SELECT  results_eq('SELECT 12, sname, tname  from _pgr_getTableName(''s3.Streets'')', 'SELECT 12, ''s3''::TEXT, ''Streets''::TEXT');         --s3.Streets
SELECT  results_eq('SELECT 13, sname, tname  from _pgr_getTableName(''s3.sTreets'')', 'SELECT 13, ''s3''::TEXT, ''sTreets''::TEXT');         --s3.sTreets
SELECT  results_eq('SELECT 14, sname, tname  from _pgr_getTableName(''s3.StrEets'')', 'SELECT 14, ''s3''::TEXT, ''StrEets''::TEXT');         --s3.StrEets

SELECT  results_eq('SELECT 15, sname, tname from _pgr_getTableName(''s3.streets'')', 'SELECT 15, ''s3''::TEXT, NULL::TEXT');         --s3.<null>
SELECT  results_eq('SELECT 16, sname, tname from _pgr_getTableName(''s3.streetS'')', 'SELECT 16, ''s3''::TEXT, NULL::TEXT');         --s3.<null>
SELECT  results_eq('SELECT 17, sname, tname from _pgr_getTableName(''S3.streEts1'')', 'SELECT 17, ''s3''::TEXT, NULL::TEXT');         --s3.<null>

SET search_path TO s1,public;

SELECT  results_eq('SELECT 18, sname, tname  from _pgr_getTableName(''StreEts'')', 'SELECT 18, ''s1''::TEXT, ''streets''::TEXT');         --s1.streets
SELECT  results_eq('SELECT 19, sname, tname  from _pgr_getTableName(''Streets'')', 'SELECT 19, ''s1''::TEXT, ''Streets''::TEXT');         --s1.Streets
SELECT  results_eq('SELECT 20, sname, tname  from _pgr_getTableName(''streEts'')', 'SELECT 20, ''s1''::TEXT, ''streets''::TEXT');         --s1.streets
SELECT  results_eq('SELECT 21, sname, tname from _pgr_getTableName(''streEts1'')', 'SELECT 21, ''s1''::TEXT, NULL::TEXT');         --s1.<null>

SELECT  results_eq('SELECT 22, sname, tname  from _pgr_getTableName(''s1.StreEts'')', 'SELECT 22, ''s1''::TEXT, ''streets''::TEXT');         --s1.streets
SELECT  results_eq('SELECT 23, sname, tname  from _pgr_getTableName(''s1.Streets'')', 'SELECT 23, ''s1''::TEXT, ''Streets''::TEXT');         --s1.Streets
SELECT  results_eq('SELECT 24, sname, tname  from _pgr_getTableName(''S1.streEts'')', 'SELECT 24, ''s1''::TEXT, ''streets''::TEXT');         --s1.streets
SELECT  results_eq('SELECT 25, sname, tname from _pgr_getTableName(''S1.streEts1'')', 'SELECT 25, ''s1''::TEXT, NULL::TEXT');         --s1.<null>

SELECT  results_eq('SELECT 26, sname, tname from _pgr_getTableName(''s2.StreEts'')', 'SELECT 26, ''s2''::TEXT, NULL::TEXT');         --s2.<null>
SELECT  results_eq('SELECT 27, sname, tname  from _pgr_getTableName(''s2.Streets'')', 'SELECT 27, ''s2''::TEXT, ''Streets''::TEXT');         --s2.Streets
SELECT  results_eq('SELECT 28, sname,  tname  from _pgr_getTableName(''S2.streEts'')', 'SELECT 28, ''S2''::TEXT, ''streets''::TEXT');         --S2.streets


SELECT  results_eq('SELECT 29, sname, tname  from _pgr_getTableName(''s3.Streets'')', 'SELECT 29, ''s3''::TEXT, ''Streets''::TEXT');         --s3.Streets
SELECT  results_eq('SELECT 30, sname, tname  from _pgr_getTableName(''s3.sTreets'')', 'SELECT 30, ''s3''::TEXT, ''sTreets''::TEXT');         --s3.sTreets
SELECT  results_eq('SELECT 31, sname, tname  from _pgr_getTableName(''s3.StrEets'')', 'SELECT 31, ''s3''::TEXT, ''StrEets''::TEXT');         --s3.StrEets
SELECT  results_eq('SELECT 32, sname, tname from _pgr_getTableName(''s3.streets'')', 'SELECT 32, ''s3''::TEXT, NULL::TEXT');         --s3.<null>
SELECT  results_eq('SELECT 33, sname, tname from _pgr_getTableName(''s3.streetS'')', 'SELECT 33, ''s3''::TEXT, NULL::TEXT');         --s3.<null>
SELECT  results_eq('SELECT 34, sname, tname from _pgr_getTableName(''S3.streEts1'')', 'SELECT 34, ''s3''::TEXT, NULL::TEXT');         --s3.<null>

SELECT  results_eq('SELECT 35, sname, tname from _pgr_getTableName(''S4.streEts1'')', 'SELECT 35, NULL::TEXT, NULL::TEXT');         --<null>.<null>


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

