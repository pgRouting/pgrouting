
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(2);

-- PARAMETERS

SELECT has_function('pgr_pointstovids',ARRAY['geometry[]', 'text', 'double precision']);
SELECT function_returns('pgr_pointstovids',ARRAY['geometry[]', 'text', 'double precision'],'integer[]');

SELECT finish();
ROLLBACK;
