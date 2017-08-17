
CREATE OR REPLACE FUNCTION style_dijkstra(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

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

CREATE OR REPLACE FUNCTION style_dijkstraTRSP(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

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

CREATE OR REPLACE FUNCTION style_lineGraph(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

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

CREATE OR REPLACE FUNCTION style_withpoints(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

RETURN QUERY SELECT style_dijkstra(fn,  $$, 'SELECT * from pointsOfInterest' $$ || rest_sql);

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
$BODY$
BEGIN

ALTER TABLE edge_table RENAME cost TO going;
ALTER TABLE edge_table RENAME reverse_cost TO coming;

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

ALTER TABLE edge_table RENAME going TO cost;
ALTER TABLE edge_table RENAME coming TO reverse_cost;

END;
$BODY$
LANGUAGE plpgsql;
