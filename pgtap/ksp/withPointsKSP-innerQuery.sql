\i setup.sql

SELECT plan(56);


SELECT has_function('pgr_withpointsksp',
    ARRAY['text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'character', 'boolean']);

SELECT function_returns('pgr_withpointsksp',
    ARRAY['text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'character', 'boolean'],
    'setof record');



-- ONE TO ONE
SELECT style_withpoints('pgr_withpointsksp', ', 2, 3, 2, true)');



SELECT finish();
ROLLBACK;
