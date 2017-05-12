
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(2);

-- RECEIVES 1 PARAMETERS

SELECT has_function('pgr_flipedges',ARRAY['geometry[]']);
SELECT function_returns('pgr_flipedges', 'geometry[]','Returns geometry[]');

SELECT finish();
ROLLBACK;
