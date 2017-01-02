\i setup.sql

SELECT plan(62);
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
CREATE OR REPLACE FUNCTION test_anyInteger(fn TEXT, params TEXT[], parameter TEXT) 
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
CREATE OR REPLACE FUNCTION test_anyNumerical(fn TEXT, params TEXT[], parameter TEXT) 
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

SELECT test_anyInteger('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'id');

SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'demand');

SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_x');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_y');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_open');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_close');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_service');

SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_x');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_y');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_open');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_close');
SELECT test_anynumerical('_pgr_pickdelivereuclidean',
    ARRAY['id', 'demand',
    'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
    'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_service');


SELECT finish();
ROLLBACK;
