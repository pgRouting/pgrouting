DROP TABLE IF EXISTS nonsc.stoptime;

CREATE TABLE nonsc.stoptime (
    trip_id text NOT NULL REFERENCES nonsc.trip,
    stop_id integer NOT NULL,
    stop_time integer NOT NULL
);
