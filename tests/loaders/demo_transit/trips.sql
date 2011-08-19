CREATE TABLE trips
(
  route_id          text NOT NULL REFERENCES routes ON DELETE CASCADE,
  service_id        text NOT NULL REFERENCES calendar,
  trip_id           text NOT NULL PRIMARY KEY,
  trip_headsign     text NULL,
  trip_short_name   text NULL,
  direction_id      boolean NULL,
  block_id          text NULL,
  shape_id          text NULL REFERENCES shapes
);
