
DROP TYPE IF EXISTS pgr_contracted_blob;
 CREATE TYPE pgr_contracted_blob AS
 (
 	contracted_graph_name text,
 	contracted_graph_blob text,
 	removedVertices text,
 	removedEdges text,
 	psuedoEdges text
 );
 
CREATE OR REPLACE FUNCTION pgr_contractgraph(query text,level bigint,
	has_rcost boolean)
RETURNS SETOF pgr_contracted_blob AS 
'$libdir/${PGROUTING_LIBRARY_NAME}', 'pgr_contractgraph' 
LANGUAGE c IMMUTABLE STRICT;
