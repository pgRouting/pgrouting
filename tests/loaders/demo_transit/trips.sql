DROP TABLE IF EXISTS demo_transit.trips;

CREATE TABLE demo_transit.trips
(
		route_id	text NOT NULL REFERENCES demo_transit.routes ON DELETE CASCADE,
		service_id	text NOT NULL REFERENCES demo_transit.calendar,
		trip_id	text NOT NULL PRIMARY KEY,
		trip_headsign	text NULL,
		trip_short_name	text NULL,
		direction_id	integer NULL CHECK(direction_id IN (0,1)),
		block_id	text NULL,
		shape_id	text NULL REFERENCES demo_transit.shapes
);
