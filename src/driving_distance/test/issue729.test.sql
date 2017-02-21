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


DROP TABLE IF EXISTS network;
CREATE TABLE network AS
SELECT *
from (
    VALUES
    (1::int, 1::int, 2::int, 100::int),
    (2::int, 2::int, 3::int, 100::int),
    (3::int, 3::int, 4::int, 100::int),
    (4::int, 5::int, 6::int, 100::int)
) as t (id, source,target,cost);

ALTER TABLE network
DROP COLUMN IF EXISTS node_count,
ADD COLUMN node_count INTEGER;

UPDATE network AS network
SET node_count=count.sum
FROM
(SELECT
    from_v,
    sum(node) AS sum
    FROM
    pgr_drivingDistance(
        'SELECT id, source, target, cost FROM network',
        ARRAY(SELECT DISTINCT source FROM network),
        1,
        false)
    GROUP BY from_v) AS count
WHERE network.source=count.from_v
;

ALTER TABLE network
DROP COLUMN IF EXISTS another_column,
ADD COLUMN another_column INTEGER;


