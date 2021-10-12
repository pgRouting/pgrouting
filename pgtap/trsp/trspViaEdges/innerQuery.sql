BEGIN;

-- This test is prepared for when:
-- Tentatively trsp will accepts ANY-INTEGER and ANY-NUMERICAL on 4.0.0
-- Adjust the version when that happens

\i innerQuery_old.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('4.0.0') THEN plan(162) ELSE plan(324) END;

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

    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true, ' || restriction);

    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, true)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, true, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, true, ' || restriction);

    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, false)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, false, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, false, ' || restriction);

    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, false)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, false, NULL)');
    RETURN QUERY SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, false, ' || restriction);

  ELSE

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true, NULL)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true, ' || restriction, TRUE);

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, true)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, true, NULL)', TRUE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, true, ' || restriction, TRUE);

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, false)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, false, NULL)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, false, ' || restriction, FALSE);

    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, false)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, false, NULL)', FALSE);
    RETURN QUERY SELECT style_dijkstra_trsp('pgr_trspViaEdges', ', ARRAY[4,9,15]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], false, false, ' || restriction, FALSE);

  END IF;

END;
$BODY$
LANGUAGE plpgsql;

SELECT inner_query();

SELECT finish();
ROLLBACK;
