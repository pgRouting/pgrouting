\i setup.sql

SELECT plan(2);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_turnrestrictedpath',
        ARRAY[ 'text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'boolean', 'boolean']
    );

SELECT function_returns('pgr_turnrestrictedpath',
    ARRAY[ 'text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'boolean', 'boolean'],
    'setof record');

/*
SELECT style_dijkstraTR('pgr_turnrestrictedpath', ', $$SELECT * FROM new_restrictions$$, 2, 3, 3)');
SELECT style_dijkstraTR('pgr_turnrestrictedpath', ', $$SELECT * FROM new_restrictions$$, 2, 3, 3, true)');
SELECT style_dijkstraTR('pgr_turnrestrictedpath', ', $$SELECT * FROM new_restrictions$$, 2, 3, 3, false)');
*/

SELECT finish();
ROLLBACK;
