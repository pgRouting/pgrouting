
ALTER TABLE edge_table RENAME TO mbta_arc;
DROP TABLE IF EXISTS edge_table_vertices_pgr;
CREATE TABLE edge_table AS SELECT * FROM mbta_arc;;
SELECT pgr_CreateTopology('edge_table', 0.001, 'geom', 'id', clean := true);

