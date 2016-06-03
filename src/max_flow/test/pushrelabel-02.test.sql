--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata


--Calculates only the max flow value.
SELECT sum(flow) AS max_flow
FROM pgr_pushrelabel('SELECT * FROM edge_table', 3, 5)
WHERE tail = 3; -- Condition can be replaced with head = 5
