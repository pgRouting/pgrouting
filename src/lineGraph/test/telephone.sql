CREATE TABLE edge_table12(
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO edge_table12(id, source, target, cost, reverse_cost) VALUES
(1, 1, 4, 1.0, 1.0),
(2, 4, 7, 1.0, 1.0),
(3, 4, 8, 1.0, 1.0),
(4, 4, 10, 1.0, 1.0),
(5, 2, 4, 1.0, -1.0),
(6, 2, 1, 1.0, -1.0),
(7, 5, 8, 1.0, -1.0),
(8, 5, 9, 1.0, -1.0),
(9, 5, 11, 1.0, -1.0),
(10, 3, 2, 1.0, -1.0),
(11, 3, 5, 1.0, -1.0),
(12, 16, 9, 1.0, 1.0),
(13, 16, 15, 1.0, 1.0),
(14, 8, 9, 1.0, 1.0),
(15, 8, 10, 1.0, -1.0),
(16, 10, 12, 1.0, -1.0),
(17, 10, 14, 1.0, 1.0),
(18, 7, 6, 1.0, 1.0),
(19, 8, 7, 1.0, 1.0),
(20, 9, 15, 1.0, 1.0),
(21, 11, 13, 1.0, -1.0),
(22, 7, 11, 1.0, 1.0),
(23, 9, 10, 1.0, -1.0),
(24, 12, 13, 1.0, -1.0),
(25, 16, 3, 1.0, -1.0),
(26, 8, 11, 1.0, -1.0),
(27, 10, 7, 1.0, 1.0),
(28, 9, 11, 1.0, -1.0);

WITH the_unique_vertices_of_the_graph AS (
(SELECT DISTINCT source AS id from edge_table12)
UNION
(SELECT DISTINCT source from edge_table12))
SELECT * INTO edge_table12_vertices_pgr
FROM the_unique_vertices_of_the_graph;
