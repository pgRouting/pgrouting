DROP TABLE IF EXISTS demo_transit.frequencies;

CREATE TABLE demo_transit.frequencies
(
		trip_id	text NOT NULL REFERENCES demo_transit.trips ON DELETE CASCADE,
		start_time	gtfstime NOT NULL,
		end_time	gtfstime NOT NULL,
		headway_secs	integer	NOT NULL
);
