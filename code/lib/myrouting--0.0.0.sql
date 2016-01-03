

CREATE OR REPLACE FUNCTION my_dijkstra(edges_sql text, start_vid bigint, end_vid bigint,has_rcost boolean)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'shortest_path'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION contract_graph(edges_sql text, level bigint,has_rcost boolean)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'contract_graph'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION my_astar(edges_sql text, start_vid bigint, end_vid bigint,has_rcost boolean)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'shortest_path_astar'
    LANGUAGE c IMMUTABLE STRICT;
