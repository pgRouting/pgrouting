
ALTER TABLE edges RENAME TO mbta_arc;
CREATE TABLE edges AS SELECT * FROM mbta_arc;;
SELECT pgr_CreateTopology('edges', 0.001, 'geom', 'id', clean := true);

