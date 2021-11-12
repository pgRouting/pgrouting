BEGIN;

-- This test is prepared for when:
-- Tentatively trsp will accepts ANY-INTEGER and ANY-NUMERICAL on 4.0.0
-- Adjust the version when that happens



UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('4.0.0') THEN plan(648) ELSE plan(324) END;

CREATE OR REPLACE FUNCTION inner_query()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  restriction TEXT;
BEGIN

  restriction := '$$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, ''32, 33''::TEXT AS via_path$$)';

  IF min_version('4.0.0') THEN
    -- Basically this is a TODO
    -- but has more fails than ok

    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, false, true)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, false, true, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, false, true, ' || restriction);

    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, true, true)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, true, true, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, true, true, ' || restriction);

    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, false, false)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, false, false, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, false, false, ' || restriction);

    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, true, false)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, true, false, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trsp', ', 2, 9, true, false, ' || restriction);

  ELSE

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, false, true)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, false, true, NULL)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, false, true, ' || restriction, TRUE);

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, true, true)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, true, true, NULL)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, true, true, ' || restriction, TRUE);

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, false, false)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, false, false, NULL)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, false, false, ' || restriction, FALSE);

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, true, false)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, true, false, NULL)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trsp', ', 2, 9, true, false, ' || restriction, FALSE);

  END IF;
END;
$BODY$
LANGUAGE plpgsql;

SELECT inner_query();


SELECT finish();
ROLLBACK;
