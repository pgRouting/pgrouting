 CREATE TYPE pgr_contracted_blob AS
 (
 	my_contracted_graph_name text,
 	contracted_blob text
 );

CREATE OR REPLACE FUNCTION my_dijkstra(edges_sql text, start_vid bigint, end_vid bigint,has_rcost boolean)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'shortest_path'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION my_astar(edges_sql text, start_vid bigint, end_vid bigint,has_rcost boolean)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'shortest_path_astar'
    LANGUAGE c IMMUTABLE STRICT;
    
CREATE OR REPLACE FUNCTION pgr_contractgraph(query text,level bigint)
RETURNS SETOF pgr_contracted_blob AS 
'$libdir/libmyrouting_library', 'pgr_contractgraph' 
LANGUAGE c IMMUTABLE STRICT;