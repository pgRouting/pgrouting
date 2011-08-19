CREATE TABLE frequencies
(
		trip_id	text NOT NULL,
		start_time	interval NOT NULL,
		end_time	interval NOT NULL,
		headway_secs	integer	NOT NULL
);
