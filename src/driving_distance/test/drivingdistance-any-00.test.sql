-- each ring will increas by 4 because it is on a square grid
-- to find the start node number
-- select *, st_distance(st_makepoint(25,25), the_geom) from vertices_tmp where st_dwithin(st_makepoint(25,25), the_geom, 1.0) order by st_distance(st_makepoint(25,25), the_geom) limit 1;


--this are equivalent
select cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1.0::float8 as cost from ddnoded2', 1274, 10, false, false)
) as foo group by cost order by cost asc;

select tot_cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10, false)
) as foo group by tot_cost order by tot_cost asc;

--- this ones are equivalent
select tot_cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10, true)
) as foo group by tot_cost order by tot_cost asc;

select tot_cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10)
) as foo group by tot_cost order by tot_cost asc;
