DROP TABLE IF EXISTS edge_table;
CREATE TABLE edge_table (
    id SERIAL,
    source INTEGER,
    target INTEGER,
    cost FLOAT
);

\echo 'Examples of dead end contraction for directed graphs'

INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 7);

SELECT * FROM edge_table;
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table ',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);

DELETE FROM edge_table;

INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 7);
INSERT INTO edge_table (source, target, cost) VALUES (3, 1, 7);
INSERT INTO edge_table (source, target, cost) VALUES (4, 1, 7);

SELECT * FROM edge_table;
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table ',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true);

DELETE FROM edge_table;

INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 7);
INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 3);
INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 4);

SELECT * FROM edge_table;
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table ',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, false);

DELETE FROM edge_table;

\echo 'Examples of dead end contraction for undirected graphs'

INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 7);
INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 3);
INSERT INTO edge_table (source, target, cost) VALUES (2, 1, 4);
INSERT INTO edge_table (source, target, cost) VALUES (2, 3, 1);
INSERT INTO edge_table (source, target, cost) VALUES (2, 4, 2);

SELECT * FROM edge_table;
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost FROM edge_table ',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, false);

DELETE FROM edge_table;

