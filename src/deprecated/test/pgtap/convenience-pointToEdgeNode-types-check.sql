
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(3);

-- PARAMETERS

SELECT has_function('pgr_pointtoedgenode',ARRAY['text', 'geometry', 'double precision']);
SELECT hasnt_function('pgr_pointtoedgenode',ARRAY['text', 'geometry']);
SELECT function_returns('pgr_pointtoedgenode', 'integer','Returns integer');

SELECT finish();
ROLLBACK;
