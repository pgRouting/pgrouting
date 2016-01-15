
\i setup.sql

SELECT plan(56);

CREATE SCHEMA s1;
CREATE SCHEMA s2;
CREATE SCHEMA "S2";
CREATE SCHEMA s3;
CREATE TABLE streets (id SERIAL PRIMARY KEY,source INTEGER,target INTEGER);
CREATE TABLE "Streets" (ID SERIAL PRIMARY KEY,SOURCE INTEGER,TARGET INTEGER);
CREATE TABLE s1.streets ("ID" SERIAL PRIMARY KEY,sour INTEGER,targ INTEGER);
CREATE TABLE s1."Streets" ("ID" SERIAL PRIMARY KEY,"SOURCE" INTEGER,"Target" INTEGER);
CREATE TABLE s2."Streets" (id SERIAL ,source INTEGER,target INTEGER);
CREATE TABLE "S2".streets (id SERIAL ,source INTEGER,target INTEGER);
CREATE TABLE s3."Streets" (id SERIAL ,source INTEGER,target INTEGER);
CREATE TABLE s3."sTreets" (id SERIAL ,source INTEGER,target INTEGER);
CREATE TABLE s3."StrEets" (id SERIAL ,source INTEGER,target INTEGER);


SELECT  results_eq('SELECT 1,   _pgr_isColumnInTable(''streets'', ''id'')',       'SELECT 1, true');
SELECT  results_eq('SELECT 2,   _pgr_isColumnInTable(''streets'', ''source'')',   'SELECT 2, true');
SELECT  results_eq('SELECT 3,   _pgr_isColumnInTable(''streets'', ''target'')',   'SELECT 3, true');
SELECT  results_eq('SELECT 4,   _pgr_isColumnInTable(''streets'', ''ID'')',       'SELECT 4, true');
SELECT  results_eq('SELECT 5,   _pgr_isColumnInTable(''streets'', ''Source'')',   'SELECT 5, true');
SELECT  results_eq('SELECT 6,   _pgr_isColumnInTable(''streets'', ''Target'')',   'SELECT 6, true');
SELECT  results_eq('SELECT 7,   _pgr_isColumnInTable(''Streets'', ''id'')',       'SELECT 7, true');
SELECT  results_eq('SELECT 8,   _pgr_isColumnInTable(''Streets'', ''source'')',   'SELECT 8, true');
SELECT  results_eq('SELECT 9,   _pgr_isColumnInTable(''Streets'', ''target'')',   'SELECT 9, true');
SELECT  results_eq('SELECT 10,   _pgr_isColumnInTable(''Streets'', ''ID'')',      'SELECT 10, true');
SELECT  results_eq('SELECT 11,   _pgr_isColumnInTable(''Streets'', ''SOURCE'')',  'SELECT 11, true');
SELECT  results_eq('SELECT 12,   _pgr_isColumnInTable(''Streets'', ''Target'')',  'SELECT 12, true');
SELECT  results_eq('SELECT 13,   _pgr_isColumnInTable(''s1.streets'', ''ID'')',   'SELECT 13, true');
SELECT  results_eq('SELECT 14,   _pgr_isColumnInTable(''s1.streets'', ''Targ'')', 'SELECT 14, true');
SELECT  results_eq('SELECT 15,   _pgr_isColumnInTable(''s1.Streets'', ''ID'')',   'SELECT 15, true');
SELECT  results_eq('SELECT 16,   _pgr_isColumnInTable(''s1.Streets'', ''SOURCE'')',   'SELECT 16, true');
SELECT  results_eq('SELECT 17,   _pgr_isColumnInTable(''s1.Streets'', ''Target'')',   'SELECT 17, true');


SELECT  results_eq('SELECT 18,   _pgr_isColumnInTable(''streets'', ''Targ'')',      'SELECT 18, false');
SELECT  results_eq('SELECT 19,   _pgr_isColumnInTable(''Streets'', ''Targ'')',      'SELECT 19, false');
SELECT  results_eq('SELECT 20,   _pgr_isColumnInTable(''s1.STREETS'', ''id'')',     'SELECT 20, false');
SELECT  results_eq('SELECT 21,   _pgr_isColumnInTable(''s1.STREETS'', ''source'')', 'SELECT 21, false');
SELECT  results_eq('SELECT 22,   _pgr_isColumnInTable(''s1.stReets'', ''target'')', 'SELECT 22, false');
SELECT  results_eq('SELECT 23,   _pgr_isColumnInTable(''s1.streets'', ''SOURCE'')', 'SELECT 23, false');
SELECT  results_eq('SELECT 24,   _pgr_isColumnInTable(''s1.streets'', ''Target'')', 'SELECT 24, false');
SELECT  results_eq('SELECT 25,   _pgr_isColumnInTable(''s1.Streets'', ''id'')',     'SELECT 25, false');
SELECT  results_eq('SELECT 26,   _pgr_isColumnInTable(''s1.Streets'', ''source'')', 'SELECT 26, false');
SELECT  results_eq('SELECT 27,   _pgr_isColumnInTable(''s1.Streets'', ''target'')', 'SELECT 27, false');
SELECT  results_eq('SELECT 28,   _pgr_isColumnInTable(''s1.Streets'', ''Targ'')',   'SELECT 28, false');

SET search_path TO s1,public;

SELECT  results_eq('SELECT 29,   _pgr_isColumnInTable(''streets'', ''ID'')',        'SELECT 29, true');
SELECT  results_eq('SELECT 30,   _pgr_isColumnInTable(''streets'', ''Targ'')',      'SELECT 30, true');
SELECT  results_eq('SELECT 31,   _pgr_isColumnInTable(''Streets'', ''ID'')',        'SELECT 31, true');
SELECT  results_eq('SELECT 32,   _pgr_isColumnInTable(''Streets'', ''SOURCE'')',    'SELECT 32, true');
SELECT  results_eq('SELECT 33,   _pgr_isColumnInTable(''Streets'', ''Target'')',    'SELECT 33, true');
SELECT  results_eq('SELECT 34,   _pgr_isColumnInTable(''s1.streets'', ''ID'')',     'SELECT 34, true');
SELECT  results_eq('SELECT 35,   _pgr_isColumnInTable(''s1.streets'', ''Targ'')',   'SELECT 35, true');
SELECT  results_eq('SELECT 36,   _pgr_isColumnInTable(''s1.Streets'', ''ID'')',     'SELECT 36, true');
SELECT  results_eq('SELECT 37,   _pgr_isColumnInTable(''s1.Streets'', ''SOURCE'')', 'SELECT 37, true');
SELECT  results_eq('SELECT 38,   _pgr_isColumnInTable(''s1.Streets'', ''Target'')', 'SELECT 38, true');

SELECT  results_eq('SELECT 39,   _pgr_isColumnInTable(''streets'', ''id'')',        'SELECT 39, false');
SELECT  results_eq('SELECT 40,   _pgr_isColumnInTable(''streets'', ''source'')',    'SELECT 40, false');
SELECT  results_eq('SELECT 41,   _pgr_isColumnInTable(''streets'', ''target'')',    'SELECT 41, false');
SELECT  results_eq('SELECT 42,   _pgr_isColumnInTable(''streets'', ''Source'')',    'SELECT 42, false');
SELECT  results_eq('SELECT 43,   _pgr_isColumnInTable(''streets'', ''Target'')',    'SELECT 43, false');
SELECT  results_eq('SELECT 44,   _pgr_isColumnInTable(''Streets'', ''id'')',        'SELECT 44, false');
SELECT  results_eq('SELECT 45,   _pgr_isColumnInTable(''Streets'', ''source'')',    'SELECT 45, false');
SELECT  results_eq('SELECT 46,   _pgr_isColumnInTable(''Streets'', ''target'')',    'SELECT 46, false');
SELECT  results_eq('SELECT 47,   _pgr_isColumnInTable(''Streets'', ''Targ'')',      'SELECT 47, false');
SELECT  results_eq('SELECT 48,   _pgr_isColumnInTable(''s1.STREETS'', ''id'')',     'SELECT 48, false');
SELECT  results_eq('SELECT 49,   _pgr_isColumnInTable(''s1.STREETS'', ''source'')', 'SELECT 49, false');
SELECT  results_eq('SELECT 50,   _pgr_isColumnInTable(''s1.stReets'', ''target'')', 'SELECT 50, false');
SELECT  results_eq('SELECT 51,   _pgr_isColumnInTable(''s1.streets'', ''SOURCE'')', 'SELECT 51, false');
SELECT  results_eq('SELECT 52,   _pgr_isColumnInTable(''s1.streets'', ''Target'')', 'SELECT 52, false');
SELECT  results_eq('SELECT 53,   _pgr_isColumnInTable(''s1.Streets'', ''id'')',     'SELECT 53, false');
SELECT  results_eq('SELECT 54,   _pgr_isColumnInTable(''s1.Streets'', ''source'')', 'SELECT 54, false');
SELECT  results_eq('SELECT 55,   _pgr_isColumnInTable(''s1.Streets'', ''target'')', 'SELECT 55, false');
SELECT  results_eq('SELECT 56,   _pgr_isColumnInTable(''s1.Streets'', ''Targ'')',   'SELECT 56, false');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
