/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli@gmail.com

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

--INTERNAL FUNCTIONS
CREATE OR REPLACE FUNCTION _pgr_executehelper(
    IN edges_sql TEXT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
	r RECORD;
  BEGIN
	FOR r in EXECUTE $1 LOOP
		RETURN NEXT r;
	END LOOP;
	RETURN;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;


--FUNCTIONS
CREATE OR REPLACE FUNCTION pgr_splitedges(
    IN edges_sql TEXT
    )
  RETURNS TABLE (id bigint, source bigint, target bigint, cost double precision) AS
  $BODY$
  DECLARE
	counter BIGINT;
	r RECORD;
  BEGIN
	counter := 1;
	FOR r in SELECT * FROM _pgr_executehelper(_pgr_get_statement($1)) AS (id bigint, source bigint, target bigint, cost double precision, reverse_cost double precision) LOOP
		IF r.cost > 0 THEN
			id:=counter;
			source=r.source;
			target=r.target;
			cost:=r.cost;
			counter:=counter+1;
			RETURN NEXT;
		END IF;
		IF r.reverse_cost > 0 THEN
			id:=counter;
			source=r.target;
			target=r.source;
			cost = r.reverse_cost;
			counter:=counter+1;
			RETURN NEXT;
		END IF;
	END LOOP;
	RETURN;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;
