Create Table edge_table25(
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO edge_table25(id, source, target, cost, reverse_cost) VALUES
(1, 1, 2, 1.0, 1.0),
(2, 3, 2, 1.0, -1.0),
(3, 4, 3, 1.0, -1.0),
(4, 2, 5, 1.0, 1.0),
(5, 3, 6, 1.0, -1.0),
(6, 7, 8, 1.0, 1.0),
(7, 8, 5, 1.0, 1.0),
(8, 5, 6, 1.0, 1.0),
(9, 6, 9, 1.0, 1.0),
(10, 6, 11, 1.0, -1.0),
(11, 5, 10, 1.0, 1.0),
(12, 10, 11, 1.0, -1.0),
(13, 11, 12, 1.0, -1.0),
(14, 10, 13, 1.0, 1.0),
(15, 9, 12, 1.0, 1.0),
(16, 4, 9, 1.0, 1.0),
(17, 14, 15, 1.0, 1.0),
(18, 16, 17, 1.0, 1.0);

WITH the_unique_vertices_of_the_graph AS (
(SELECT DISTINCT source AS id from edge_table25)
UNION
(SELECT DISTINCT source from edge_table25))
SELECT * INTO edge_table25_vertices_pgr
FROM the_unique_vertices_of_the_graph;
