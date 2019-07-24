
ALTER TABLE edge_table RENAME TO mbta_arc;
CREATE TABLE edge_table AS SELECT * FROM mbta_arc;;
SELECT pgr_CreateTopology('edge_table', 0.001, 'the_geom', 'id', clean := true);

