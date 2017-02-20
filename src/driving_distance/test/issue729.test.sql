ALTER TABLE edge_table
DROP COLUMN IF EXISTS node_count,
ADD COLUMN node_count INTEGER;

UPDATE edge_table AS edge_table
SET node_count=count.sum
FROM
        (SELECT
        from_v,
        sum(node) AS sum
        FROM
            pgr_drivingDistance(
            'SELECT id, source, target, ST_Length(the_geom) AS cost FROM edge_table',
            ARRAY(SELECT DISTINCT source FROM edge_table),
            1,
            false)
    GROUP BY from_v) AS count
    WHERE edge_table.source=count.from_v
    ;

ALTER TABLE edge_table
DROP COLUMN IF EXISTS another_column,
ADD COLUMN another_column INTEGER;
