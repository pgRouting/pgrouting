select (r).seq, (r).id1, (r).id2, round((r).cost::numeric, 6) from ( select pgr_bddijkstra('select * from bdd_ways'::text, 5700, 6733, true, true) as r ) as foo;
