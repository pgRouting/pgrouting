SELECT * FROM orders ORDER BY id;
SELECT * FROM vehicles ORDER BY id;

WITH
A AS (
    SELECT p_node_id AS id, p_x AS x, p_y AS y FROM orders
    UNION
    SELECT d_node_id AS id, d_x, d_y FROM orders
    UNION
    SELECT start_node_id, start_x, start_y FROM vehicles
)
SELECT A.id AS start_vid, B.id AS end_vid, sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) AS agg_cost
FROM A, A AS B
WHERE A.id != B.id AND A.id in (89, 25, 101) AND B.id in (89, 25, 101)
ORDER BY start_vid, end_vid;


\echo --q1
-- using the default values
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
