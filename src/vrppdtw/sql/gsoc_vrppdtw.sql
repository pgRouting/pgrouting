
CREATE OR REPLACE FUNCTION pgr_gsoc_vrppdtw(
    sql text,
    vehicle_num INTEGER,
    capacity INTEGER
)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
    has_reverse BOOLEAN;
    orders_sql TEXT;
    results_sql TEXT;
BEGIN

    /* the query has to meet the wanted queries in pick_delivery */

    orders_sql :=
    'CREATE TEMP TABLE __vrp__orders ON COMMIT DROP AS (
    WITH
    curstomer AS (' || $1 || '),
    pickups AS (
        SELECT id, demand, x as pick_x, y as pick_y, opentime as pick_open, closetime as pick_close, servicetime as pick_service
        FROM customer where pindex = 0
    ),
    deliveries AS (
        SELECT pindex AS id, x as deliver_x, y as deliver_y, opentime as deliver_open, closetime as deliver_close, servicetime as deliver_service
        FROM customer where dindex = 0
    )
    SELECT * 
    FROM pickups JOIN deliveries USING(id) ORDER BY pickups.id)';

    EXECUTE orders_sql;

    results_sql :='
    WITH
    curstomer AS (' || $1 ||'),
    results AS (
        SELECT seq, order_id, vehicle_id, stop_type, departure_time FROM _pgr_pickDeliver(
            $$ SELECT * FROM __vrp__orders $$, 
            ' || $2 || ',' || $3 || ', 1, 30) WHERE vehicle_id != -1
    )
    SELECT seq::INTEGER, vehicle_id::INTEGER AS id1,
        CASE
            WHEN stop_type = 2 THEN  dindex
            ELSE id   
        END::INTEGER AS id2,
        departure_time AS cost
        FROM customer JOIN results
        ON (customer.id = results.order_id)';

    RETURN query EXECUTE results_sql;
    DROP TABLE IF EXISTS __vrp__orders;

    EXCEPTION WHEN others THEN 
        DROP TABLE IF EXISTS __vrp__orders;
        RAISE EXCEPTION '% %', SQLERRM, SQLSTATE;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
