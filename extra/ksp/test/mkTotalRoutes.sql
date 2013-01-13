--
-- Dump a 15 fastest routes between nodes 4 and 5 by total route length
--
select ksp.route_id,sum(st_length(ksp.the_geom))  from ksp_sp('select source,target,cost,reverse_cost,gid from network','network',4,5,15,'f') ksp  group by route_id order by route_id
