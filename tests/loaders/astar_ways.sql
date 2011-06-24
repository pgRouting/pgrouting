DROP TABLE IF EXISTS astar_ways;

CREATE TABLE astar_ways
(
   id       serial primary key,
   source   integer,
   target   integer,
   cost     double precision
);
