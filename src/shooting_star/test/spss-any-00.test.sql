select * from pgr_shootingstar('select eid as id, source::int4, target::int4, case when cost<=0 then 999 else cost end as cost, case when reverse_cost<=0 then 999 else reverse_cost end as reverse_cost, x1, y1, x2, y2, to_cost, rule from edges1', 11, 6, true, true);

