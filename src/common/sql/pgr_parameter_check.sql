/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

-----------------------------------------------------------------------
-- Function _pgr_parameter_check
-- Check's the parameters type of the sql input
-----------------------------------------------------------------------

-- change the default to true when all the functions will use the bigint
CREATE OR REPLACE FUNCTION _pgr_parameter_check(fn text, sql text, big boolean default false)
  RETURNS bool AS
  $BODY$  

  DECLARE
  rec record;
  rec1 record;
  has_rcost boolean;
  safesql text;
  BEGIN 
    -- checking query is executable
    BEGIN
      safesql =  'select * from ('||sql||' ) AS __a__ limit 1';
      execute safesql into rec;
      EXCEPTION
        WHEN OTHERS THEN
            RAISE EXCEPTION 'Could not execute query please verify syntax of: '
              USING HINT = sql;
    END;

    -- checking the fixed columns and data types of the integers
    IF fn IN ('driving', 'dijkstra', 'ksp', 'warshall') THEN
      BEGIN
        execute 'select id,source,target,cost  from ('||safesql||') as __b__' into rec;
        EXCEPTION
          WHEN OTHERS THEN
              RAISE EXCEPTION 'An expected column was not found in the query'
                USING HINT = 'Please veryfy the column names: id, source, target, cost';
      END;
    END IF;
 
    IF fn IN ('driving', 'dijkstra', 'ksp', 'warshall') THEN
      execute 'select pg_typeof(id)::text as id_type, pg_typeof(source)::text as source_type, pg_typeof(target)::text as target_type, pg_typeof(cost)::text as cost_type'
            || ' from ('||safesql||') AS __b__ ' into rec;
      if (big) then
        if not (rec.id_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
           OR   not (rec.source_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
           OR   not (rec.target_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
           OR   not (rec.cost_type in ('bigint'::text, 'integer'::text, 'smallint'::text, 'double precision'::text, 'real'::text)) then
           RAISE EXCEPTION 'Illegar type found in query.';
        end if;
      else -- Version 2.0.0 is more restrictive
        if not(   (rec.id_type in ('integer'::text))
              and (rec.source_type in ('integer'::text))
              and (rec.target_type in ('integer'::text))
              and (rec.cost_type = 'double precision'::text)) then
            RAISE EXCEPTION 'Support for id,source,target columns only of type: integer. Support for Cost: double precision';
        end if;
      end if;
    END IF;


    -- Checking the data types of the optional reverse_cost";
    has_rcost := false;
    IF fn IN ('driving', 'dijkstra', 'ksp', 'warshall') THEN
      BEGIN
        execute 'select reverse_cost, pg_typeof(reverse_cost)::text as rev_type  from ('||safesql||' ) AS __b__ limit 1 ' into rec1;
        has_rcost := true;
        EXCEPTION
          WHEN OTHERS THEN
            has_rcost = false;
            return has_rcost;  
      END;
      if (has_rcost) then
        IF (big) then
           IF  not (rec1.rev_type in ('bigint'::text, 'integer'::text, 'smallint'::text, 'double precision'::text, 'real'::text)) then
             RAISE EXCEPTION 'Illegar type in optional parameter reverse_cost.';
           END IF;
        ELSE -- Version 2.0.0 is more restrictive
           IF (rec1.rev_type != 'double precision') then
             RAISE EXCEPTION 'Illegal type in optimal parameter reverse_cost, expected: double precision';
           END IF;
        END IF;
      end if;
      return true;
    END IF;
    -- just for keeps
    return true;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 1;


