--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata


--Calculates the max flow from source #3 to sink #5.
SELECT * FROM pgr_pushrelabel('SELECT * FROM edge_table', 3, 5);
