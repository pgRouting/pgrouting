
CREATE OR REPLACE FUNCTION my_astar(edges_sql text, start_vid bigint, end_vid bigint)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'shortest_path_astar'
    LANGUAGE c IMMUTABLE STRICT;
