
CREATE OR REPLACE FUNCTION contract_graph(edges_sql text, level bigint)
  RETURNS SETOF RECORD AS
 '$libdir/libmyrouting_library', 'contract_graph'
    LANGUAGE c IMMUTABLE STRICT;
