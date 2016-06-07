-- http://www.math.uwaterloo.ca/tsp/world/wi29.tsp
-- http://www.math.uwaterloo.ca/tsp/world/countries.html

-- NAME : wi29
-- COMMENT : 29 locations in Western Sahara
-- COMMENT : Derived FROM National Imagery and Mapping Agency data
-- TYPE : TSP
-- DIMENSION : 29
-- EDGE_WEIGHT_TYPE : EUC_2D
-- NODE_COORD_SECTION

-- best 27603

DROP TABLE IF EXISTS wi29;
CREATE TABLE wi29 (id BIGINT, x FLOAT, y FLOAT, the_geom geometry);
COPY wi29 (id, x, y) FROM stdin WITH DELIMITER ' ';
1 20833.3333 17100.0000
2 20900.0000 17066.6667
3 21300.0000 13016.6667
4 21600.0000 14150.0000
5 21600.0000 14966.6667
6 21600.0000 16500.0000
7 22183.3333 13133.3333
8 22583.3333 14300.0000
9 22683.3333 12716.6667
10 23616.6667 15866.6667
11 23700.0000 15933.3333
12 23883.3333 14533.3333
13 24166.6667 13250.0000
14 25149.1667 12365.8333
15 26133.3333 14500.0000
16 26150.0000 10550.0000
17 26283.3333 12766.6667
18 26433.3333 13433.3333
19 26550.0000 13850.0000
20 26733.3333 11683.3333
21 27026.1111 13051.9444
22 27096.1111 13415.8333
23 27153.6111 13203.3333
24 27166.6667 9833.3333
25 27233.3333 10450.0000
26 27233.3333 11783.3333
27 27266.6667 10383.3333
28 27433.3333 12400.0000
29 27462.5000 12992.2222
\.


UPDATE wi29 SET the_geom = ST_makePoint(x,y);
SET client_min_messages TO NOTICE;

SELECT * from pgr_tsp($$SELECT id::INTEGER, x, y FROM wi29$$, 17);

SELECT * FROM pgr_eucledianTSP($$select * FROM wi29$$, 17, randomize := false);
SELECT * FROM pgr_eucledianTSP($$select * FROM wi29$$, 17, 25, randomize := false);

UPDATE wi29 SET id = id + 100;
SELECT * FROM pgr_eucledianTSP($$select * FROM wi29$$, 117, 125, randomize := false);
SELECT * FROM pgr_eucledianTSP($$select * FROM wi29$$, randomize := false);


/*
CREATE VIEW wi29_path AS
WITH
results AS (
    SELECT * FROM pgr_eucledianTSP($$select * FROM wi29$$);
),
geoms AS (
    SELECT seq, node, cost, agg_cost, the_geom AS second  FROM results JOIN wi29 ON (node = id)
),
edges AS (
    SELECT seq, node, ST_MakeLine(lag(second) over(), second) AS line, cost, agg_cost FROM geoms
)
SELECT * FROM edges;
*/
