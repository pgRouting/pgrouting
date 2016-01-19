
CREATE OR REPLACE FUNCTION my_dijkstra(edges_sql text, start_vid bigint, end_vid bigint)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'shortest_path'
    LANGUAGE c IMMUTABLE STRICT;
