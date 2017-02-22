
CREATE OR REPLACE FUNCTION style_old_dijkstra_with(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with reverse cost
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');
END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION style_old_dijkstra_no_rev(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
--without reverse cost
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION style_old_dijkstra_no_id_no_reverse(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'source');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'target');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'cost');
END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION style_old_dijkstra_no_id(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with reverse cost
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'source');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'target');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'cost');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'source');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'target');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['source', 'target', 'cost'],
    'cost');
END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION style_old_astar_with(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--with reverse cost
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'reverse_cost');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');
END;
$BODY$
LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION style_old_astar_no_rev(fn TEXT, rest_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

--without reverse cost
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
RETURN QUERY SELECT test_integer(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
RETURN QUERY SELECT test_float(fn, rest_sql,
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');

END;
$BODY$
LANGUAGE plpgsql;

