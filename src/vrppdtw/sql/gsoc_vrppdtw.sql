
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
    customer_tmp AS (' || $1 || '),
    pickups AS (
        SELECT id, demand, x as pick_x, y as pick_y, opentime as pick_open, closetime as pick_close, servicetime as pick_service
        FROM customer_tmp WHERE pindex = 0 AND id != 0
    ),
    deliveries AS (
        SELECT pindex AS id, x as deliver_x, y as deliver_y, opentime as deliver_open, closetime as deliver_close, servicetime as deliver_service
        FROM customer_tmp WHERE dindex = 0 AND id != 0
    )
    SELECT * 
    FROM pickups JOIN deliveries USING(id) ORDER BY pickups.id)';

    EXECUTE orders_sql;

    results_sql :='
    WITH
    customer_tmp AS (' || $1 || '),
    results AS (
        SELECT seq, order_id, vehicle_id, stop_type, departure_time FROM _pgr_pickDeliver(
            $$ SELECT * FROM __vrp__orders $$, 
            $$ WITH
                customer_tmp1 AS (' || $1 || ')
                SELECT id,
                    x AS start_x, y AS start_y,
                    opentime AS start_open, closetime AS start_close, '
                || $2 || ' AS number,' || $3 || ' AS capacity FROM customer_tmp1 WHERE id = 0$$,
         30)
    )
    SELECT seq::INTEGER, vehicle_id::INTEGER AS id1,
        CASE
            WHEN stop_type = 2 THEN  dindex
            ELSE id   
        END::INTEGER AS id2,
        departure_time AS cost
        FROM customer_tmp JOIN results
        ON (customer_tmp.id = results.order_id)';

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
