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

/* invert the comments when pgRouting decides for bigints */
CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id bigint, target_id bigint, no_paths integer, has_reverse_cost boolean default false, heap_paths boolean default false)
  --RETURNS SETOF pgr_costresult3Big AS
  RETURNS SETOF pgr_costresult3 AS
  $BODY$  

  DECLARE
  rec record;
  rec1 record;
  reverse boolean;
  BEGIN 
    -- checking query is executable
    BEGIN
      execute 'select * from ('||sql||' limit 1) AS a ';
      EXCEPTION
        WHEN OTHERS THEN
            RAISE EXCEPTION 'Could not excecute query please verify sintax of: '
              USING HINT = sql;
    END;

    -- checking the fixed columns and data types of the integers
    BEGIN
      execute 'select id,source,target,cost  from ('||sql||' limit 1) AS a ' into rec;
      EXCEPTION
        WHEN OTHERS THEN
            RAISE EXCEPTION 'An expected column was not found in the query'
              USING HINT = 'Please veryfy the column names: id, source, target, cost';
    END;
    
    BEGIN
    execute 'select pg_typeof(id)::text as id_type, pg_typeof(source)::text as source_type, pg_typeof(target)::text as target_type, pg_typeof(cost)::text as cost_type'
            || ' from ('||sql||' limit 1) AS a ' into rec;
    /*
    if not (rec.id_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
       OR   not (rec.source_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
       OR   not (rec.target_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
       OR   not (rec.cost_type = 'double precision'::text) then
        RAISE EXCEPTION 'support for id,source,target columns only of type: BigInt, integer or smallint. Support for Cost: double precision';
    end if;
    */

    if not (rec.id_type in ('integer'::text, 'smallint'::text))
       OR   not (rec.source_type in ('integer'::text, 'smallint'::text))
       OR   not (rec.target_type in ('integer'::text, 'smallint'::text)) 
       OR   not (rec.cost_type = 'double precision'::text) then
        RAISE EXCEPTION 'support for id,source,target columns only of type: integer or smallint. Support for Cost: double precision';
    end if;
    END;

    -- checking the data types of the optional reverse_cost
    --  execute 'select pg_typeof(reverse_cost) as reverse_type::text, reverse_cost  from ('||sql||' limit 1) AS a ' into rec1;
    if (has_reverse_Cost) then
         BEGIN
            execute 'select reverse_cost  from ('||sql||' limit 1) AS a ' into rec1;
            EXCEPTION
              WHEN OTHERS THEN
                 raise EXCEPTION 'has_reverse_cost set to true but reverse_cost not found';
         END;
         execute 'select pg_typeof(reverse_cost)::text as reverse_type from ('||sql||' limit 1) AS a ' into rec1;
         if (rec1.reverse_type != 'double precision') then
               raise EXCEPTION 'Reverse_cost is not double precision';
         end if;
    else
         BEGIN
            execute 'select reverse_cost  from ('||sql||' limit 1) AS a ' into rec1;
            raise NOTICE 'has_reverse_cost set to false. reverse_cost found, Ignoring reverse_cost';
            EXCEPTION
              WHEN OTHERS THEN  CONTINUE;
         END;
    end if;



    if $6 = false then
       return query SELECT seq,id1,id2::integer, id3::integer,cost FROM _pgr_ksp($1, $2, $3, $4, $5) where id1 < $4;
       -- return query SELECT * FROM _pgr_ksp($1, $2, $3, $4, $5) where id1 < $4;
    else
       return query SELECT seq,id1,id2::integer, id3::integer,cost FROM _pgr_ksp($1, $2, $3, $4, $5);
       -- return query SELECT * FROM _pgr_ksp($1, $2, $3, $4, $5);
    end if;
  end
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

