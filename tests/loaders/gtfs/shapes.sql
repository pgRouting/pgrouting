DROP TABLE IF EXISTS gtfs.shapes;

CREATE TABLE gtfs.shapes
(
		shape_id	text PRIMARY KEY,
		shape_pt_lat	wgs84_lat NOT NULL,
		shape_pt_lon	wgs84_lon NOT NULL,
		shape_pt_sequence	integer NOT NULL,
		shape_dist_traveled	double precision NULL
);
