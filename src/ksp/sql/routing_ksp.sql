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
CREATE TYPE pgr_costResult3Big AS
(
    seq integer,
    id1 integer,
    id2 bigint,
    id3 bigint,
    cost float8
);


CREATE OR REPLACE FUNCTION _pgr_ksp(sql text, source_id bigint, target_id bigint, no_paths integer, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult3Big
    AS '$libdir/librouting_ksp', 'kshortest_path'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id bigint, target_id bigint, no_paths integer, has_reverse_cost boolean, heap_paths boolean default false)
  RETURNS SETOF pgr_costresult3Big AS
  $BODY$  

  DECLARE
  rec record;

  BEGIN 
    BEGIN
      execute 'select id,source,target,cost  from ('||sql||' limit 1) AS a ' into rec;
      EXCEPTION
        WHEN OTHERS THEN
            RAISE EXCEPTION 'An expected column was not found in the query'
              USING HINT = 'Please veryfy the column names: id, source, target, cost';
    END;
    
    IF (has_reverse_cost) THEN
      BEGIN
        execute 'select reverse_cost  from ('||sql||' limit 1) AS a ' into rec;
        EXCEPTION
          WHEN OTHERS THEN
            RAISE EXCEPTION 'An expected column was not found in the query'
              USING HINT = 'Please veryfy the column name: reverse_cost';
      END;
    END IF;
  


    execute 'select pg_typeof(id)::text as id_type, pg_typeof(source)::text as source_type, pg_typeof(target)::text as target_type '
            || ' from ('||sql||' limit 1) AS a ' into rec;
    if not (rec.id_type in ('integer'::text, 'smallint'::text)) then
        RAISE EXCEPTION 'id is NOT of type integer or smallint';
    end if;
    if not (rec.source_type in ('integer'::text, 'smallint'::text)) then
        RAISE EXCEPTION 'id is NOT of type integer or smallint';
    end if;
    if not (rec.target_type in ('integer'::text, 'smallint'::text)) then
        RAISE EXCEPTION 'id is NOT of type integer or smallint';
    end if;
        
       
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

