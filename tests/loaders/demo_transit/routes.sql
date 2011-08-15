DROP TABLE IF EXISTS demo_transit.routes;

CREATE TABLE demo_transit.routes
(
		route_id	text PRIMARY KEY,
		agency_id	text NULL REFERENCES demo_transit.agency(agency_id) ON DELETE CASCADE,
		route_short_name	text NOT NULL,
		route_long_name	text NOT NULL,
		route_desc	text NULL,
		route_type	integer NOT NULL CHECK(route_type >= 0 and route_type <= 7),
		route_url	text NULL,
		route_color	text NULL,
		route_text_color	text NULL
);
