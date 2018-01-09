\set QUIET 1

SET client_min_messages = WARNING;


DROP TABLE IF EXISTS edge_table;

--EDGE TABLE CREATE
CREATE TABLE edge_table (
    id BIGSERIAL,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT
);

--EDGE TABLE ADD DATA
INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 1, 1, 2, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 2, 2, 3, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 3, 3, 4, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 4, 6, 5, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 5, 7, 5, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 6, 8, 5, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 7, 8, 7, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 8, 7, 6, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 9, 2, 10, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 10, 10, 9, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 11, 9, 1, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 12, 3, 11, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 13, 11, 10, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 14, 4, 12, 1 , -1);

INSERT INTO edge_table (id, source, target, cost,reverse_cost)
VALUES ( 15, 11, 12, 1 , -1);