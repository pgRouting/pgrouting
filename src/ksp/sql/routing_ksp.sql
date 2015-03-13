-----------------------------------------------------------------------
-- Function for k shortest_path computation
-- See README for description
-----------------------------------------------------------------------
/* original query   (also Yen*.cpp has to be modified)
CREATE OR REPLACE FUNCTION _pgr_ksp(sql text, source_id integer, target_id integer, no_paths integer, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult3
    AS '$libdir/librouting_ksp', 'kshortest_path'
    LANGUAGE c IMMUTABLE STRICT;
*/

CREATE OR REPLACE FUNCTION _pgr_ksp(sql text, source_id integer, target_id integer, no_paths integer, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult3
    AS '$libdir/librouting_ksp', 'kshortest_path'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id integer, target_id integer, no_paths integer, has_reverse_cost boolean, heap_paths boolean default false)
  RETURNS SETOF pgr_costresult3 AS
  $BODY$ begin 
    if $6 = false then
       return query SELECT * FROM _pgr_ksp($1, $2, $3, $4, $5) where id1 < $4; 
    else
       return query SELECT * FROM _pgr_ksp($1, $2, $3, $4, $5); 
    end if;
  end 
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

