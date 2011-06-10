DROP TABLE IF EXISTS nonsc.trip;
CREATE TABLE nonsc.trip (
    trip_id text PRIMARY KEY,
    headway_secs integer NOT NULL
);
