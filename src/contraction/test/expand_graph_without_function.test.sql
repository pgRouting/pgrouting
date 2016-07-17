BEGIN;

SELECT 'step 1: Initial edge table';

SELECT id, source, target, cost, reverse_cost FROM edge_table;

SELECT 'step 2: Initial vertex table';

SELECT id FROM edge_table_vertices_pgr;


-- add extra columns to the edges and vertices table
SELECT 'step 3: Adding is_contracted column to edge_table......';

ALTER TABLE edge_table ADD is_contracted BOOLEAN DEFAULT false;

SELECT 'step 4: Adding contracted_vertices column to edge_table.....';

ALTER TABLE edge_table ADD contracted_vertices integer[];

SELECT 'step 5: Adding is_contracted column to edge_table......';

ALTER TABLE edge_table_vertices_pgr ADD is_contracted BOOLEAN DEFAULT false;

SELECT 'step 6: Adding contracted_vertices column to edge_table.....';

ALTER TABLE edge_table_vertices_pgr ADD contracted_vertices integer[];

SELECT 'step 7: Edge table after adding columns';

SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table;

SELECT 'step 8: Vertex table after adding columns';

SELECT id, is_contracted, contracted_vertices FROM edge_table_vertices_pgr;


/*
-- Testing the update function
   SELECT * FROM pgr_update_contraction_columns(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[0, 1]::integer[], 1, true );

SELECT  'step 9: Creating the edge table and vertex table of the contracted graph';

CREATE TABLE contracted_edge_table_vertices_pgr AS
SELECT *
FROM edge_table_vertices_pgr WHERE is_contracted = false;

CREATE TABLE contracted_edge_table AS
SELECT *
FROM edge_table WHERE source IN (SELECT id FROM contracted_edge_table_vertices_pgr)
AND target IN (SELECT id FROM contracted_edge_table_vertices_pgr);

SELECT 'step 10: Edge table after modification';

SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table ORDER BY id;

SELECT 'step 11: Vertex table after modification';

SELECT id, is_contracted, contracted_vertices FROM edge_table_vertices_pgr  ORDER BY id;

SELECT 'step 12: Edge table representing the contracted graph';

SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM contracted_edge_table  ORDER BY id;

SELECT 'step 13: Vertex table representing the contracted graph';

SELECT id, is_contracted, contracted_vertices FROM contracted_edge_table_vertices_pgr  ORDER BY id;

SELECT 'step 14: Expanding the contracted graph......';

-- Testing the expand function
   SELECT * FROM pgr_expand_contracted_graph(
    'edge_table', 'edge_table_vertices_pgr',
    'contracted_edge_table', 'contracted_edge_table_vertices_pgr');

SELECT 'step 15: Edge table representing the expanded graph';

SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM contracted_edge_table  ORDER BY id;

SELECT 'step 16: Vertex table representing the expanded graph';

SELECT id, is_contracted, contracted_vertices FROM contracted_edge_table_vertices_pgr  ORDER BY id;
*/
ROLLBACK;