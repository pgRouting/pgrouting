DROP TABLE IF EXISTS jet_stops;
DROP TABLE IF EXISTS jet_flyers;
DROP TABLE IF EXISTS jet_orders;
DROP TABLE IF EXISTS jet_airplane;

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


CREATE TABLE jet_flyers AS
    SELECT
    row_number() over() AS id,
    s1.iata_faa  AS from_airport,
    s2.iata_faa  AS to_airport,
    (1 + mod(s1.id, 5))::integer AS num_passengers,
    -- the passengers dont want to depart before this time
    mod(s1.id,7)*60 AS departureFromTime,
    -- the passengers dont want to arrive after this time
    (mod(s1.id,7) + 4) * 60 AS arrivalToTime
    FROM
    (SELECT * FROM jet_stops) AS s1 ,
    (SELECT * FROM jet_stops) AS s2
    WHERE s1.id <> s2.id;


-- Listing Create jet_orders table to be used in pgr_pickDeliverEuclidean
CREATE TABLE jet_orders AS
    SELECT
    id,
    num_passengers AS demand,
    (SELECT x FROM jet_stops WHERE iata_faa = jet_flyers.from_airport) AS pick_x,
    (SELECT y FROM jet_stops WHERE iata_faa = jet_flyers.from_airport) AS pick_y,
    
    departureFromTime AS pick_open,
    arrivalToTime AS pick_close,

    (SELECT x FROM jet_stops WHERE iata_faa = jet_flyers.to_airport) AS deliver_x,
    (SELECT y FROM jet_stops WHERE iata_faa = jet_flyers.to_airport) AS deliver_y,

    departureFromTime AS deliver_open,
    arrivalToTime AS deliver_close

    FROM jet_flyers;

 
--
CREATE TABLE jet_airplane AS
SELECT
    1 AS id,
    0 AS capacity,
    272 AS speed, -- mts/min
    20 as "number",
    x AS start_x, y AS start_y,
    0 AS start_open,
    100000 AS start_close,
    0 AS start_service
    FROM jet_stops
    WHERE iata_faa = 'TEB';

SELECT * FROM _pgr_pickDeliverEuclidean(
    $$ SELECT * FROM jet_orders $$,
    $$ SELECT * FROM jet_airplane $$
);


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
