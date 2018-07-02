-------------------------------------------------------------------------------
-- Legacy FUNCTIONs that were meant to be used by
-- pgRouting developers
-- These FUNCTIONs wer used on the plpgsql FUNCTIONs


-- FILE intended to be used on 3.0
-------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Deprecated on 2.3
--
-- pgr_getColumnName
-- pgr_getTableName
-- pgr_isColumnIndexed
-- pgr_isColumnInTable
-- pgr_quote_ident
-- pgr_versionless
-- pgr_startPoint
-- pgr_endPoint
-- pgr_pointToid RENAMED and deprecated and never announced no one has complained
------------------------------------------------------------------------------


-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_getColumnName(tab text, col text)
RETURNS text AS
$BODY$
DECLARE
    sname text;
    tname text;
    table_name text;
BEGIN
    IF pgr_isColumnInTable(tab, col) THEN RETURN NULL;
    END IF;
    SELECT pgr_getTableName($1) into sname, tname;
    table_name := sname || '.' || tname;
    EXECUTE 'SELECT ' || quote_ident(col) || ' FROM ' || table_name || ' LIMIT 1';
    RETURN col;
    EXCEPTION WHEN others THEN
    BEGIN
        EXECUTE 'SELECT ' || quote_ident(lower(col)) || ' FROM ' || table_name || ' LIMIT 1';
        RETURN lower(col);
        EXCEPTION WHEN others THEN
            RETURN NULL;
    END;

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_getTableName(IN tab text,OUT sname text,OUT tname text)
RETURNS RECORD AS
$BODY$
DECLARE
    table_oid regclass;
    table_name text;
BEGIN
    table_oid := $1::regclass;
    -- $1 := replace($1, '"', '');
    SELECT * FROM _pgr_getTableName($1, 0, 'pgr_getTableName') into sname,tname;
    EXCEPTION WHEN others THEN
    BEGIN
        table_oid := lower($1)::regclass;
        SELECT * FROM _pgr_getTableName(lower($1), 0, 'pgr_getTableName') into sname,tname;
        EXCEPTION WHEN others THEN
        sname = 'public';
        tname = NULL;
    END;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;





-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_isColumnIndexed(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE table_oid regclass;
BEGIN
    table_oid := $1::regclass;
    $1 := replace(tab, '"', '');
    RETURN  _pgr_isColumnIndexed($1, $2);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_isColumnInTable(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE
    cname text;
    sname text;
    tname text;
    table_name text;
BEGIN
    SELECT pgr_getTableName($1) into sname, tname;
    table_name := sname || '.' || tname;
    EXECUTE 'SELECT ' || quote_ident(col) || ' FROM ' || table_name || ' LIMIT 1';
    RETURN true;
    EXCEPTION WHEN others THEN
        BEGIN
            EXECUTE 'SELECT ' || quote_ident(lower(col)) || ' FROM ' || table_name || ' LIMIT 1';
            RETURN true;
            EXCEPTION WHEN others THEN
            RETURN false;
        END;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_quote_ident(idname text)
RETURNS text as
$BODY$
    SELECT  quote_ident($1);
$BODY$
LANGUAGE sql IMMUTABLE;





-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_versionless(v1 text, v2 text)
RETURNS boolean AS
$BODY$
    SELECT _pgr_versionless($1, $2);
$BODY$
LANGUAGE sql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_startPoint(g geometry)
    RETURNS geometry as
$BODY$
    SELECT  ST_startPoint($1);
$BODY$
LANGUAGE sql IMMUTABLE;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_endPoint(g geometry)
    RETURNS geometry as
$BODY$
    SELECT  ST_endPoint($1);
$BODY$
LANGUAGE sql IMMUTABLE;



/* RENAMED and deprecated and never announced no one has complained
-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_pointToId(point geometry, tolerance double precision, vertname text, srid integer)
RETURNS bigint AS
$BODY$
    SELECT _pgr_pointToId($1, $2, $3, $4);
$BODY$
LANGUAGE sql VOLATILE STRICT;
*/

------------------------------------------------------------------------------
-- Deprecated on 2.3
--
-- pgr_flipedges
-- pgr_texttopoints
-- pgr_vidsToDMatrix
-- pgr_vidsToDMatrix
-- pgr_pointstodmatrix
------------------------------------------------------------------------------

-- Added on 2.1.0
-- deprecated on 2.3.0
CREATE OR REPLACE FUNCTION pgr_flipedges(ga geometry[])
    RETURNS geometry[] as
$BODY$
DECLARE
    nn integer;
    i integer;
    g geometry;

BEGIN
    nn := array_length(ga, 1);
    IF nn = 1 THEN
        RETURN ga;
    END IF;

    g := ST_StartPoint(ga[1]);

    IF ST_StartPoint(ga[2]) = g or ST_EndPoint(ga[2]) = g THEN
        ga[1] := ST_Reverse(ga[1]);
    END IF;

    g := ST_EndPoint(ga[1]);

    FOR i in 2 .. nn LOOP
        IF g = ST_EndPoint(ga[i]) THEN
            ga[i] := ST_Reverse(ga[i]);
        END IF;
        g := ST_EndPoint(ga[i]);
    END LOOP;

    RETURN ga;
end;
$BODY$
LANGUAGE plpgsql immutable;



-- Added on 2.1.0
-- deprecated on 2.3.0
CREATE OR REPLACE FUNCTION pgr_texttopoints(pnts text, srid integer DEFAULT(4326))
    RETURNS geometry[] as
$BODY$
DECLARE
    a text[];
    t text;
    p geometry;
    g geometry[];

BEGIN
    a := string_to_array(replace(pnts, ',', ' '), ';');
    FOR t in SELECT unnest(a) LOOP
        p := ST_pointfromtext('POINT(' || t || ')', srid);
        g := g || p;
    END LOOP;

    RETURN g;
end;
$BODY$
LANGUAGE plpgsql immutable;



-- Added on 2.1.0
-- deprecated on 2.3.0
CREATE OR REPLACE FUNCTION pgr_vidsToDMatrix(sql TEXT, vids  INTEGER[], dir BOOLEAN, has_rcost BOOLEAN, want_symmetric BOOLEAN)
RETURNS float8[] AS
$BODY$
DECLARE
dmatrix_row float8[];
dmatrix float8[];
rr RECORD;
ids INTEGER[];
total BIGINT;
BEGIN
    ids :=  ARRAY(SELECT DISTINCT UNNEST(vids) ORDER BY 1);

    IF want_symmetric THEN
        dir = false;
    END IF;

    total := array_length(ids, 1);

    FOR i in 1 .. total LOOP
        dmatrix_row := dmatrix_row || '+Infinity'::float8;
    END LOOP;

    FOR i in 1 .. total LOOP
    dmatrix := dmatrix || ARRAY[dmatrix_row];
    dmatrix[i][i] = 0;
    END LOOP;

    FOR rr IN EXECUTE
        'SELECT start_vid, end_vid, agg_cost FROM pgr_dijkstraCostMatrix($1, $2, $3)'
        USING
            sql, ids, dir
    LOOP
        dmatrix[(SELECT idx FROM generate_subscripts(ids, 1) AS idx WHERE ids[idx] = rr.start_vid)]
            [(SELECT idx FROM generate_subscripts(ids, 1) AS idx WHERE ids[idx] = rr.end_vid)] := rr.agg_cost;
    END LOOP;

    RETURN dmatrix;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100;


-- Added on 2.1.0
-- deprecated on 2.3.0
CREATE OR REPLACE FUNCTION pgr_vidstodmatrix(
    IN vids integer[],
    IN pnts geometry[],
    IN edges text,
    tol float8 DEFAULT(0.1),
    OUT dmatrix double precision[],
    OUT ids integer[])
    RETURNS record as
$BODY$
DECLARE
    dmatrix_row float8[];
    nn integer;
    rr record;
    t float8[];

BEGIN
    ids := array(SELECT DISTINCT unnest(vids::integer[]) ORDER BY 1);

    nn := array_length(ids, 1);

    FOR i in 1 .. nn LOOP
        dmatrix_row := dmatrix_row || '+Infinity'::float8;
    END LOOP;

    FOR i in 1 .. nn LOOP
    dmatrix := dmatrix || ARRAY[dmatrix_row];
    dmatrix[i][i] = 0;
    END LOOP;

    FOR i in 1 .. nn LOOP
        dmatrix := dmatrix || ARRAY[t];
    END LOOP;

    FOR rr IN EXECUTE
        'SELECT start_vid, end_vid, agg_cost FROM pgr_dijkstraCostMatrix($1, $2, false)'
        USING
            'SELECT id, source, target, cost FROM ' || edges,
            ids
    LOOP
        dmatrix[(SELECT idx FROM generate_subscripts(ids, 1) AS idx WHERE ids[idx] = rr.start_vid)]
            [(SELECT idx FROM generate_subscripts(ids, 1) AS idx WHERE ids[idx] = rr.end_vid)] := rr.agg_cost;
    END LOOP;

    RETURN;
end;
$BODY$
LANGUAGE plpgsql STABLE cost 200;



-- Added on 2.1.0
-- deprecated on 2.3.0
CREATE OR REPLACE FUNCTION pgr_pointstodmatrix(pnts geometry[], mode integer default (0), OUT dmatrix double precision[], OUT ids integer[])
    RETURNS record as
$BODY$
DECLARE
    r record;

BEGIN
    dmatrix := array[]::double precision[];
    ids := array[]::integer[];

    FOR r in with nodes AS (SELECT row_number() over()::integer AS id, p FROM (select unnest(pnts) AS p) AS foo)
        SELECT i, array_agg(dist) AS arow FROM (
            SELECT a.id AS i, b.id AS j,
                case when mode=0
                    THEN ST_distance(a.p, b.p)
                    ELSE ST_distance_sphere(a.p, b.p)
                END AS dist
              FROM nodes a, nodes b
             ORDER BY a.id, b.id
           ) AS foo group by i ORDER BY i LOOP

        dmatrix := array_cat(dmatrix, array[r.arow]);
        ids := ids || array[r.i];
    END LOOP;
end;
$BODY$
LANGUAGE plpgsql STABLE;

-----------------------------------------------------------------------

-- Added on 2.1.0
-- deprecated on 2.4.0
CREATE OR REPLACE FUNCTION pgr_pointstovids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))
    RETURNS integer[] as
$BODY$
DECLARE
    v integer[];
    g geometry;
BEGIN
    FOR g in SELECT unnest(pnts) LOOP
        v := v || pgr_pointtoedgenode(edges, g, tol);
    END LOOP;

    RETURN v;
end;
$BODY$
LANGUAGE plpgsql STABLE;



-----------------------------------------------------------------------

-- Added on 2.1.0
-- deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_pointtoedgenode(edges text, pnt geometry, tol float8)
    RETURNS integer as
$BODY$
DECLARE
    rr record;
    pct float;
    debuglevel text;

BEGIN
    execute 'SELECT * FROM ' || quote_ident(edges) ||
            ' where ST_dwithin(''' || pnt::text ||
            '''::geometry, the_geom, ' || tol || ') ORDER BY ST_distance(''' || pnt::text ||
            '''::geometry, the_geom) asc limit 1' into rr;

    IF rr.the_geom IS NOT null THEN
        IF geometrytype(rr.the_geom)='MULTILINESTRING' THEN
            rr.the_geom := ST_GeometryN(rr.the_geom, 1);
        END IF;

        IF _pgr_versionless(postgis_version(), '2.0') THEN
            pct := ST_line_locate_point(rr.the_geom, pnt);
        ELSE
            pct := ST_lineLocatePoint(rr.the_geom, pnt);
        END IF;

        IF pct < 0.5 THEN
            RETURN rr.source;
        ELSE
            RETURN rr.target;
        END IF;
    ELSE
        RETURN -1;
    END IF;
end;
$BODY$
LANGUAGE plpgsql VOLATILE
cost 5;



------------------------------------------------------------------------------
