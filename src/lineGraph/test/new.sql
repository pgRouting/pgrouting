CREATE TABLE edge_table15(
    id BIGINT,
    source BIGINT,
    target BIGINT,
    reverse BOOLEAN
);

INSERT INTO edge_table15(id, source, target, reverse) VALUES
(1, 1, 4, true),
(2, 4, 7, true),
(3, 4, 8, true),
(4, 4, 10, true),
(5, 2, 4, false),
(6, 2, 1, false),
(7, 5, 8, false),
(8, 5, 9, false),
(9, 5, 11, false),
(10, 3, 2, false),
(11, 3, 5, false),
(12, 16, 9, true),
(13, 16, 15, true),
(14, 8, 9, true),
(15, 8, 10, false),
(16, 10, 12, false),
(17, 10, 14, true),
(18, 7, 6, true),
(19, 8, 7, true),
(20, 9, 15, true),
(21, 11, 13, false),
(22, 7, 11, true),
(23, 9, 10, false),
(24, 12, 13, false),
(25, 16, 3, false),
(26, 8, 11, false),
(27, 10, 7, true),
(28, 9, 11, false);


WITH the_unique_vertices_of_the_graph AS (
(SELECT DISTINCT source AS id from edge_table15)
UNION
(SELECT DISTINCT source from edge_table15))
SELECT * INTO edge_table15_vertices_pgr
FROM the_unique_vertices_of_the_graph;
