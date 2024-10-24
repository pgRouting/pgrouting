-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
SET extra_float_digits=-3;
/* -- q1 */
SELECT id, capacity, start_x, start_y, start_open, start_close
FROM vehicles;
/* -- q2 */
SELECT id, demand,
       p_x, p_y, p_open, p_close, p_service,
       d_x, d_y, d_open, d_close, d_service
FROM orders;
/* -- q3 */
SELECT * FROM pgr_pickDeliverEuclidean(
  $$SELECT id, demand,
       p_x, p_y, p_open, p_close, p_service,
       d_x, d_y, d_open, d_close, d_service
    FROM orders$$,
  $$SELECT id, capacity, start_x, start_y, start_open, start_close
    FROM vehicles$$);
/* -- q4 */
