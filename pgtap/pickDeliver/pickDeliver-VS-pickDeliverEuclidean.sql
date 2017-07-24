
\i setup.sql

SELECT plan(1);
SET client_min_messages TO ERROR;

PREPARE pd AS
SELECT seq, vehicle_seq, vehicle_id, stop_seq, stop_type,
    order_id, cargo,
    travel_time, arrival_time, wait_time, service_time, departure_time
FROM _pgr_pickDeliver(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles',
    -- matrix query
    'WITH
    A AS (
        SELECT p_node_id AS id, p_x AS x, p_y AS y FROM orders
        UNION
        SELECT d_node_id AS id, d_x, d_y FROM orders
        UNION
        SELECT start_node_id, start_x, start_y FROM vehicles
    )
    SELECT A.id AS start_vid, B.id AS end_vid, sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) AS agg_cost
    FROM A, A AS B WHERE A.id != B.id'
    );

PREPARE pd_e AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles');

SELECT set_eq('pd', 'pd_e');



SELECT finish();
ROLLBACK;
