-----------------------------------------------------------------------
-- This function should not be used directly. Use assign_vertex_id instead
--
-- Inserts a point into a temporary vertices table, and return an id
--  of a new point or an existing point. Tolerance is the minimal distance
--  between existing points and the new point to create a new point.
--
-- Last changes: 16.04.2008
-- Author: Christian Gonzalez
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION point_to_id(p geometry, tolerance double precision)
RETURNS BIGINT
AS
$$

DECLARE
    _r record;
    _id bigint;
    _srid integer;

BEGIN

    _srid := Find_SRID('public','vertices_tmp','the_geom');

    SELECT

        Distance(the_geom,GeometryFromText( AsText(p), _srid)) AS d, id, the_geom

    INTO _r FROM vertices_tmp WHERE

        the_geom && Expand(GeometryFromText(AsText(p), _srid), tolerance ) AND Distance(the_geom, GeometryFromText(AsText(p), _srid)) < tolerance

    ORDER BY d LIMIT 1; IF FOUND THEN

        _id:= _r.id;

    ELSE

        INSERT INTO vertices_tmp(the_geom) VALUES (p); _id:=lastval();

    END IF;

    RETURN _id;

END; $$ LANGUAGE 'plpgsql' VOLATILE STRICT;


-----------------------------------------------------------------------
-- Fill the source and target_id column for all lines. All line ends
--  with a distance less than tolerance, are assigned the same id
--
-- Last changes: 16.04.2008
-- Author: Christian Gonzalez
-----------------------------------------------------------------------

CREATE OR REPLACE FUNCTION assign_vertex_id(geom_table varchar, tolerance double precision, geo_cname varchar, gid_cname varchar)
RETURNS VARCHAR AS
$$
DECLARE
    _r record;
    source_id int;
    target_id int;
    srid integer;
BEGIN

    BEGIN
    DROP TABLE vertices_tmp;
    EXCEPTION
    WHEN UNDEFINED_TABLE THEN
    END;

    EXECUTE 'CREATE TABLE vertices_tmp (id serial)';

--    FOR _r IN EXECUTE 'SELECT srid FROM geometry_columns WHERE f_table_name='''|| quote_ident(geom_table)||''';' LOOP
--	srid := _r.srid;
--    END LOOP;

    srid := Find_SRID('public',quote_ident(geom_table),quote_ident(geo_cname));


    EXECUTE 'SELECT addGeometryColumn(''vertices_tmp'', ''the_geom'', '||srid||', ''POINT'', 2)';
    CREATE INDEX vertices_tmp_idx ON vertices_tmp USING GIST (the_geom);

    FOR _r IN EXECUTE 'SELECT ' || quote_ident(gid_cname) || ' AS id,'
	    || ' StartPoint('|| quote_ident(geo_cname) ||') AS source,'
            || ' EndPoint('|| quote_ident(geo_cname) ||') as target'
	    || ' FROM ' || quote_ident(geom_table) || ' WHERE ' || quote_ident(geo_cname) || ' IS NOT NULL '
    LOOP

        source_id := point_to_id(setsrid(_r.source, srid), tolerance);
	target_id := point_to_id(setsrid(_r.target, srid), tolerance);

	EXECUTE 'update ' || quote_ident(geom_table) ||
		' SET source = ' || source_id ||
		', target = ' || target_id ||
		' WHERE ' || quote_ident(gid_cname) || ' =  ' || _r.id;
    END LOOP;

    RETURN 'OK';

END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;

