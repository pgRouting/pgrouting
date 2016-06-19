--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata


--Calculates the max flow from source #9 to sink #2.
SELECT * FROM pgr_maxflowedmondskarp('SELECT * FROM edge_table', 9, 2);
