BEGIN;
-- add extra columns to the edges and vertices table
SELECT 'Adding is_contracted column to edge_table......';
ALTER TABLE edge_table ADD is_contracted BOOLEAN DEFAULT false;
SELECT 'Adding contracted_vertices column to edge_table.....';
ALTER TABLE edge_table ADD contracted_vertices integer[];
SELECT 'Adding is_contracted column to edge_table......';
ALTER TABLE vertex_table ADD is_contracted BOOLEAN DEFAULT false;
SELECT 'Adding contracted_vertices column to edge_table.....';
ALTER TABLE vertex_table ADD contracted_vertices integer[];
-- SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table;
-- SELECT id, is_contracted, contracted_vertices FROM vertex_table;

-- update the added columns to the above tables 
-- based on the results of contraction query
CREATE OR REPLACE FUNCTION pgr_create_contracted_graph(
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
contracted_vertices_string text;
query text;
update_query text;
split_query text;
contracted_vertices_array integer[];
row record;
split_row record;
BEGIN
total := 0;
query := 'SELECT * FROM pgr_contractGraph('|| quote_literal(edge_table)||', ' || quote_literal(forbidden_vertices)||', ' || quote_literal(contraction_order)||', ' || quote_literal(max_cycles)||', ' || quote_literal(directed) || ')';
    -- execute 'SELECT * FROM pgr_contractGraph('|| quote_literal(edge_table)||', ' || quote_literal(forbidden_vertices)||', ' || quote_literal(contraction_order)||', ' || quote_literal(max_cycles)||', ' || quote_literal(directed) || ')'  into contracted_graph;
    -- SELECT count(*) into total FROM contracted_graph;
    FOR row IN EXECUTE(query)
    LOOP
        EXECUTE 'SELECT TRIM(LEADING ' || quote_literal('{') || ' FROM ' || quote_literal(row.contracted_vertices) || ')' INTO contracted_vertices_string;
        raise notice 'cvs: %', contracted_vertices_string;
        EXECUTE 'SELECT TRIM(TRAILING ' || quote_literal('}') || ' FROM ' || quote_literal(contracted_vertices_string) || ')' INTO contracted_vertices_string;
        raise notice 'cvs: %', contracted_vertices_string;
        EXECUTE 'SELECT TRIM(TRAILING ' || quote_literal(', ') || ' FROM ' || quote_literal(contracted_vertices_string) || ')' INTO contracted_vertices_string;
        raise notice 'cvs: %', contracted_vertices_string;
        split_query := 'select regexp_split_to_array('|| quote_literal(contracted_vertices_string) || ', ' || quote_literal(',')|| ')::integer[] AS array';
        
        EXECUTE split_query INTO split_row;
        contracted_vertices_array := split_row.array;
        -- raise notice 'arr[1]: %', contracted_vertices_array[1];
        IF row.type = 'v' THEN
            total := total + 1;
            raise notice 'vertex';
            update_query := 'UPDATE vertex_table SET is_contracted = true, contracted_vertices = array_append(contracted_vertices, ';  
            FOREACH contracted_vertex IN ARRAY contracted_vertices_array
            LOOP
                EXECUTE update_query || quote_literal(contracted_vertex) || ') WHERE id = ' || quote_literal(row.id);
                raise notice 'cv: %', contracted_vertex;
            END LOOP;
        ELSEIF row.type = 'e' THEN
            total := total + 1;
            raise notice 'edge';
        raise notice 'id: %, type: %, cv: %',row.id, row.type, row.contracted_vertices;
        END IF;
    END LOOP;

   RETURN total;
   END;
   $total$ LANGUAGE plpgsql;




   SELECT * FROM pgr_create_contracted_graph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 1',
    ARRAY[]::BIGINT[], ARRAY[0]::integer[], 1, true );


SELECT id, source, target, cost, reverse_cost, is_contracted, contracted_vertices FROM edge_table;
SELECT id, is_contracted, contracted_vertices FROM vertex_table;

   ROLLBACK;