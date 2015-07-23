select (r).seq, (r).id1, (r).id2, round((r).cost::numeric, 6) from ( select pgr_bdastar('select * from bdaways'::text, 3606, 9426, true, true) as r ) as foo;
