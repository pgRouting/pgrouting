Create Table vertex_table(
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

INSERT INTO line_edge_table(id, source, target, cost, reverse_cost) VALUES
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
(18, 16, 17, 1.0, 1.0),

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
