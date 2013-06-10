update kdgrid set cost=-1.0 where  source=2 and target=3;
update kdgrid set cost=-1.0 where  source=12 and target=13;
update kdgrid set cost=-1.0 where  source=22 and target=23;
update kdgrid set cost=-1.0 where  source=32 and target=33;
update kdgrid set cost=-1.0 where  source=42 and target=43;
select * from pgr_kdijkstrapath('select gid as id, source, target, cost from kdgrid', 288, array[1,25,601,625], false, false);
