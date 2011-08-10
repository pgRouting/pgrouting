--
-- Copyright (c) 2011 Jay Mahadeokar
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

drop type pgr_time_dep_costs cascade;

create type pgr_time_dep_costs as (
 edge_id int,
 start_time float,
 end_time float,
 travel_time float
);



CREATE OR REPLACE FUNCTION pgr_get_time_dep_costs
(	table_name text, 
	query_start_time int, 
	max_time_duration int
)
RETURNS SETOF pgr_time_dep_costs AS
$$
DECLARE
out_rec pgr_time_dep_costs;
temp record;
max_time float;
min_time float;
left_time float;
limit_time float;
cycles float;

BEGIN
	
	cycles := 0;
	
	FOR temp in EXECUTE 'SELECT max(end_time) as max_time FROM ' || quote_ident(table_name)
	LOOP
		limit_time := temp.max_time;
	END LOOP;
	
	left_time := max_time_duration;
	min_time := query_start_time;
	max_time := query_start_time + max_time_duration;
	
	-- This is to avoid repetition of data due to cycles
	IF left_time > limit_time THEN
		left_time := limit_time;
	END IF;
	
	IF max_time > limit_time  THEN
		max_time := limit_time;
	END IF;
	
	LOOP
			out_rec.edge_id := 0;
			out_rec.start_time := min_time;
			out_rec.end_time := max_time;
			out_rec.travel_time := left_time;
			RETURN NEXT out_rec;
	
		FOR temp IN EXECUTE 'SELECT edge_id, start_time, end_time, travel_time FROM ' || 
                     quote_ident(table_name) || ' where  (start_time >= ' || min_time ||
                     ' AND end_time < ' || max_time || ' ) or (start_time <= ' || min_time || ' and  end_time > ' ||
                     min_time || ' ) or (start_time < ' || max_time || ' and  end_time >= ' || max_time || ' )'
		LOOP
			out_rec.edge_id := temp.edge_id;
			IF (temp.start_time < min_time AND temp.end_time > min_time) THEN
				out_rec.start_time := min_time - query_start_time;
			ELSE
				out_rec.start_time := temp.start_time + (cycles * limit_time) - query_start_time;
			END IF;
			
			out_rec.end_time := temp.end_time + (cycles * limit_time) - query_start_time;
			out_rec.travel_time := temp.travel_time;
			RETURN NEXT out_rec;
	    END LOOP;
	    
	    left_time := left_time - (max_time - min_time);
	    cycles := cycles + 1;
	    
	    
	    IF left_time = 0 THEN
			EXIT;
		ELSE
			min_time := 0;
		END IF;
		
		IF left_time > (max_time - min_time) THEN
			max_time := limit_time;
		ELSE
			max_time := left_time;
		END IF;
		
			--out_rec.edge_id := 0;
			--out_rec.start_time := min_time;
			--out_rec.end_time := max_time;
			--out_rec.travel_time := left_time;
			--RETURN NEXT out_rec;
		
	END LOOP;
		 
   -- compute values of out from the input
   
	RETURN;  -- exit the procedure
END
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 
 
