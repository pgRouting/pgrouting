
\i setup.sql
-- TESTING DOCUMNETATION INFORMATION

SELECT plan(4);

-- PARAMETERS

SELECT has_function('pgr_vidstodmatrix',ARRAY['integer[]', 'geometry[]', 'text','double precision']);
SELECT function_returns('pgr_vidstodmatrix',ARRAY['integer[]', 'geometry[]', 'text','double precision'],'record');

--pgr_vidsToDMatrix(IN sql text, IN vids integer[], IN directed boolean, IN has_reverse_cost boolean, IN want_symmetric boolean, OUT dmatrix double precision[])
SELECT has_function('pgr_vidstodmatrix',ARRAY['text','integer[]', 'boolean', 'boolean', 'boolean']);
SELECT function_returns('pgr_vidstodmatrix',ARRAY['text','integer[]', 'boolean', 'boolean', 'boolean'],'double precision[]');

SELECT finish();
ROLLBACK;
