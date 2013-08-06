-----------------------------------------------------------------------
-- This function should not be used directly. Use assign_vertex_id instead
-- 
-- Inserts a point into a temporary vertices table, and return an id
--  of a new point or an existing point. Tolerance is the minimal distance
--  between existing points and the new point to create a new point.
--
-- Last changes: 2013-03-22
-- Author: Christian Gonzalez
-- Author: Stephen Woodbridge
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_pointToId(p geometry, tolerance double precision)
    RETURNS BIGINT AS 
$BODY$ 

DECLARE
    rec record; 
    pid bigint; 

BEGIN

    SELECT ST_Distance(the_geom, p) AS d, id, the_geom
        INTO rec
        FROM vertices_tmp
        WHERE ST_DWithin(the_geom, p, tolerance)
        ORDER BY d
        LIMIT 1;
    
    IF FOUND THEN
        pid := rec.id;
    ELSE
        INSERT INTO vertices_tmp(the_geom) VALUES (p);
        pid := lastval();
    END IF;

    RETURN pid;

END;
$BODY$
    LANGUAGE 'plpgsql' VOLATILE STRICT; 


-----------------------------------------------------------------------
-- Fill the source and target_id column for all lines. All line ends
--  with a distance less than tolerance, are assigned the same id
--
-- Last changes: 2013-03-22
-- Author: Christian Gonzalez
-- Author: Stephen Woodbridge
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_createTopology(geom_table varchar, tolerance double precision, geo_cname varchar, gid_cname varchar)
    RETURNS VARCHAR AS
$BODY$

DECLARE
    points record;
    tabinfo record;
    source_id bigint;
    target_id bigint;
    totcount bigint;
    rowcount bigint;
    srid integer;
    cname text;
    sql text;
    parts text[];

BEGIN
    /*
     * get the srid of the geometry
     * we could use FIND_SRID(), but that assumes that the table
     * has been entered in the geometry_columns table/view
     * the follow simply fetches the first geometry and gets
     * the srid from that.
    */
    EXECUTE 'SELECT ST_SRID(' || quote_ident(geo_cname) || ') as srid '
        || ' FROM ' || pgr_quote_ident(geom_table)
        || ' WHERE ' || quote_ident(geo_cname)
        || ' IS NOT NULL LIMIT 1'
        INTO tabinfo;

    IF tabinfo IS NULL OR tabinfo.srid IS NULL THEN
        RAISE EXCEPTION 'We can not determine the srid of "%.%"!', geom_table, geo_cname;
    END IF;

    srid := tabinfo.srid;
    cname := quote_ident(geo_cname);

    parts := regexp_split_to_array(geom_table, E'\\.');

    -- get the approximate count of records for geom_table
    IF array_length(parts, 1) = 1 THEN
        EXECUTE 'SELECT reltuples::bigint AS totcount FROM pg_class WHERE relname='''|| quote_ident(parts[1]) ||'''' INTO totcount;
    ELSIF array_length(parts, 1) = 2 THEN
        EXECUTE 'SELECT p.reltuples::bigint AS totcount FROM pg_class p JOIN pg_catalog.pg_namespace n ON  n.oid = p.relnamespace WHERE relname='''|| quote_ident(parts[2]) || '''' || ' AND n.nspname=''' || quote_ident(parts[1]) || '''' INTO totcount;
    END IF;
    totcount := coalesce(totcount, 0);

    /*
     * drop and create the vertices_tmp table
    */

    DROP TABLE IF EXISTS vertices_tmp;
    CREATE TABLE vertices_tmp (id bigserial);
    PERFORM addGeometryColumn('vertices_tmp', 'the_geom', srid, 'POINT', 2);
    CREATE INDEX vertices_tmp_idx ON vertices_tmp USING GIST (the_geom);

    rowcount := 0;
    FOR points IN EXECUTE 'SELECT ' || quote_ident(gid_cname) || ' AS id,'
        || ' PGR_StartPoint(' || cname || ') AS source,'
        || ' PGR_EndPoint('   || cname || ') AS target'
        || ' FROM '  || pgr_quote_ident(geom_table)
        || ' WHERE ' || quote_ident(geo_cname) || ' IS NOT NULL '
    LOOP

        IF rowcount % 10000 = 0 THEN
            RAISE NOTICE '% out of % edges processed', rowcount, totcount;
        END IF;

        rowcount := rowcount + 1;

        source_id := pgr_pointToId(points.source, tolerance);
        target_id := pgr_pointToId(points.target, tolerance);
                                
        sql := 'UPDATE ' || pgr_quote_ident(geom_table) || 
            ' SET source = ' || source_id || ', target = ' || target_id || 
            ' WHERE ' || quote_ident(gid_cname) || ' =  ' || points.id;

        IF sql IS NULL THEN
            RAISE NOTICE 'ERROR: UPDATE % SET source = %, target = % WHERE % = % ', pgr_quote_ident(geom_table), source_id, target_id, quote_ident(gid_cname),  points.id;
        ELSE
            EXECUTE sql;
        END IF;
    END LOOP;

    RETURN 'OK';

END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

