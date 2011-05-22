DROP TABLE IF EXISTS gtfs.trips;

CREATE TABLE gtfs.trips
(
		route_id	text NOT NULL REFERENCES gtfs.routes,
		service_id	text NOT NULL REFERENCES gtfs.calendar,
		trip_id	text NOT NULL PRIMARY KEY,
		trip_headsign	text NULL,
		trip_short_name	text NULL,
		direction_id	integer NULL CHECK(direction_id IN (0,1)),
		block_id	text NULL,
		shape_id	text NULL REFERENCES gtfs.shapes
);
