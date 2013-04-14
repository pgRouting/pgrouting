--
-- Generate 15 shortest paths between nodes 4 and 5
--
-- List by links
--
select ksp.id, ksp.route_id,n.source,n.target ,st_length(ksp.the_geom) from ksp_sp('select source,target,cost,reverse_cost,gid from network','network',4,5,15,'f') ksp,network n where ksp.edge_id=n.gid;
