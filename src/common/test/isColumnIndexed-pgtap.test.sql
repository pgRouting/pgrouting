\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
SELECT plan(60);

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



SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''id'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''id'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''ID'')', 'SELECT true');

SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''id'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''ID'')', 'SELECT true');

SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''Source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''Targ'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''SOURCE'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''Targ'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.STREETS'',''id'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.STREETS'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.stReets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''SOURCE'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''Targ'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''id'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''SOURCE'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''Targ'')', 'SELECT false');

SET search_path TO s1,public;

SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''ID'')', 'SELECT true');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''ID'')', 'SELECT true');

SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''id'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''Source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''streets'',''Targ'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''id'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''SOURCE'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''Streets'',''Targ'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.STREETS'',''id'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.STREETS'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.stReets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''SOURCE'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.streets'',''Targ'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''id'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''source'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''SOURCE'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''Target'')', 'SELECT false');
SELECT  results_eq('SELECT _pgr_isColumnIndexed(''s1.Streets'',''Targ'')', 'SELECT false');




-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
