-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
UPDATE edges SET id = id * 100, source = 1000 * source, target = 1000 * target;
UPDATE vertices SET id = id * 1000;

SELECT id, source, target, cost, reverse_cost FROM edges;
SELECT id FROM vertices;

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost FROM edges WHERE id = 1$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edges
    WHERE id = 200$$
);
SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edges
    WHERE id = 1100$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edges
    WHERE id = 100$$
);

SELECT  *  FROM pgr_lineGraphFull($$
    SELECT id AS id, target, source, reverse_cost AS cost FROM edges WHERE id = 100
    UNION
    SELECT id, source, target, cost FROM edges WHERE id = 100
    $$
);

SELECT  *  FROM pgr_lineGraphFull($$
    SELECT id, target, source, reverse_cost AS cost FROM edges WHERE id = 100
    UNION
    SELECT -id AS id, source, target, cost FROM edges WHERE id = 100
    $$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edges
    WHERE id IN (200, 300)$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id * 100 AS id, source * 1000 AS source, target * 1000 AS target,
        cost, reverse_cost
    FROM edges
    WHERE id in (600, 700)$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id * 100 AS id, source * 1000 AS source, target * 1000 AS target,
        cost, reverse_cost
    FROM edges
    where id in (500, 900, 1100)$$
);

