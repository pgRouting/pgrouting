DROP TABLE IF EXISTS jet_stops;
DROP TABLE IF EXISTS jet_flyers;
DROP TABLE IF EXISTS jet_orders;
DROP TABLE IF EXISTS jet_airplane;
DROP TABLE IF EXISTS jet_trips;
DROP TABLE IF EXISTS vrp_solution;

CREATE TABLE jet_stops (
    id SERIAL,
    iata_faa TEXT,
    city TEXT,
    x integer,
    y integer,
    longitude double precision,
    latitude double precision
);


INSERT INTO jet_stops (iata_faa, city, longitude, latitude) VALUES
('BOS',  'Boston', -71.005180999999993, 42.364347000000002),
('BGR',  'Bangor', -68.828138999999993, 44.807443999999997),
('MIA',  'Miami',  -80.290555999999995, 25.79325),
('TEB',  'Teterboro NJ', -74.060837000000006, 40.850102999999997),
('MVY',  'Martha''s Vineyard', -70.615278000000004, 41.391666999999998),
('ABE',  'Lehigh Valley', -75.440805999999995, 40.652082999999998),
('BKW',  'Raleigh County Memorial', -81.124200000000002, 37.787300000000002),
('BGE',  'Decatur County Industrial', -84.636927799999995, 30.971598100000001);


UPDATE jet_stops SET
x = ST_X(ST_Transform(ST_SetSRID(ST_point(longitude, latitude)::geometry,4326),2163)),
y = ST_Y(ST_Transform(ST_SetSRID(ST_point(longitude, longitude)::geometry,4326),2163));


CREATE TABLE jet_flyers AS
    SELECT
    row_number() over() AS id,
    s1.iata_faa  AS from_airport,
    s2.iata_faa  AS to_airport,
    (1 + mod(s1.id, 5))::integer AS num_passengers,
    -- the passengers don't want to depart before this time
    mod(s1.id,7)*60 AS departureFromTime,
    -- the passengers don't want to arrive after this time
    (mod(s1.id,7) + 4) * 60 AS arrivalToTime
    FROM
    -- people departing from BOS BGR MIA TEB
    (SELECT * FROM jet_stops WHERE id < 4) AS s1,
    -- people want to go to MVY ABE BKW BGE
    (SELECT * FROM jet_stops WHERE id >= 4) AS s2
    WHERE s1.id <> s2.id;


-- Listing Create jet_orders table to be used in pgr_pickDeliverEuclidean
CREATE TABLE jet_orders AS
    SELECT
    id,
    num_passengers AS demand,
    (SELECT x FROM jet_stops WHERE iata_faa = jet_flyers.from_airport) AS p_x,
    (SELECT y FROM jet_stops WHERE iata_faa = jet_flyers.from_airport) AS p_y,

    departureFromTime AS p_open,
    arrivalToTime AS p_close,

    (SELECT x FROM jet_stops WHERE iata_faa = jet_flyers.to_airport) AS d_x,
    (SELECT y FROM jet_stops WHERE iata_faa = jet_flyers.to_airport) AS d_y,

    departureFromTime AS d_open,
    arrivalToTime AS d_close,

    from_airport, to_airport

    FROM jet_flyers;


--
CREATE TABLE jet_airplane AS
SELECT
    1 AS id,
    -- small plane
    5 AS capacity,
    -- not so fast
    10000 AS speed, -- mts/min
    20 as "number",
    x AS start_x, y AS start_y,
    0 AS start_open,
    300 AS start_close,
    0 AS start_service
    FROM jet_stops
    WHERE iata_faa = 'TEB';

CREATE TABLE vrp_solution AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$ SELECT * FROM jet_orders $$,
    $$ SELECT * FROM jet_airplane $$
);


CREATE TABLE jet_trips AS
WITH the_results AS (
SELECT vrp_solution.*,  from_airport, to_airport,
CASE WHEN stop_type = 1 THEN ST_POINT(p_x, p_y)
        WHEN stop_type = 2 THEN ST_POINT(d_x, d_y)
        WHEN stop_type = 0 THEN  ST_POINT(start_x, start_y)
    END AS geom
FROM vrp_solution LEFT JOIN  jet_orders on (order_id = jet_orders.id)
LEFT JOIN jet_airplane on (vehicle_id = jet_airplane.id)
)
SELECT *, ST_SetSRID(ST_MakeLine(geom, lead(geom) over(order by seq)),2163) AS lines from the_results;


SELECT * FROM jet_orders;

SELECT * FROM jet_trips;


