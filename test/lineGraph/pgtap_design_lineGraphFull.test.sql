UPDATE edge_table SET id = id * 100, source = 1000 * source, target = 1000 * target;
UPDATE edge_table_vertices_pgr SET id = id * 1000;

SELECT id, source, target, cost, reverse_cost FROM edge_table;
SELECT id FROM edge_table_vertices_pgr;

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost FROM edge_table WHERE id = 1$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 200$$
);
SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 1100$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id = 100$$
);

SELECT  *  FROM pgr_lineGraphFull($$
    SELECT id AS id, target, source, reverse_cost AS cost FROM edge_table WHERE id = 100
    UNION
    SELECT id, source, target, cost FROM edge_table WHERE id = 100
    $$
);

SELECT  *  FROM pgr_lineGraphFull($$
    SELECT id, target, source, reverse_cost AS cost FROM edge_table WHERE id = 100
    UNION
    SELECT -id AS id, source, target, cost FROM edge_table WHERE id = 100
    $$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id IN (200, 300)$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id * 100 AS id, source * 1000 AS source, target * 1000 AS target,
        cost, reverse_cost
    FROM edge_table
    WHERE id in (600, 700)$$
);

SELECT  *  FROM pgr_lineGraphFull(
    $$SELECT id * 100 AS id, source * 1000 AS source, target * 1000 AS target,
        cost, reverse_cost
    FROM edge_table
    where id in (500, 900, 1100)$$
);

