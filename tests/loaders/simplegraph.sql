DROP TABLE IF EXISTS simplegraph;

CREATE TABLE simplegraph
(
		id	serial primary key,
		source	integer,
		target	integer,
		cost	double precision
);

COPY simplegraph FROM STDIN WITH CSV HEADER;
id, source, target, cost
1, 1000, 1001, 1.29
2, 1000, 1002, 0.11
3, 1002, 1003, 0.32
4, 1001, 1003, 0.23
\.
