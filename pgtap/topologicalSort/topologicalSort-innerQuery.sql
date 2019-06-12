\i setup.sql
SELECT plan(1);

SELECT * FROM pgr_topologicalSort(
    'SELECT id, source, target, cost, reverse_cost
        FROM edge_table
     WHERE id = 18'
);
SELECT finish();
ROLLBACK;
