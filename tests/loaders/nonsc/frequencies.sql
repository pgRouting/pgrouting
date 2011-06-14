DROP TABLE IF EXISTS nonsc.frequencies;

CREATE TABLE nonsc.frequencies
(
		trip_id	text NOT NULL,
		start_time	gtfstime NOT NULL,
		end_time	gtfstime NOT NULL,
		headway_secs	integer	NOT NULL
);
