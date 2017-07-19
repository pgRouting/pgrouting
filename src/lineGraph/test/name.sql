CREATE TYPE vertex_edge AS ENUM ('vertex', 'edge');

Create Table line_graph_table(
    id BIGINT,
    type vertex_edge,
    source BIGINT,
    target BIGINT,
    cost FLOAT
);
