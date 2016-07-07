BEGIN;

SELECT 'initial edge table';
SELECT id, source, target, cost, reverse_cost FROM edge_table;
SELECT 'initial vertex table';
SELECT id FROM edge_table_vertices_pgr;



-- add extra columns to the edges and vertices table
SELECT 'Adding is_contracted column to edge_table......';
ALTER TABLE edge_table ADD is_contracted BOOLEAN DEFAULT false;
SELECT 'Adding contracted_vertices column to edge_table.....';
ALTER TABLE edge_table ADD contracted_vertices integer[];
SELECT 'Adding is_contracted column to edge_table......';
ALTER TABLE edge_table_vertices_pgr ADD is_contracted BOOLEAN DEFAULT false;
SELECT 'Adding contracted_vertices column to edge_table.....';
ALTER TABLE edge_table_vertices_pgr ADD contracted_vertices integer[];

SELECT 'Edge table after adding columns';
SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table;
SELECT 'Vertex table after adding columns';
SELECT id, is_contracted, contracted_vertices FROM edge_table_vertices_pgr;


-- update the added columns to the above tables 
-- based on the results of contraction query
CREATE OR REPLACE FUNCTION pgr_update_contraction_columns(
	edge_table text,
	forbidden_vertices BIGINT[],
	contraction_order integer[],
	max_cycles integer,
	directed BOOLEAN
)
RETURNS integer AS $total$
declare
total integer;
contracted_vertex integer;
query text;
update_query text;
split_query text;
insert_query text;
row record;
split_row record;
BEGIN
total := 0;
query := 'SELECT * FROM pgr_contractGraph('|| quote_literal(edge_table)||', ' || quote_literal(forbidden_vertices)||', ' || quote_literal(contraction_order)||', ' || quote_literal(max_cycles)||', ' || quote_literal(directed) || ')';
    FOR row IN EXECUTE(query)
    LOOP
        
        -- contracted_vertices_array := row.contracted_vertices;
        -- If it is a vertex we update the two columns of the vertex table 
        IF row.type = 'v' THEN
            total := total + 1;
            -- raise notice 'vertex';
            update_query := 'UPDATE edge_table_vertices_pgr SET contracted_vertices = array_append(contracted_vertices, ';  
            FOREACH contracted_vertex IN ARRAY row.contracted_vertices
            LOOP
                EXECUTE update_query || quote_literal(contracted_vertex) 
                || ') WHERE id = ' || quote_literal(row.id);
                EXECUTE 'UPDATE edge_table_vertices_pgr SET is_contracted = true WHERE id = '
                || quote_literal(contracted_vertex);
                -- raise notice 'cv: %', contracted_vertex;
            END LOOP;
        -- If it is an edge we insert a new entry to the edge table 
        ELSEIF row.type = 'e' THEN
            total := total + 1;
            -- raise notice 'edge';
            insert_query := 'INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES ('
            || quote_literal(row.id)|| ', '
            || quote_literal(row.source)|| ', '
            || quote_literal(row.target)|| ', '
            || quote_literal(row.cost)|| ', '
            || quote_literal(-1)|| ', '
            || quote_literal(true)|| ', '
            || quote_literal(row.contracted_vertices)
            || ')';
            EXECUTE insert_query;
            FOREACH contracted_vertex IN ARRAY row.contracted_vertices
            LOOP
            EXECUTE 'UPDATE edge_table_vertices_pgr SET is_contracted = true WHERE id = '
                || quote_literal(contracted_vertex);
            END LOOP;
        END IF;
        -- raise notice 'id: %, type: %, cv: %',row.id, row.type, row.contracted_vertices;
    END LOOP;

   RETURN total;
   END;
   $total$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION pgr_expand_contracted_graph(
    original_edge_table text,
    original_vertex_table text,
    contracted_edge_table text,
    contracted_vertex_table text
)
RETURNS integer AS $total$
declare
total integer;
contracted_vertex integer;
query text;
row record;
BEGIN
    total := 0;
    query := 'SELECT * FROM '
    || quote_ident(contracted_edge_table)
    ||' WHERE id < 0 ';
    FOR row IN EXECUTE(query)
    LOOP
        -- Inserting the contracted vertices into the vertex table
        FOREACH contracted_vertex IN ARRAY row.contracted_vertices
        LOOP
            EXECUTE 'INSERT INTO ' || quote_ident(contracted_vertex_table) || '(id, is_contracted)' ||' VALUES ('
            || quote_literal(contracted_vertex) || ', false);';   
        END LOOP;
        -- Adding the edges(id < 0) between these vertices to the edge table
        EXECUTE 'INSERT INTO ' 
        || quote_ident(contracted_edge_table) 
        || ' SELECT * FROM ' || quote_ident(original_edge_table)
        || ' WHERE source = ANY(' || quote_literal(row.contracted_vertices) || ')'
        || ' OR target = ANY(' || quote_literal(row.contracted_vertices) || ')'
        || ' AND id > 0';

    END LOOP;

    -- Removing all those edges with id < 0
    EXECUTE 'DELETE FROM ' 
    || quote_ident(contracted_edge_table)
    || ' WHERE id < 0';

    query := 'SELECT * FROM '|| quote_ident(contracted_vertex_table)||' WHERE array_length(contracted_vertices, 1) > 0';
    FOR row IN EXECUTE(query)
    LOOP
        -- Inserting the contracted vertices into the vertex table
        FOREACH contracted_vertex IN ARRAY row.contracted_vertices
        LOOP
            EXECUTE 'INSERT INTO ' || quote_ident(contracted_vertex_table) || '(id, is_contracted)' ||' VALUES ('
            || quote_literal(contracted_vertex) || ', false);';    
        END LOOP;
        -- Adding the edges(id < 0) between these vertices to the edge table
        EXECUTE 'INSERT INTO ' 
        || quote_ident(contracted_edge_table) 
        || ' SELECT * FROM ' || quote_ident(original_edge_table)
        || ' WHERE source = ANY(' || quote_literal(row.contracted_vertices) || ')'
        || ' OR target = ANY(' || quote_literal(row.contracted_vertices) || ')'
        || ' AND id > 0';
    EXECUTE 'UPDATE '
    || quote_ident(contracted_vertex_table)
    || ' SET contracted_vertices = NULL' 
    || ' WHERE array_length(contracted_vertices, 1) > 0 ';
    END LOOP;

RETURN total;
END;
   $total$ LANGUAGE plpgsql;


-- Testing the update function
   SELECT * FROM pgr_update_contraction_columns(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::BIGINT[], ARRAY[0, 1]::integer[], 1, true );

SELECT  'Creating the edge table and vertex table of the contracted graph';

CREATE TABLE contracted_edge_table_vertices_pgr AS
SELECT *
FROM edge_table_vertices_pgr WHERE is_contracted = false;

CREATE TABLE contracted_edge_table AS
SELECT *
FROM edge_table WHERE source IN (SELECT id FROM contracted_edge_table_vertices_pgr)
AND target IN (SELECT id FROM contracted_edge_table_vertices_pgr);

SELECT 'edge table after modification';
SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table ORDER BY id;
SELECT 'vertex table after modification';
SELECT id, is_contracted, contracted_vertices FROM edge_table_vertices_pgr  ORDER BY id;

SELECT 'edge table representing the contracted graph';
SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM contracted_edge_table  ORDER BY id;
SELECT 'vertex table representing the contracted graph';
SELECT id, is_contracted, contracted_vertices FROM contracted_edge_table_vertices_pgr  ORDER BY id;


SELECT 'Expanding the contracted graph......';

-- Testing the expand function
   SELECT * FROM pgr_expand_contracted_graph(
    'edge_table', 'edge_table_vertices_pgr',
    'contracted_edge_table', 'contracted_edge_table_vertices_pgr');


SELECT 'Contracted graph after expansion';
SELECT 'edge table representing the expanded graph';
SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM contracted_edge_table  ORDER BY id;
SELECT 'vertex table representing the expanded graph';
SELECT id, is_contracted, contracted_vertices FROM contracted_edge_table_vertices_pgr  ORDER BY id;



ROLLBACK;