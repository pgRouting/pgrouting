DROP TABLE IF EXISTS gtfs.stops CASCADE;

CREATE TABLE gtfs.stops
(
		stop_id	text PRIMARY KEY,
		stop_code	text	UNIQUE NULL,
		stop_name	text NOT NULL,
		stop_desc	text	NULL,
		stop_lat	wgs84_lat NOT NULL,
		stop_lon	wgs84_lon NOT NULL,
		zone_id	text NULL,
		stop_url	text NULL,
		location_type	integer NULL CHECK(location_type IN (0,1)),
		parent_station	text NULL
);
