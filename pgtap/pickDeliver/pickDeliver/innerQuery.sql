\i setup.sql
SET client_min_messages TO NOTICE;

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(80);
SET client_min_messages TO ERROR;

/* A call looks like this
SELECT * INTO pickDeliverResults FROM pgr_pickdeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    $sql1$ SELECT * from pgr_dijkstraCostMatrix($$SELECT * FROM edge_table$$,
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a)) $sql1$)';
    $sql1$
    );
*/

SELECT has_function('pgr_pickdeliver',
    ARRAY['text', 'text', 'text', 'double precision', 'integer', 'integer']);

SELECT function_returns('pgr_pickdeliver',
    ARRAY['text', 'text', 'text', 'double precision', 'integer', 'integer'],
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
    end_sql = ' FROM orders $$,  $$SELECT * FROM vehicles $$,  $sql1$ SELECT * from pgr_dijkstraCostMatrix($$SELECT * FROM edge_table$$,
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a)) $sql1$)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'SMALLINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query,'INTEGER on ' ||parameter||' is OK');

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'BIGINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Expected Exception REAL with '||parameter);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Expected Exception FLOAT8 with '||parameter);

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Expected Exception NUMERIC with '||parameter);

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
    end_sql = ' FROM orders $$,  $$SELECT * FROM vehicles $$,  $sql1$ SELECT * from pgr_dijkstraCostMatrix($$SELECT * FROM edge_table$$,
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a)) $sql1$)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'SMALLINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query,'INTEGER on ' ||parameter||' is OK');

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'BIGINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT lives_ok(query,'REAL on ' ||parameter||' is OK');

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query,'FLOAT8 on ' ||parameter||' is OK');

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT lives_ok(query,'NUMERIC ' ||parameter||' is OK');
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
    end_sql = ' FROM vehicles $$, $sql1$ SELECT * from pgr_dijkstraCostMatrix($$SELECT * FROM edge_table$$,
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a)) $sql1$)';


    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'SMALLINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query,'INTEGER on ' ||parameter||' is OK');

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'BIGINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Expected Exception REAL with '||parameter);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Expected Exception FLOAT8 with '||parameter);

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Expected Exception NUMERIC with '||parameter);

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
    end_sql = ' FROM vehicles $$, $sql1$ SELECT * from pgr_dijkstraCostMatrix($$SELECT * FROM edge_table$$,
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a)) $sql1$)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'SMALLINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query,'INTEGER on ' ||parameter||' is OK');

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'BIGINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT lives_ok(query,'REAL on ' ||parameter||' is OK');

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query,'FLOAT8 on ' ||parameter||' is OK');

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT lives_ok(query,'NUMERIC ' ||parameter||' is OK');
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
    start_sql = 'SELECT * FROM ' || fn || '($$ SELECT * FROM orders $$, $$ SELECT * FROM vehicles$$, $sql1$SELECT ';

    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM pgr_dijkstraCostMatrix($$SELECT * FROM edge_table$$,
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a)) $sql1$)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix SMALLINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix INTEGER on ' ||parameter||' is OK');

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix BIGINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Matrix Expected Exception REAL with '||parameter);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Matrix Expected Exception FLOAT8 with '||parameter);

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$||parameter||$$' type. Expected ANY-INTEGER$$,'Matrix Expected Exception NUMERIC with '||parameter);

END;
$BODY$ LANGUAGE plpgsql;

/*
testing the pick/deliver matrix
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
    start_sql = 'SELECT * FROM ' || fn || '($$ SELECT * FROM orders $$, $$ SELECT * FROM vehicles$$, $sql1$SELECT ';
    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM pgr_dijkstraCostMatrix($$SELECT * FROM edge_table$$,
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a)) $sql1$)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix SMALLINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix INTEGER on ' ||parameter||' is OK');

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix BIGINT on ' ||parameter||' is OK');

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix REAL on ' ||parameter||' is OK');

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix FLOAT8 on ' ||parameter||' is OK');

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT lives_ok(query,'matrix NUMERIC ' ||parameter||' is OK');

END;
$BODY$ LANGUAGE plpgsql;

SELECT test_anyInteger_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'id');

SELECT test_anyInteger_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'p_node_id');

SELECT test_anyInteger_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'd_node_id');

SELECT test_anynumerical_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'demand');

SELECT test_anynumerical_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'p_open');

SELECT test_anynumerical_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'p_close');

SELECT test_anynumerical_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'p_service');

SELECT test_anynumerical_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'd_open');

SELECT test_anynumerical_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'd_close');

SELECT test_anynumerical_orders('pgr_pickdeliver',
    ARRAY['id', 'demand',
    'p_node_id', 'p_open', 'p_close', 'p_service',
    'd_node_id', 'd_open', 'd_close', 'd_service'],
    'd_service');

/* Currently this are not used TODO add when they are used
    'end_x', 'end_y', 'end_open', 'end_close', 'end_service'],
    'speed' is optional defaults to 1
    'start_service' is optional defaults to 0
*/

SELECT test_anyInteger_matrix('pgr_pickdeliver',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'start_vid');

SELECT test_anyInteger_matrix('pgr_pickdeliver',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'end_vid');

SELECT test_anyNumerical_matrix('pgr_pickdeliver',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'agg_cost');

SELECT finish();
ROLLBACK;
