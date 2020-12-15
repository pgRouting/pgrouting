SET extra_float_digits=-3;
\echo --q1
SELECT * FROM pgr_pickDeliver(
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

SELECT * FROM pgr_pickDeliver(
    $$ SELECT * FROM orders ORDER BY id $$,
    $$ SELECT * FROM vehicles ORDER BY id$$,
    $$ SELECT * from pgr_dijkstraCostMatrix(
        'SELECT * FROM edge_table ',
        (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a))
    $$
);

\echo --q3
