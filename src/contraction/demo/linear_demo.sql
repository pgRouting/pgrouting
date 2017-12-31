DROP TABLE IF EXISTS edge_table;
CREATE TABLE edge_table (
    id SERIAL,
    source INTEGER,
    target INTEGER,
    cost FLOAT
);

\echo 'Examples of linear contraction for directed graphs'

INSERT INTO edge_table (source, target, cost) VALUES (1, 2, 7);
INSERT INTO edge_table (source, target, cost) VALUES (2, 3, 4);

SELECT * FROM edge_table;
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table ',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);

DELETE FROM edge_table;

INSERT INTO edge_table (source, target, cost) VALUES (1, 2, 7);
INSERT INTO edge_table (source, target, cost) VALUES (2, 3, 4);
INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 7);
INSERT INTO edge_table (source, target, cost) VALUES (3, 2, 4);

SELECT * FROM edge_table;
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table ',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);

DELETE FROM edge_table;

INSERT INTO edge_table (source, target, cost) VALUES (1, 2, 7);
INSERT INTO edge_table (source, target, cost) VALUES (1, 2, 3);
INSERT INTO edge_table (source, target, cost) VALUES (2, 3, 4);
INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 7);
INSERT INTO edge_table (source, target, cost) VALUES (3, 2, 4);

SELECT * FROM edge_table;
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table ',
    ARRAY[]::BIGINT[], ARRAY[1]::integer[], 1, true);

DELETE FROM edge_table;