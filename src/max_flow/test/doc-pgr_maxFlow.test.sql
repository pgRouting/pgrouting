--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata
--The edge in the edge table are augmented with road-like categories for capacity values.

BEGIN;


\echo -- q1
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id
    ORDER BY id'
    , 6, 11
);

\echo -- q2
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id
    ORDER BY id'
    , 6, ARRAY[11, 1, 13]
);

\echo -- q3
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id
    ORDER BY id'
    , ARRAY[6, 8, 12], 11
);

\echo -- q4
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id
    ORDER BY id'
    , ARRAY[6, 8, 12], ARRAY[1, 3, 11]
);

\echo -- q5

ROLLBACK;
