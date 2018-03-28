\i setup.sql

SELECT plan(122);

SELECT todo_start('not ready yet');

/* A call looks like this
SELECT * INTO pickDeliverResults FROM _pgr_pickdeliver(
    $$SELECT * FROM orders ORDER BY id$$,
    $$SELECT * FROM vehicles ORDER BY id$$,
    $$SELECT * FROM dist_matrix$$,
    30);
*/

SELECT has_function('_pgr_pickdeliver',
    ARRAY['text', 'text', 'text', 'integer']);

SELECT function_returns('_pgr_pickdeliver',
    ARRAY['text', 'text', 'text', 'integer'],
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
    end_sql = ' FROM orders WHERE id = 11$$,
        $$SELECT * FROM vehicles ORDER BY id$$,
        $$SELECT * FROM dist_matrix$$,
        30)';

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
    end_sql = ' FROM orders WHERE id = 11$$,
        $$SELECT * FROM vehicles ORDER BY id$$,
        $$SELECT * FROM dist_matrix$$,
        30)';

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
    end_sql = ' FROM vehicles $$,
        $$SELECT * FROM dist_matrix$$,
        30)';

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
    end_sql = ' FROM vehicles $$,
        $$SELECT * FROM dist_matrix$$,
        30)';

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
testing the pick/deliver matrix
*/
CREATE OR REPLACE FUNCTION test_anyInteger_matrix(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'SELECT * FROM ' || fn || '(
        $$ SELECT * FROM orders WHERE id = 11$$,
        $$ SELECT * FROM vehicles $$,
        $$ SELECT ';

    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM dist_matrix $$,
        30)';

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
CREATE OR REPLACE FUNCTION test_anyNumerical_matrix(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'SELECT * FROM ' || fn || '(
        $$ SELECT * FROM orders WHERE id = 11$$,
        $$ SELECT * FROM vehicles $$,
        $$ SELECT ';

    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM dist_matrix $$,
        30)';

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


SELECT test_anyInteger_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'id');
SELECT test_anynumerical_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'demand');

SELECT test_anyInteger_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_node_id');
SELECT test_anynumerical_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_open');
SELECT test_anynumerical_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_close');
SELECT test_anynumerical_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'pick_service');

SELECT test_anyInteger_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_node_id');
SELECT test_anynumerical_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_open');
SELECT test_anynumerical_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_close');
SELECT test_anynumerical_orders('_pgr_pickdeliver',
    ARRAY['id', 'demand',
    'pick_node_id', 'pick_open', 'pick_close', 'pick_service',
    'deliver_node_id', 'deliver_open', 'deliver_close', 'deliver_service'],
    'deliver_service');

/* Currently this are not used TODO add when they are used
    'end_x', 'end_y', 'end_open', 'end_close', 'end_service'],
    'speed' is optional defaults to 1
    'start_service' is optional defaults to 0
*/
/*
without optional: number
*/
SELECT test_anyInteger_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity',
    'start_node_id', 'start_open', 'start_close'],
    'id');
SELECT test_anyNumerical_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity',
    'start_node_id', 'start_open', 'start_close'],
    'capacity');
SELECT test_anyInteger_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity',
    'start_node_id', 'start_open', 'start_close'],
    'start_node_id');
SELECT test_anyNumerical_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity',
    'start_node_id', 'start_open', 'start_close'],
    'start_open');
SELECT test_anyNumerical_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity',
    'start_node_id', 'start_open', 'start_close'],
    'start_close');


/*
with optional: number
*/
SELECT test_anyInteger_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity', 'number',
    'start_node_id', 'start_open', 'start_close'],
    'id');
SELECT test_anyNumerical_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity', 'number',
    'start_node_id', 'start_open', 'start_close'],
    'capacity');
SELECT test_anyInteger_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity', 'number',
    'start_node_id', 'start_open', 'start_close'],
    'number');
SELECT test_anyInteger_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity', 'number',
    'start_node_id', 'start_open', 'start_close'],
    'start_node_id');
SELECT test_anyNumerical_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity', 'number',
    'start_node_id', 'start_open', 'start_close'],
    'start_open');
SELECT test_anyNumerical_vehicles('_pgr_pickdeliver',
    ARRAY['id', 'capacity', 'number',
    'start_node_id', 'start_open', 'start_close'],
    'start_close');


/*
Testing the matrix
*/
SELECT test_anyInteger_matrix('_pgr_pickdeliver',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'start_vid');
SELECT test_anyInteger_matrix('_pgr_pickdeliver',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'end_vid');
SELECT test_anyNumerical_matrix('_pgr_pickdeliver',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'agg_cost');

SELECT todo_end();

SELECT finish();
ROLLBACK;
