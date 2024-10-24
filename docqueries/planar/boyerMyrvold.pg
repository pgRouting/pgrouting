-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
        FROM edges'
);

/* -- q2 */
INSERT INTO edges (source, target, cost, reverse_cost) VALUES
(1,3,1,-1),(1,4,1,-1),(1,5,1,-1),(2,4,1,-1),(2,5,1,-1),(3,5,1,-1),(4,5,1,-1);

/* -- q3 */

SELECT * FROM pgr_boyerMyrvold(
    'SELECT id, source, target, cost, reverse_cost
        FROM edges'
);

/* -- q4 */
SELECT * FROM pgr_boyerMyrvold(
$$
 SELECT id, source, target, cost, reverse_cost FROM edges
    where source = any (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edges ')
                        WHERE component = 14)
                       )
                   OR
          target = any (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edges ')
                        WHERE component = 14)
                       )
$$
 );

 /* -- q5 */
