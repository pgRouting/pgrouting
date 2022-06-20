/* --q1 */
SELECT * FROM pgr_bipartite(
    $$SELECT id, source, target, cost, reverse_cost FROM edges$$
) ORDER BY vertex_id;
/* --q2 */
INSERT INTO edges (source, target, cost, reverse_cost) VALUES
(5, 1, 1, 1);
/* --q3 */
SELECT * FROM pgr_bipartite(
    $$SELECT id, source, target, cost, reverse_cost FROM edges$$
);
/* --q4 */
