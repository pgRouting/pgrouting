-----------------------------------------------------------------------
-- Function _pgr_parameter_check
-- Check's the parameters type of the sql input

-- See README for description
-----------------------------------------------------------------------

-- change the default to true when all the functions will use the bigint
CREATE OR REPLACE FUNCTION _pgr_parameter_check(sql text, big boolean default false)
  RETURNS bool AS
  $BODY$  

  DECLARE
  rec record;
  rec1 record;
  has_reverse boolean;
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
    if (big) then
      if not (rec.id_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
         OR   not (rec.source_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
         OR   not (rec.target_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
         OR   not (rec.cost_type = 'double precision'::text) then
         RAISE EXCEPTION 'support for id,source,target columns only of type: BigInt, integer or smallint. Support for Cost: double precision';
      end if;
    else
      if not (rec.id_type in ('integer'::text, 'smallint'::text))
         OR   not (rec.source_type in ('integer'::text, 'smallint'::text))
         OR   not (rec.target_type in ('integer'::text, 'smallint'::text)) 
         OR   not (rec.cost_type = 'double precision'::text) then
          RAISE EXCEPTION 'support for id,source,target columns only of type: integer or smallint. Support for Cost: double precision';
      end if;
    end if;
    END;

    -- checking the data types of the optional reverse_cost
    has_reverse = false;
    BEGIN
      execute 'select reverse_cost  from ('||sql||' limit 1) AS a ' into rec1;
      has_reverse := true;
      EXCEPTION
         WHEN OTHERS THEN
            has_reverse = false;
      END;
      if (has_reverse) then
        execute 'select pg_typeof(reverse_cost)::text as reverse_type from ('||sql||' limit 1) AS a ' into rec1;
        if (rec1.reverse_type != 'double precision') then
          raise EXCEPTION 'Reverse_cost is not double precision';
        end if;
      end if;

    return has_reverse;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 1;


