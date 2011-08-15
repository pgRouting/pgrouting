DROP TABLE IF EXISTS demo_transit.stop_times;

CREATE TABLE demo_transit.stop_times
(
		trip_id	text NOT NULL REFERENCES demo_transit.trips ON DELETE CASCADE,
		arrival_time	gtfstime NOT NULL,
		departure_time	gtfstime NOT NULL,
		stop_id	text NOT NULL REFERENCES demo_transit.stops ON DELETE CASCADE,
		stop_sequence	integer NOT NULL,
		stop_headsign	text NULL,
		pickup_type	integer NULL CHECK(pickup_type >= 0 and pickup_type <=3),
		drop_off_type	integer NULL CHECK(drop_off_type >= 0 and drop_off_type <=3),
		shape_dist_traveled	double precision NULL
);
