select (r).seq, (r).id1, (r).id2, round((r).cost::numeric, 6) from ( select pgr_bddijkstra('select * from bdd_ways'::text, 6733, 5700, true, true) as r ) as foo;
