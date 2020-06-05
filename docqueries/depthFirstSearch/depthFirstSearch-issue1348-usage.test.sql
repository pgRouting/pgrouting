\echo -- SAMPLE TABLE CREATE start
CREATE TABLE sample_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);
\echo -- SAMPLE TABLE CREATE end

\echo -- SAMPLE TABLE ADD DATA start
INSERT INTO sample_table (source, target, cost, reverse_cost) VALUES
    (3, 6, 20, 15),
    (3, 8, 10, -10),
    (6, 8, -1, 12);
\echo -- SAMPLE TABLE ADD DATA end
