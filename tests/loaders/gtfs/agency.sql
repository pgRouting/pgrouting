DROP TABLE IF EXISTS gtfs.agency CASCADE;

CREATE TABLE gtfs.agency
(
		agency_id	text UNIQUE NULL,
		agency_name	text NOT NULL,
		agency_url	text	NOT NULL,
		agency_timezone	text	NOT NULL,
		agency_lang	text NULL,
		agency_phone	text NULL
);
