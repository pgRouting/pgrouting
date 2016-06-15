
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(3);

-- PARAMETERS
SELECT todo('Fix: code or documentation',3);
SELECT has_function('pgr_pointstodmatrix',ARRAY['geometry[]']);
SELECT hasnt_function('pgr_pointstodmatrix',ARRAY['geometry[]', 'integer']);
SELECT function_returns('pgr_pointstodmatrix',ARRAY['geometry[]'],'record');

SELECT finish();
ROLLBACK;
