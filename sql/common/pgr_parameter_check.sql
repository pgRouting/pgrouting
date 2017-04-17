/*PGR-GNU*****************************************************************

File: pgr_parameter_check.sql

Copyright (c) 2014 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

------

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

********************************************************************PGR-GNU*/


/*
-----------------------------------------------------------------------
-- Function _pgr_parameter_check
-- Check's the parameters type of the sql input
-----------------------------------------------------------------------

-- change the default to true when all the functions will use the bigint
-- put TRUE when it uses BGINT
-- Query styles:
-- dijkstra (id, source, target, cost, [reverse_cost])
-- johnson (source, target, cost, [reverse_cost])
*/

CREATE OR REPLACE FUNCTION _pgr_parameter_check(fn text, sql text, big boolean default false)
  RETURNS bool AS
  $BODY$

  DECLARE
  rec record;
  rec1 record;
  has_rcost boolean;
  safesql text;
  BEGIN
    IF (big) THEN
       RAISE EXCEPTION 'This function is for old style functions';
    END IF;

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
    IF fn IN ('dijkstra','astar') THEN
        BEGIN
          execute 'select id,source,target,cost  from ('||safesql||') as __b__' into rec;
          EXCEPTION
            WHEN OTHERS THEN
                RAISE EXCEPTION 'An expected column was not found in the query'
                  USING ERRCODE = 'XX000',
                   HINT = 'Please veryfy the column names: id, source, target, cost';
        END;
        execute 'select pg_typeof(id)::text as id_type, pg_typeof(source)::text as source_type, pg_typeof(target)::text as target_type, pg_typeof(cost)::text as cost_type'
            || ' from ('||safesql||') AS __b__ ' into rec;
        -- Version 2.0.0 is more restrictive
        IF NOT(   (rec.id_type in ('integer'::text))
              AND (rec.source_type in ('integer'::text))
              AND (rec.target_type in ('integer'::text))
              AND (rec.cost_type = 'double precision'::text)) THEN
            RAISE EXCEPTION 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8'
            USING ERRCODE = 'XX000';
        END IF;
    END IF;


    IF fn IN ('astar') THEN
        BEGIN
          execute 'select x1,y1,x2,y2  from ('||safesql||') as __b__' into rec;
          EXCEPTION
            WHEN OTHERS THEN
                RAISE EXCEPTION 'An expected column was not found in the query'
                  USING ERRCODE = 'XX000',
                   HINT = 'Please veryfy the column names: x1,y1, x2,y2';
        END;
        execute 'select pg_typeof(x1)::text as x1_type, pg_typeof(y1)::text as y1_type, pg_typeof(x2)::text as x2_type, pg_typeof(y2)::text as y2_type'
            || ' from ('||safesql||') AS __b__ ' into rec;
        -- Version 2.0.0 is more restrictive
        IF NOT(   (rec.x1_type = 'double precision'::text)
              AND (rec.y1_type = 'double precision'::text)
              AND (rec.x2_type = 'double precision'::text)
              AND (rec.y2_type = 'double precision'::text)) THEN
            RAISE EXCEPTION 'Columns: x1, y1, x2, y2 must be of type float8'
            USING ERRCODE = 'XX000';
        END IF;
    END IF;

    -- checking the fixed columns and data types of the integers
    IF fn IN ('johnson') THEN
        BEGIN
          execute 'select source,target,cost  from ('||safesql||') as __b__' into rec;
          EXCEPTION
            WHEN OTHERS THEN
                RAISE EXCEPTION 'An expected column was not found in the query'
                  USING HINT = 'Please veryfy the column names: id, source, target, cost',
                         ERRCODE = 'XX000';
        END;

        execute 'select pg_typeof(source)::text as source_type, pg_typeof(target)::text as target_type, pg_typeof(cost)::text as cost_type'
            || ' from ('||safesql||') AS __b__ ' into rec;
        -- Version 2.0.0 is more restrictive
        IF NOT(   (rec.source_type in ('integer'::text))
              AND (rec.target_type in ('integer'::text))
              AND (rec.cost_type = 'double precision'::text)) THEN
            RAISE EXCEPTION 'Support for source,target columns only of type: integer. Support for Cost: double precision'
            USING ERRCODE = 'XX000';
        END IF;
    END IF;


    -- Checking the data types of the optional reverse_cost";
    has_rcost := false;
    IF fn IN ('johnson','dijkstra','astar') THEN
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
             RAISE EXCEPTION 'Illegar type in optional parameter reverse_cost.'
             USING ERRCODE = 'XX000';
           END IF;
        ELSE -- Version 2.0.0 is more restrictive
           IF (rec1.rev_type != 'double precision') then
             RAISE EXCEPTION 'Illegal type in optional parameter reverse_cost, must be of type float8'
             USING ERRCODE = 'XX000';
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


