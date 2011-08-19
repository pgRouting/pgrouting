DROP TABLE IF EXISTS stop_times;

CREATE TABLE stop_times (
    trip_id text NOT NULL,
    stop_id text NOT NULL,
    arrival_time interval NOT NULL,
    departure_time interval NOT NULL
);
