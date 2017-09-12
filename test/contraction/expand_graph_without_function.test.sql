
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

