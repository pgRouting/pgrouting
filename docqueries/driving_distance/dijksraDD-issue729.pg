-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
-- TODO move to pgtap

ALTER TABLE edges
DROP COLUMN IF EXISTS node_count,
ADD COLUMN node_count INTEGER;

UPDATE edges AS edge_table
SET node_count=count.sum
FROM
        (SELECT
        start_vid,
        sum(node) AS sum
        FROM
            pgr_drivingDistance(
            'SELECT id, source, target, ST_Length(geom) AS cost FROM edges',
            ARRAY(SELECT DISTINCT source FROM edges),
            1,
            false)
    GROUP BY start_vid) AS count
    WHERE edge_table.source=count.start_vid
    ;

ALTER TABLE edges
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
    start_vid,
    sum(node) AS sum
    FROM
    pgr_drivingDistance(
        'SELECT id, source, target, cost FROM network',
        ARRAY(SELECT DISTINCT source FROM network),
        1,
        false)
    GROUP BY start_vid) AS count
WHERE network.source=count.start_vid
;

ALTER TABLE network
DROP COLUMN IF EXISTS another_column,
ADD COLUMN another_column INTEGER;


