\i setup.sql

SELECT plan(92);
SET client_min_messages TO ERROR;

/* A call looks like this
TODO select a smaller test, because each passing test takes about 19 seconds
SELECT * INTO pickDeliverResults FROM _pgr_pickdeliverEuclidean(
    $$SELECT * FROM orders ORDER BY id$$,
    $$SELECT * FROM vehicles ORDER BY id$$,
    30);
*/

SELECT has_function('_pgr_pickdelivereuclidean',
    ARRAY['text', 'text', 'integer']);

SELECT function_returns('_pgr_pickdelivereuclidean',
    ARRAY['text', 'text', 'integer'],
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
    end_sql = ' FROM orders WHERE id = 11$$,  $$SELECT * FROM vehicles ORDER BY id$$, 30)';
    
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
    end_sql = ' FROM orders WHERE id = 11$$,  $$SELECT * FROM vehicles ORDER BY id$$, 30)';
    
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
    start_sql = 'SELECT * FROM ' || fn || '($$ SELECT * FROM orders WHERE id = 11$$, $$SELECT ';

    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM  vehicles $$, 30)';
    
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
    start_sql = 'SELECT * FROM ' || fn || '($$ SELECT * FROM orders WHERE id = 11$$, $$ SELECT ';
    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM vehicles $$, 30)';
    
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

SELECT test_anyInteger_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'id');

SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'demand');

SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_x');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_y');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_open');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_close');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_service');

SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_x');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_y');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_open');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_close');
SELECT test_anynumerical_orders('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_service');

/* Currently this are not used TODO add when they are used
    'end_x', 'end_y', 'end_open', 'end_close', 'end_service'],
    'speed' is optional defaults to 1
    'start_service' is optional defaults to 0
*/
SELECT test_anyInteger_vehicles('_pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity', 
    'start_x', 'start_y', 'start_open', 'start_close'],
    'id');
SELECT test_anyNumerical_vehicles('_pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity', 
    'start_x', 'start_y', 'start_open', 'start_close'],
    'capacity');
SELECT test_anyNumerical_vehicles('_pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity', 
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_x');
SELECT test_anyNumerical_vehicles('_pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity', 
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_y');
SELECT test_anyNumerical_vehicles('_pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity', 
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_open');
SELECT test_anyNumerical_vehicles('_pgr_pickdelivereuclidean',
    ARRAY['id', 'capacity', 
    'start_x', 'start_y', 'start_open', 'start_close'],
    'start_close');

SELECT finish();
ROLLBACK;
