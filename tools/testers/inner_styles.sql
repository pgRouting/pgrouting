
CREATE OR REPLACE FUNCTION style_dijkstra(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF (fn NOT IN ('pgr_dagshortestpath(','pgr_topologicalsort(')) THEN
    --with reverse cost
    RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'id');
    RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'source');
    RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'target');
    RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'cost');
    RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'reverse_cost');
  END IF;

--without reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION innerquery_combinations(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target'],
    'source','combinations WHERE target NOT IN (5,6)');

  RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target'],
    'target','combinations WHERE target NOT IN (5,6)');
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION innerquery_points(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  -- with pid
  RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['pid', 'edge_id','fraction','side'],
    'pid','pointsofinterest');

  RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['pid', 'edge_id','fraction','side'],
    'edge_id','pointsofinterest');

  RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['pid', 'edge_id','fraction','side'],
    'fraction','pointsofinterest');

  -- withiout pid
  RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['edge_id','fraction','side'],
    'edge_id','pointsofinterest');

  RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['edge_id','fraction','side'],
    'fraction','pointsofinterest');
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION innerquery_restrictions(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

RETURN QUERY SELECT test_anyIntegerArr(fn, rest_sql,
    ARRAY['id', 'path', 'cost'],
    'path','restrictions');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'path', 'cost'],
    'cost','restrictions');
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION style_matrix(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'start_vid', 'matrix');

  RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'end_vid', 'matrix');

  RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'agg_cost', 'matrix');

END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION style_dijkstra_no_id(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'target');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'cost');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'target');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'cost');
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION style_astar(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'reverse_cost');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');


--without reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');

END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION style_max_flow(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity'],
    'target');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity'],
    'capacity');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity'],
    'reverse_capacity');


--without reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity'],
    'target');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity'],
    'capacity');
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION style_cardinalitymatch(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
-- TODO v4 going & coming are to be removed, remove this function
$BODY$
BEGIN

ALTER TABLE edges RENAME cost TO going;
ALTER TABLE edges RENAME reverse_cost TO coming;

--with reverse cost
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going', 'coming'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going', 'coming'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going', 'coming'],
    'target');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going', 'coming'],
    'going');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going', 'coming'],
    'coming');


--without coming
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going'],
    'target');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'going'],
    'going');

ALTER TABLE edges RENAME going TO cost;
ALTER TABLE edges RENAME coming TO reverse_cost;

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION style_cost_flow(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with reverse cost & with reverse_capacity
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost', 'reverse_cost'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost', 'reverse_cost'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost', 'reverse_cost'],
    'target');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost', 'reverse_cost'],
    'capacity');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost', 'reverse_cost'],
    'reverse_capacity');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost', 'reverse_cost'],
    'cost');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost', 'reverse_cost'],
    'reverse_cost');

--with reverse cost & without reverse_capacity
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost', 'reverse_cost'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost', 'reverse_cost'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost', 'reverse_cost'],
    'target');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost', 'reverse_cost'],
    'capacity');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost', 'reverse_cost'],
    'cost');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost & with reverse_capacity
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost'],
    'target');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost'],
    'capacity');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost'],
    'reverse_capacity');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'reverse_capacity', 'cost'],
    'cost');

--without reverse cost & without reverse_capacity
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost'],
    'id');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost'],
    'source');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost'],
    'target');
RETURN QUERY SELECT test_anyInteger(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost'],
    'capacity');
RETURN QUERY SELECT test_anyNumerical(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'capacity', 'cost'],
    'cost');

END;
$BODY$
LANGUAGE plpgsql;

