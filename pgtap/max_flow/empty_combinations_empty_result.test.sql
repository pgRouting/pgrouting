
\i setup.sql

SELECT plan(7);

CREATE OR REPLACE FUNCTION foo( TestFunction TEXT, sql_EdgesQuery TEXT )
RETURNS SETOF TEXT AS
$BODY$
BEGIN
    IF NOT min_version('3.2.0') THEN
      RETURN QUERY
      SELECT skip(1, 'Combinations functionality is new on 3.2.0');
      RETURN;
    END IF;


    IF TestFunction = 'pgr_maxFlow' THEN
        RETURN query SELECT set_eq( 'SELECT * FROM ' || TestFunction || '( $$' || sql_EdgesQuery || '$$,
            $$SELECT * FROM combinations_table WHERE source IN (-1)$$ ) ',
            'SELECT NULL::BIGINT'
        );
    ELSIF TestFunction = 'pgr_maxFlowMinCost_Cost' THEN
        RETURN query SELECT set_eq( 'SELECT * FROM ' || TestFunction || '( $$' || sql_EdgesQuery || '$$,
            $$SELECT * FROM combinations_table WHERE source IN (-1)$$ ) ',
            'SELECT NULL::FLOAT'
        );
    ELSE
        RETURN query SELECT is_empty( 'SELECT * FROM ' || TestFunction || '( $$' || sql_EdgesQuery || '$$,
            $$SELECT * FROM combinations_table WHERE source IN (-1)$$ ) '
        );
    END IF;
    RETURN;
END
$BODY$
language plpgsql;

-- test pgr_maxFlow
SELECT * FROM foo(
    'pgr_maxFlow',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
);

-- test pgr_boykovKolmogorov
SELECT * FROM foo(
    'pgr_boykovKolmogorov',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
);

-- test pgr_edmondsKarp
SELECT * FROM foo(
    'pgr_edmondsKarp',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
);

-- test pgr_pushRelabel
SELECT * FROM foo(
    'pgr_pushRelabel',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
);

-- test pgr_edgeDisjointPaths
SELECT * FROM foo(
    'pgr_edgeDisjointPaths',
    'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);

-- test pgr_maxFlowMinCost
SELECT * FROM foo(
    'pgr_maxFlowMinCost',
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table'
);

-- test pgr_maxFlowMinCost_Cost
SELECT * FROM foo(
    'pgr_maxFlowMinCost_Cost',
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table'
);

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
