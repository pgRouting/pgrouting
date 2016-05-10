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
  RETURNS SETOF pathElement AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'shortest_path_Astar'
    LANGUAGE c IMMUTABLE STRICT;
