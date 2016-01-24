

CREATE OR REPLACE FUNCTION pgr_contracted_dijkstra(query text,start_vid bigint,
	end_vid bigint,has_rcost boolean)
RETURNS SETOF pathElement AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'shortest_path_c'
    LANGUAGE c IMMUTABLE STRICT;
