----------------------------------------------------------
-- Draws an alpha shape around given set of points.
-- ** This should be rewritten as an aggregate. **
----------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_pointsAsPolygon(query varchar)
       RETURNS SETOF pgr_geomResult AS
$$
DECLARE
     r record;
     path_result record;					     
     i int;							     
     q text;
     x float8[];
     y float8[];
     geom pgr_geomResult;
BEGIN
	
     i := 1;								     
     q := 'select 1 as gid, ST_GeometryFromText(''POLYGON((';
     
     FOR path_result IN EXECUTE 'select x, y from alphashape('''|| 
         query || ''')' LOOP
         x[i] = path_result.x;
         y[i] = path_result.y;
         i := i+1;
     END LOOP;

     q := q || x[1] || ' ' || y[1];
     i := 2;

     WHILE x[i] IS NOT NULL LOOP
         q := q || ', ' || x[i] || ' ' || y[i];
         i := i + 1;
     END LOOP;

    q := q || ', ' || x[1] || ' ' || y[1];
    q := q || '))'',-1) as the_geom';

    EXECUTE q INTO r;

	geom.seq  := 0;
    geom.id1  := 0;
    geom.id2  := 0;
    geom.geom := r.the_geom;
	RETURN NEXT geom;

    RETURN;
END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;


CREATE OR REPLACE FUNCTION pgr_drivingDistance(
    table_name varchar, 
    x double precision, 
    y double precision,
    distance double precision,
    delta double precision,
    cost varchar,
    reverse_cost varchar,
    directed boolean,
    has_reverse_cost boolean)
RETURNS SETOF pgr_geomResult AS
$$
DECLARE
    q text;
    srid integer;
    r record;
    geom pgr_geomResult;
BEGIN
     
    EXECUTE 'SELECT srid FROM geometry_columns WHERE f_table_name = ''' ||
        table_name || '''' INTO r;
    srid := r.srid;
     
    -- RAISE NOTICE 'SRID: %', srid;

    q := 'SELECT * FROM pgr_pointsAsPolygon(' ||
        ' ''SELECT a.id1::integer AS id, b.x1::double precision AS x, ' ||
        ' b.y1::double precision AS y' || ' FROM pgr_drivingDistance(' ||
        ' ''''''''SELECT gid AS id,source::integer,target::integer, ' ||
        cost || '::double precision AS cost, ' ||
        reverse_cost || '::double precision as reverse_cost FROM ' ||
        table_name || ' WHERE ST_SetSRID(''''''''''''''''BOX3D(' ||
        x-delta || ' ' || y-delta || ', ' ||
        x+delta || ' ' || y+delta || ')''''''''''''''''::BOX3D, ' ||
        srid || ') && the_geom  '''''''', (' ||
        'SELECT id FROM pgr_findNodeByNearestLinkDwithin(''''''''POINT(' ||
        x || ' ' || y || ')'''''''',' || distance/10 || ',''''''''' ||
        table_name || ''''''''')),' || distance ||
        ',true, true) a, (SELECT * FROM ' || table_name ||
        ' WHERE ST_SetSRID(''''''''BOX3D(' ||
        x-delta || ' ' || y-delta || ', ' ||
        x+delta || ' ' || y+delta || ')''''''''::BOX3D, '
        || srid || ') && the_geom) b WHERE a.id1 = b.source'')';

    -- RAISE NOTICE 'Query: %', q;
     
     EXECUTE q INTO r;
     geom.seq  := r.seq;
     geom.id1  := r.id1;
     geom.id2  := r.id2;
     geom.geom := r.geom;
     RETURN NEXT geom;
     
     RETURN;

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;



-----------------------------------------------------------------------
-- Calculates the driving distance.
--
-- A delta-sized bounding box around the start is used for data clipping.
--
-- This function differs from the pgr_drivingDistance in that the signature
-- is now similar to the shortest path delta functions and the delta is
-- passed as argument.
--
-- If you're accustomed to the shortest path delta functions, you probably
-- want to use this as the preferred way to get the driving distance.
--
-- table_name        the table name to work on
-- source_id         start id
-- distance          the max. cost
-- delta             delta for data clipping
-- directed          is graph directed
-- has_reverse_cost  use reverse_cost column
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_drivingDistance(
    table_name varchar,
    source_id integer,
	distance double precision,
    delta float8,
    directed boolean,
    has_reverse_cost boolean)
RETURNS SETOF pgr_geomResult AS
$$
DECLARE
    q text;
    srid integer;
    r record;
    geom pgr_geomResult;
    source_x float8;
    source_y float8;
BEGIN

    EXECUTE 'select ST_X(ST_StartPoint(the_geom)) as source_x from ' ||
            quote_ident(table_name) || ' where source = ' ||
            source_id || ' limit 1' INTO r;
    source_x := r.source_x;


    EXECUTE 'select ST_Y(ST_StartPoint(the_geom)) as source_y from ' ||
            quote_ident(table_name) || ' where source = ' ||
            source_id || ' limit 1' INTO r;
    source_y := r.source_y;

    RETURN QUERY select * from pgr_drivingDistance(table_name,
        source_x, source_y, distance, delta, directed, has_reverse_cost);

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;

