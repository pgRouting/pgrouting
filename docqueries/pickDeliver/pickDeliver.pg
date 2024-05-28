-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
SET extra_float_digits=-3;
/* -- q1 */
SELECT id, capacity, start_node_id, start_open, start_close
FROM vehicles;
/* -- q2 */
SELECT id, demand,
       p_node_id, p_open, p_close, p_service,
       d_node_id, d_open, d_close, d_service
FROM orders;
/* -- q3 */
SELECT * FROM pgr_pickDeliver(
  $$SELECT id, demand,
       p_node_id, p_open, p_close, p_service,
       d_node_id, d_open, d_close, d_service
    FROM orders$$,
  $$SELECT id, capacity, start_node_id, start_open, start_close
    FROM vehicles$$,
  $$SELECT * from pgr_dijkstraCostMatrix(
    'SELECT * FROM edges ',
    (SELECT array_agg(id) FROM (SELECT p_node_id AS id FROM orders
        UNION
        SELECT d_node_id FROM orders
        UNION
        SELECT start_node_id FROM vehicles) a))
  $$);
/* -- q4 */
