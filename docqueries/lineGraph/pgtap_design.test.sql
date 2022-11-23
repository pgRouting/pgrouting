-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
-- TODO move to pgtap

/* -- one edge graph */
SELECT * FROM pgr_lineGraph(
    'SELECT id, source, target, cost FROM edges WHERE id = 1'
);

/* -- two edge graphs */
SELECT * FROM pgr_lineGraph($$
    SELECT -id AS id, target, source, reverse_cost AS cost FROM edges WHERE id = 1
    UNION
    SELECT id, source, target, cost FROM edges WHERE id = 1
    $$
);
SELECT * FROM pgr_lineGraph($$
    SELECT id, target, source, reverse_cost AS cost FROM edges WHERE id = 1
    UNION
    SELECT -id AS id, source, target, cost FROM edges WHERE id = 1
    $$
);
SELECT * FROM pgr_lineGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edges WHERE id = 1'
);
SELECT * FROM pgr_lineGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edges WHERE id = 17'
);

