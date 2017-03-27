DROP TABLE IF EXISTS jet_stops;
DROP TABLE IF EXISTS jet_orders;

CREATE TABLE jet_stops (
    id SERIAL,
    iata_faa TEXT,
    x integer,
    y integer,
    longitude double precision,
    latitude double precision
);


INSERT INTO jet_stops (iata_faa, longitude, latitude) VALUES
('BOS',  -71.005180999999993, 42.364347000000002),
('BGR',  -68.828138999999993, 44.807443999999997),
('MIA',  -80.290555999999995, 25.79325),
('TEB',  -74.060837000000006, 40.850102999999997),
('MVY',  -70.615278000000004, 41.391666999999998),
('ABE',  -75.440805999999995, 40.652082999999998),
('BKW',  -81.124200000000002, 37.787300000000002),
('BGE',  -84.636927799999995, 30.971598100000001);


UPDATE jet_stops SET
x = ST_X(ST_Transform(ST_SetSRID(ST_point(longitude, latitude)::geometry,4326),2163)),
y = ST_Y(ST_Transform(ST_SetSRID(ST_point(longitude, longitude)::geometry,4326),2163));


-- Listing  Create jet_customers table --page 190
-- create dummy customers for pick and deliver
CREATE TABLE jet_orders AS
WITH
stops AS (select * from stops)
,
pairs AS (
    SELECT
    ((row_number() OVER())::integer * 2 - 1) AS pid,
    ((row_number() OVER())::integer * 2) AS did,
    s1.iata_faa AS d_airport,
    s2.iata_faa AS a_airport,
    s1.x AS p_x, s1.y AS p_y, s2.x AS d_x, s2.y AS d_y,
    (1 + mod(s1.id,5))::integer AS num_passengers,
    mod(s1.id,7)*60 AS openTime,
    (
        (mod(s1.id,7) + 2) * 60
    )::float AS closeTime,
    0 AS serviceTime
    FROM
    (SELECT * FROM jet_stops) AS s1 ,
    (SELECT * FROM jet_stops) AS s2
    WHERE s1.id <> s2.id
)
SELECT * FROM pairs;

/*
SELECT
d_airport AS airport,
pid AS id, p_x AS x, p_y AS y, 0 AS pIndex, did AS dIndex,
num_passengers AS demand,
openTime, closeTime, serviceTime
FROM pairs AS pickups
UNION ALL
SELECT
a_airport AS airport,
did AS id, d_x AS x, d_y AS y, pid AS pIndex, 0 AS dIndex,
-num_passengers AS demand,
openTime, closeTime + 2*981*1000 AS closeTime, serviceTime
FROM pairs AS delivers
UNION ALL
SELECT
'TEB' AS airport,
0 AS id, x, y, 0 AS pIndex, 0 AS dIndex, 0 AS demand,
0 AS openTime, 1000*981*12 AS closeTime, 0 AS serviceTime
FROM stops
WHERE iata_faa = 'TEB';
*/
