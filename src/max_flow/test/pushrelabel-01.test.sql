--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata


--Calculates the max flow from source #3 to sink #5.
--Calculates the max flow from source #6 to sink #11.
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table AS edges, category as c1, category as c2
    WHERE edges.category = c1.category AND edges.reverse_category = c2.category'
    , 6, 11
);
