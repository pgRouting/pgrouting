--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata


--Calculates the max flow from source #6 to sink #11.
SELECT * FROM pgr_maxflowboykovkolmogorov('SELECT * FROM edge_table', 6, 11);