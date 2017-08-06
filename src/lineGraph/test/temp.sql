Create Table edge_table20(
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT
);

INSERT INTO edge_table20(id, source, target, cost) VALUES
(1, 1, 2, 1.0),
(2, 2, 1, 1.0),
(3, 3, 2, 1.0),
(4, 4, 3, 1.0),
(5, 2, 5, 1.0),
(6, 5, 2, 1.0),
(7, 3, 6, 1.0),
(8, 7, 8, 1.0),
(9, 8, 7, 1.0),
(10, 8, 5, 1.0),
(11, 5, 8, 1.0),
(12, 5, 6, 1.0),
(13, 6, 5, 1.0),
(14, 6, 9, 1.0),
(15, 9, 6, 1.0),
(16, 6, 11, 1.0),
(17, 5, 10, 1.0),
(18, 10, 5, 1.0),
(19, 10, 11, 1.0),
(20, 11, 12, 1.0),
(21, 10, 13, 1.0),
(22, 13, 10, 1.0),
(23, 9, 12, 1.0),
(24, 12, 9, 1.0),
(25, 4, 9, 1.0),
(26, 9, 4, 1.0),
(27, 14, 15, 1.0),
(28, 15, 14, 1.0),
(29, 16, 17, 1.0),
(30, 17, 16, 1.0);

SELECT * FROM pgr_dijkstra(
    'SELECT id, source, target, cost FROM edge_table20',
    2, 3
);

UPDATE edge_table20 SET cost = cost + 0.001*id*id;

SELECT * FROM pgr_dijkstra(
    'SELECT id, source, target, cost FROM edge_table20',
    2, 3
);
