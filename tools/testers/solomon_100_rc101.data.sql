
DROP TABLE IF EXISTS solomon_100_RC_101 cascade;
CREATE TABLE solomon_100_RC_101 (
    id integer NOT NULL PRIMARY KEY,
    order_unit integer,
    open_time integer,
    close_time integer,
    service_time integer,
    x float8,
    y float8
);

COPY solomon_100_RC_101
(id, x, y, order_unit, open_time, close_time, service_time) FROM stdin;
1	40.000000	50.000000	0	0	240	0
2	25.000000	85.000000	20	145	175	10
3	22.000000	75.000000	30	50	80	10
4	22.000000	85.000000	10	109	139	10
5	20.000000	80.000000	40	141	171	10
6	20.000000	85.000000	20	41	71	10
7	18.000000	75.000000	20	95	125	10
8	15.000000	75.000000	20	79	109	10
9	15.000000	80.000000	10	91	121	10
10	10.000000	35.000000	20	91	121	10
11	10.000000	40.000000	30	119	149	10
\.

DROP TABLE IF EXISTS vrp_vehicles cascade;
CREATE TABLE vrp_vehicles (
    vehicle_id integer not null primary key,
    capacity integer,
    case_no integer
);

copy vrp_vehicles (vehicle_id, capacity, case_no) from stdin;
1	200	5
2	200	5
3	200	5
\.

DROP TABLE IF EXISTS vrp_distance cascade;
WITH
the_matrix_info AS (
    SELECT A.id AS src_id, B.id AS dest_id, sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) AS cost
    FROM solomon_100_rc_101 AS A, solomon_100_rc_101 AS B WHERE A.id != B.id
)
SELECT src_id, dest_id, cost, cost AS distance, cost AS traveltime
INTO vrp_distance
FROM the_matrix_info;
