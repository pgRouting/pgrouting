DROP TABLE IF EXISTS simplegraph;

CREATE TABLE simplegraph
(
		id	serial primary key,
		source	integer,
		target	integer,
		cost	double precision
);
