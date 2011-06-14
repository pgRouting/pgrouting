DROP TABLE IF EXISTS nonsc.stop_times;

CREATE TABLE nonsc.stop_times (
    trip_id text NOT NULL,
    stop_id text NOT NULL,
    arrival_time gtfstime NOT NULL,
    departure_time gtfstime NOT NULL
);
