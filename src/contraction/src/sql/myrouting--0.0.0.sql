	 
DROP TYPE IF EXISTS pgr_contracted_blob;
 CREATE TYPE pgr_contracted_blob AS
 (
 	contracted_graph_name text,
 	contracted_graph_blob text,
 	removedVertices text,
 	removedEdges text,
 	psuedoEdges text
 );

DROP TYPE IF EXISTS pathElement;
 CREATE TYPE pathElement AS
 (
 	seq integer,
    source integer,
    target integer,
    cost float8,
    tot_cost float8
 );

CREATE OR REPLACE FUNCTION my_astar(edges_sql text, start_vid bigint, end_vid bigint,has_rcost boolean)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'shortest_path_astar'
    LANGUAGE c IMMUTABLE STRICT;
    
CREATE OR REPLACE FUNCTION pgr_contractgraph(query text,level bigint,
	has_rcost boolean)
RETURNS SETOF pgr_contracted_blob AS 
'$libdir/libmyrouting_library', 'pgr_contractgraph' 
LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_contracted_dijkstra(query text,start_vid bigint,
	end_vid bigint,has_rcost boolean)
RETURNS SETOF pathElement AS
'$libdir/libmyrouting_library', 'shortest_path_c'
    LANGUAGE c IMMUTABLE STRICT;