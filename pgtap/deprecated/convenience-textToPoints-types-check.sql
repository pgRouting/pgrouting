
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(2);

-- PARAMETERS

SELECT has_function('pgr_texttopoints',ARRAY['text', 'integer']);

SELECT todo('Documentation''s signature says it returns an integer',1);
SELECT function_returns('pgr_texttopoints', ARRAY['text', 'integer'], 'integer');

SELECT finish();
ROLLBACK;
