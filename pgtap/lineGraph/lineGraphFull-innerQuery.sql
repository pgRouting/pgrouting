SELECT plan(2);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_linegraphfull',
    'text');

SELECT function_returns('pgr_linegraphfull', 'setof record');

SELECT finish();
ROLLBACK;
