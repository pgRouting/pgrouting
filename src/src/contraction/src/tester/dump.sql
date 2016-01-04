DROP TABLE IF EXISTS test_ways;
DROP TABLE IF EXISTS test_ways_vertices;
DROP TABLE IF EXISTS contracted_test_ways_0;
DROP TABLE IF EXISTS final_contracted_ways_0;

SELECT gid AS id,source,target,cost,reverse_cost AS revcost,x1,y1,x2,y2  INTO test_ways FROM ways;
SELECT id,lon,lat INTO test_ways_vertices FROM ways_vertices_pgr;

CREATE table contracted_test_ways_0(id INT PRIMARY KEY NOT NULL,source INT NOT NULL,
target INT NOT NULL,cost REAL NOT NULL);

COPY contracted_test_ways_0 FROM '/home/rohith/mystuff/labwork/3-1/codes/OSMContraction/code/src/contraction/src/tester/contracted_ways.txt' DELIMITER ',' CSV;

CREATE TABLE final_contracted_ways_0(id INT PRIMARY KEY NOT NULL,source INT NOT NULL,
target INT NOT NULL,cost REAL NOT NULL,x1 REAL NOT NULL,y1 REAL NOT NULL,x2 REAL ,y2 REAL );

SELECT AddGeometryColumn('public','test_ways','the_geom',4326,'LINESTRING',2);

UPDATE test_ways SET 
the_geom=ST_SetSRID(ST_MakeLine(ST_MakePoint(x1,y1),ST_MakePoint(x2,y2)),4326);


SELECT AddGeometryColumn('public','final_contracted_ways_0','the_geom',4326,'LINESTRING',2);


INSERT INTO  final_contracted_ways_0(id,source,target,cost,x1,y1) 
SELECT t1.id,t1.source,t1.target,t1.cost,t2.lon,t2.lat
FROM contracted_test_ways_0 AS t1,test_ways_vertices AS t2
WHERE t1.source=t2.id;


UPDATE final_contracted_ways_0
SET x2=lon,y2=lat,the_geom=ST_SetSRID(ST_MakeLine(ST_MakePoint(x1,y1),ST_MakePoint(lon,lat)),4326)
FROM  test_ways_vertices
WHERE target=test_ways_vertices.id;