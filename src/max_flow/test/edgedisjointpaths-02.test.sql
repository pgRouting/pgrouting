--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata

SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, 5,
    FALSE
);

