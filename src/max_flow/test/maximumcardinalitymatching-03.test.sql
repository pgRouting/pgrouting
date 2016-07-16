--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata

SELECT * FROM pgr_maximumcardinalitymatching(
    'SELECT id, source, target FROM edge_table',
    FALSE
);

