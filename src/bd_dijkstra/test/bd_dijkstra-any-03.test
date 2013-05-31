select (r).seq, (r).id2, (r).id2, round((r).cost::numeric, 6) from ( select pgr_bddijkstra('select * from bdd_ways'::text, 9426, 3606, true, true) as r ) as foo;
