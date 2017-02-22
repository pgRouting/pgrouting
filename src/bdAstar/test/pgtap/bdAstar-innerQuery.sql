\i setup.sql

-- SELECT plan(342);
SELECT plan(87);


SELECT has_function('pgr_bdastar',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT function_returns('pgr_bdastar',
    ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision'],
    'setof record');

-- ONE TO ONE
SELECT style_astar('pgr_bdastar', ', 2, 3, true)');

/*
SELECT todo_start('for version 3.0');
-- ONE TO MANY
SELECT style_astar('pgr_bdastar', ', 2, ARRAY[3], true)');
-- MANY TO ONE
SELECT style_astar('pgr_bdastar', ', ARRAY[2], 3, true)');
-- MANY TO MANY
SELECT style_astar('pgr_bdastar', ', ARRAY[2], ARRAY[3], true)');
SELECT todo_end();
*/

SELECT finish();
ROLLBACK;
