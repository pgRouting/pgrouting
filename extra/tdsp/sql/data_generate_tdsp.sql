/*drop table time_dep_costs;

create table time_dep_costs
(
	edge_id integer,
	start_time integer,
	end_time integer,
	travel_time double precision
);

drop table time_dep_costs_random;
create table time_dep_costs_random
(
	edge_id integer,
	start_time double precision,
	end_time double precision,
	travel_time double precision
);


DROP FUNCTION generate_time_dep_data();
 create function generate_time_dep_data() returns integer as $$
        declare 
        way ways%rowtype;
        BEGIN

       FOR way IN SELECT gid,class_id,length from ways
       LOOP
        IF way.class_id in (101,111,102) THEN
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 0, 6, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 6, 7, (1.10 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 7, 8, (1.25 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 8, 9, (1.55 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 9, 10, (1.45 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 10, 11, (1.15 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 11, 17, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 17, 18, (1.15 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 18, 19, (1.30 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 19, 20, (1.55 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 20, 21, (1.45 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 21, 22, (1.10 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 22, 24, (1 * way.length));
        END IF;

        IF way.class_id in (106,108,109,110,100) THEN
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 0, 6, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 6, 7, (1.05 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 7, 8, (1.20 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 8, 9, (1.50 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 9, 10, (1.40 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 10, 11, (1.10 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 11, 17, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 17, 18, (1.10 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 18, 19, (1.25 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 19, 20, (1.50 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 20, 21, (1.40 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 21, 22, (1.05 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 22, 24, (1 * way.length));
        END
        IF;

        IF way.class_id in (112,119,117,114,122,401) THEN
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 0, 6, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 6, 7, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 7, 8, (1.10 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 8, 9, (1.15 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 9, 10, (1.15 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 10, 11, (1.05 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 11, 17, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 17, 18, (1.05 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 18, 19, (1.10 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 19, 20, (1.15 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 20, 21, (1.15 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 21, 22, (1 * way.length));
            INSERT INTO time_dep_costs(edge_id,start_time,end_time,travel_time) values (way.gid, 22, 24, (1 * way.length));
        END
        IF;

        END 
        LOOP;
        RETURN 1;
        END;
        $$ language plpgsql;

*/

--update time_dep_costs set travel_time = 100000 where edge_id in (select gid from ways where class_id = 106) and start_time = 19; 
--update time_dep_costs set travel_time = 100000 where edge_id in (select gid from ways where class_id = 106) and start_time = 9; 


DROP FUNCTION generate_random_time_dep_data();
create function generate_random_time_dep_data() returns integer as $$
declare 
way ways%rowtype;
temp record;
random_time float;
i integer;
BEGIN
	
	FOR way IN SELECT gid,class_id,length from ways
	LOOP
		FOR i IN 0..23 LOOP
		
			FOR temp in EXECUTE 'SELECT random()'
			LOOP
				random_time := temp.random;
			END LOOP;
			
			INSERT INTO time_dep_costs_random(edge_id,start_time,end_time,travel_time) values (way.gid, i,i+1,random_time);
			
		END LOOP;
		
	END LOOP;
	RETURN 1;
END;
$$ language plpgsql;
