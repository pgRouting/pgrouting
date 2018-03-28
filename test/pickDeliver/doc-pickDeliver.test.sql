\echo --q1
SELECT * FROM _pgr_pickDeliver(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles',
    -- matrix query
    'WITH
    A AS (
        SELECT p_node_id AS id, p_x AS x, p_y AS y FROM orders
        UNION
        SELECT d_node_id AS id, d_x, d_y FROM orders
        UNION
        SELECT start_node_id, start_x, start_y FROM vehicles
    )
    SELECT A.id AS start_vid, B.id AS end_vid, sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) AS agg_cost
    FROM A, A AS B WHERE A.id != B.id'
    );

\echo --q2

SELECT * FROM _pgr_pickDeliver(
    $$ SELECT * FROM orders ORDER BY id $$,
    $$ SELECT * FROM vehicles $$,
    $$ SELECT * from pgr_dijkstraCostMatrix(
        ' SELECT * FROM edge_table ', ARRAY[3, 4, 5, 8, 9, 11])
    $$
);

\echo --q3
