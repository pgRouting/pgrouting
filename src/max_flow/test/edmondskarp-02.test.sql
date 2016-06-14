--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata


/*
 * Calculates only the max flow value.
 * This works by aggregating on the outgoing flow on the source, or on the
 * incoming flow on the sink.
 */
SELECT sum(flow) AS max_flow
FROM pgr_edmondskarp('SELECT * FROM edge_table', 9, 2)
WHERE tail = 9; -- Condition can be replaced with head = 5
