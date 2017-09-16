SELECT plan(2);

SET client_min_messages TO ERROR;

SELECT has_function('pgr_turnpenaltygraph',
    'text');

SELECT function_returns('pgr_turnpenaltygraph', 'setof record');

SELECT finish();
ROLLBACK;
