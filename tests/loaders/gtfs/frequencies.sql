DROP TABLE IF EXISTS gtfs.frequencies;

CREATE TABLE gtfs.frequencies
(
		trip_id	text NOT NULL REFERENCES gtfs.trips ON DELETE CASCADE,
		start_time	gtfstime NOT NULL,
		end_time	gtfstime NOT NULL,
		headway_secs	integer	NOT NULL
);
