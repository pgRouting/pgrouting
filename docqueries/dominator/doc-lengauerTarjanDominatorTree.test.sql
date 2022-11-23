-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* --q1 */
SELECT * FROM pgr_lengauertarjandominatortree(
  $$SELECT id,source,target,cost,reverse_cost FROM edges$$,
  5) ORDER BY vertex_id;
/* --q2 */
SELECT * FROM pgr_lengauertarjandominatortree(
  $$SELECT id,source,target,cost,reverse_cost FROM edges$$,
  13) ORDER BY vertex_id;
/* --q3 */
