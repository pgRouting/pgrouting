/* --q1 */
SELECT * FROM pgr_lengauertarjandominatortree(
  $$SELECT id,source,target,cost,reverse_cost FROM edge_table$$,
  5);
/* --q2 */
SELECT * FROM pgr_lengauertarjandominatortree(
  $$SELECT id,source,target,cost,reverse_cost FROM edge_table$$,
  13);
/* --q3 */
