\i setup.sql

SELECT plan(104);
SET client_min_messages TO ERROR;

/* A call looks like this
TODO select a smaller test, because each passing test takes about 19 seconds
SELECT * INTO pickDeliverResults FROM pgr_pickdeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    30);
*/

SELECT has_function('pgr_pickdelivereuclidean',
    ARRAY['text', 'text', 'double precision', 'integer', 'integer']);

SELECT function_returns('pgr_pickdelivereuclidean',
    ARRAY['text', 'text', 'double precision', 'integer', 'integer'],
    'setof record');

/* testing the pick/deliver orders*/
CREATE OR REPLACE FUNCTION test_anyInteger_orders(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'select * from ' || fn || '($$ SELECT ';
    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM orders $$,  $$SELECT * FROM vehicles $$, max_cycles := 30)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT throws_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

/* testing the pick/deliver orders*/
CREATE OR REPLACE FUNCTION test_anyNumerical_orders(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'select * from ' || fn || '($$ SELECT ';
    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM orders $$,  $$SELECT * FROM vehicles $$, max_cycles := 30)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT lives_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

/*
testing the pick/deliver vehicles
*/
CREATE OR REPLACE FUNCTION test_anyInteger_vehicles(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'SELECT * FROM ' || fn || '($$ SELECT * FROM orders $$, $$SELECT ';

    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM  vehicles $$, max_cycles := 30)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

/*
testing the pick/deliver vehicles
 */
CREATE OR REPLACE FUNCTION test_anyNumerical_vehicles(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'SELECT * FROM ' || fn || '($$ SELECT * FROM orders $$, $$ SELECT ';
    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM vehicles $$, max_cycles := 30)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

SELECT test_anyInteger_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'id');

SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'demand');

SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'p_x');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'p_y');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'p_open');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'p_close');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'p_service');

SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'd_x');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'd_y');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'd_open');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'd_close');
SELECT test_anynumerical_orders('pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'p_x', 'p_y', 'p_open', 'p_close', 'p_service',
    'd_x', 'd_y', 'd_open', 'd_close', 'd_service'],
    'd_service');

/* Currently this are not used TODO add when they are used
    'end_x', 'end_y', 'end_open', 'end_close', 'end_service'],
    'speed' is optional defaults to 1
    'start_service' is optional defaults to 0
*/
SELECT test_anyInteger_vehicles('pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity',
    'start_x', 'start_y', 'start_open', 'start_close'],
    'id');
SELECT test_anyNumerical_vehicles('pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity',
    'start_x', 'start_y', 'start_open', 'start_close'],
    'capacity');
SELECT test_anyNumerical_vehicles('pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity',
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_x');
SELECT test_anyNumerical_vehicles('pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity',
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_y');
SELECT test_anyNumerical_vehicles('pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity',
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_open');
SELECT test_anyNumerical_vehicles('pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity',
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_close');

SELECT finish();
ROLLBACK;
