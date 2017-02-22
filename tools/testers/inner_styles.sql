
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

