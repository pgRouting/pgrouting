--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata

SELECT * FROM pgr_maximumcardinalitymatching(
    'SELECT * FROM pgr_splitedges(''
        SELECT id, source, target, cost, reverse_cost FROM edge_table
    '')',
    TRUE
);

