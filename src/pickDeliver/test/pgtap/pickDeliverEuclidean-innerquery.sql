\i setup.sql

SELECT plan(61);


SELECT has_function('_pgr_pickdelivereuclidean',
    ARRAY['text', 'text', 'integer']);


-- pick deliver
CREATE OR REPLACE FUNCTION orders_style_pd_euclidean(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
    RETURN QUERY 
    SELECT test_anyIntegerOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'id');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'demand');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'pick_x');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'pick_y');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'pick_open');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'pick_close');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'pick_service');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'deliver_x');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'deliver_y');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'deliver_open');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'deliver_close');
    RETURN QUERY 
    SELECT test_anyNumericalOrders(fn, rest_sql,
        ARRAY['id', 'demand', 'pick_x', 'pick_y', 'pick_open', 'pick_close', 'pick_service',
         'deliver_x', 'deliver_y', 'deliver_open', 'deliver_close', 'deliver_service'],
        'deliver_service');
END;
$BODY$
LANGUAGE plpgsql;


SELECT * FROM orders_style_pd_euclidean('_pgr_pickdelivereuclidean', ', 2)');

SELECT finish();
ROLLBACK;
