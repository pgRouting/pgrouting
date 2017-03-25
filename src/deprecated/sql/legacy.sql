-------------------------------------------------------------------------------
-- Legacy FUNCTIONs that were ment to be used by 
-- Temporarly while the rewrite of 2.0
-- These FUNCTIONs wer used on the plpgsql functions


-- FILE intended to be used on 3.0
-------------------------------------------------------------------------------

-- I do not know yet if they will create a conflict when putting the legacy

-- Deprecated signature on 2.1.0
CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq-1 , node::integer, edge::integer, cost
    FROM pgr_dijkstra($1, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;




-- Deprecated signature on 2.1.0
CREATE OR REPLACE FUNCTION pgr_drivingDistance(edges_sql text, source INTEGER, distance FLOAT8, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq - 1, node::integer, edge::integer, agg_cost
    FROM pgr_drivingDistance($1, ARRAY[$2]::BIGINT[], $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;




-- Deprecated signature on 2.1.0
CREATE OR REPLACE FUNCTION pgr_ksp(edges_sql text, start_vid integer, end_vid integer, k integer, has_rcost boolean)
RETURNS SETOF pgr_costresult3 AS
$BODY$
    SELECT ((row_number() over()) -1)::integer,  (path_id - 1)::integer, node::integer, edge::integer, cost
    FROM _pgr_ksp($1::text, $2, $3, $4, TRUE, FALSE) WHERE path_id <= k;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000


------------------------------------------------------------------------------

-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_apspJohnson(edges_sql text)
RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () - 1)::integer, start_vid::integer, end_vid::integer, agg_cost 
    FROM  pgr_johnson($1, TRUE);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;




-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_apspWarshall(edges_sql text, directed boolean, has_rcost boolean)
    RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () -1)::integer, start_vid::integer, end_vid::integer, agg_cost
    FROM  pgr_floydWarshall($1, $2);
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_kdijkstraPath(
    sql text,
    source INTEGER,
    targets INTEGER ARRAY,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult3 AS
$BODY$
    SELECT (row_number() over () -1)::integer, end_vid::INTEGER, node::INTEGER, edge::INTEGER, cost
    FROM pgr_dijkstra($1, $2, $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;


-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    sql text,
    source INTEGER,
    targets INTEGER array,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () -1)::integer, start_vid::integer, end_vid::INTEGER, agg_cost
    FROM pgr_dijkstraCost($1, $2, $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;


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
create or replace function pgr_flipedges(ga geometry[])
    returns geometry[] as
$body$
declare
    nn integer;
    i integer;
    g geometry;
    
begin
    nn := array_length(ga, 1);
    if nn = 1 then
        return ga;
    end if;

    g := ST_StartPoint(ga[1]);

    if ST_StartPoint(ga[2]) = g or ST_EndPoint(ga[2]) = g then
        ga[1] := ST_Reverse(ga[1]);
    end if;

    g := ST_EndPoint(ga[1]);

    for i in 2 .. nn loop
        if g = ST_EndPoint(ga[i]) then
            ga[i] := ST_Reverse(ga[i]);
        end if;
        g := ST_EndPoint(ga[i]);
    end loop;

    return ga;
end;
$body$
language plpgsql immutable;



-- Added on 2.1.0
-- deprecated on 2.3.0
create or replace function pgr_texttopoints(pnts text, srid integer DEFAULT(4326))
    returns geometry[] as
$body$
declare
    a text[];
    t text;
    p geometry;
    g geometry[];
    
begin
    a := string_to_array(replace(pnts, ',', ' '), ';');
    for t in select unnest(a) loop
        p := st_pointfromtext('POINT(' || t || ')', srid);
        g := g || p;
    end loop;

    return g;
end;
$body$
language plpgsql immutable;



-- Added on 2.1.0
-- deprecated on 2.3.0
CREATE OR REPLACE FUNCTION pgr_vidsToDMatrix(sql TEXT, vids  INTEGER[], dir BOOLEAN, has_rcost BOOLEAN, want_symmetric BOOLEAN)
RETURNS float8[] AS
$BODY$
DECLARE
directed BOOLEAN;
has_reverse BOOLEAN;
edges_sql TEXT;
dmatrix_row float8[];
dmatrix float8[];
cell RECORD;
unique_vids INTEGER[];
total BIGINT;
from_v BIGINT;
to_v BIGINT;
BEGIN
    RAISE NOTICE 'Deprecated function pgr_vidsToDMatrix';
    has_reverse =_pgr_parameter_check('dijkstra', sql, false);
    edges_sql = sql;
    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            sql = 'SELECT id, source, target, cost FROM (' || sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    unique_vids :=  ARRAY(SELECT DISTINCT UNNEST(vids) ORDER BY 1);

    IF want_symmetric THEN
        directed = false;
    ELSE
        directed = dir;
    END IF;

    total := array_length(unique_vids, 1);

    -- initializing dmatrix
    FOR i in 1 .. total LOOP
        dmatrix_row := dmatrix_row || '+Infinity'::float8;
    END LOOP;
    FOR i in 1 .. total LOOP
    dmatrix := dmatrix || ARRAY[dmatrix_row];
    dmatrix[i][i] = 0;
    END LOOP;

    CREATE TEMP TABLE __x___y____temp AS
        WITH result AS
            (SELECT unnest(unique_vids) AS vid)
        SELECT row_number() OVER() AS idx, vid FROM result;

    FOR cell IN SELECT * FROM pgr_dijkstraCostMatrix(sql, unique_vids, directed) LOOP
        SELECT idx INTO from_v FROM __x___y____temp WHERE vid =  cell.start_vid;
        SELECT idx INTO to_v FROM __x___y____temp WHERE vid =  cell.end_vid;

        dmatrix[from_v][to_v] = cell.agg_cost;
        dmatrix[to_v][from_v] = cell.agg_cost;
    END LOOP;

    DROP TABLE IF EXISTS __x___y____temp;
    RETURN dmatrix;

    EXCEPTION WHEN others THEN 
       DROP TABLE IF EXISTS __x___y____temp;
       raise exception '% %', SQLERRM, SQLSTATE;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100;


-- Added on 2.1.0
-- deprecated on 2.3.0
create or replace function pgr_vidstodmatrix(
    IN vids integer[],
    IN pnts geometry[],
    IN edges text,
    tol float8 DEFAULT(0.1),
    OUT dmatrix double precision[],
    OUT ids integer[])
    returns record as
$body$
declare
    i integer;
    j integer;
    nn integer;
    rr record;
    bbox geometry;
    t float8[];

begin
    -- TODO convert pgr_dijkstraCost(many to many) to a matrix
    RAISE NOTICE 'Deprecated function pgr_vidsToDMatrix';
    -- check if the input arrays has any -1 values, maybe this whould be a raise exception
    if vids @> ARRAY[-1] then
    raise notice 'Some vids are undefined (-1)!';
    dmatrix := null;
    ids := null;
    return;
    end if;

    ids := vids;

    -- get the count of nodes
    nn := array_length(vids,1);

    -- zero out a dummy row
    for i in 1 .. nn loop
        t := t || 0.0::float8;
    end loop;

    -- using the dummy row, zero out the whole matrix
    for i in 1 .. nn loop
    dmatrix := dmatrix || ARRAY[t];
    end loop;

    for i in 1 .. nn-1 loop
        j := i;
        -- compute the bbox for the point needed for this row
        select st_expand(st_collect(pnts[id]), tol) into bbox
          from (select generate_series as id from generate_series(i, nn)) as foo;

        -- compute kdijkstra() for this row
        for rr in execute 'select * from pgr_dijkstracost($1, $2, $3, false)'
                  using 'select id, source, target, cost from ' || edges || 
                        ' where the_geom && ''' || bbox::text || '''::geometry'::text, vids[i], vids[i+1:nn] loop

            -- TODO need to check that all node were reachable from source
            -- I think unreachable paths between nodes returns cost=-1.0

            -- populate the matrix with the cost values, remember this is symmetric
            j := j + 1;
            -- raise notice 'cost(%,%)=%', i, j, rr.agg_cost;
            dmatrix[i][j] := rr.agg_cost;
            dmatrix[j][i] := rr.agg_cost;
        end loop;
    end loop;

end;
$body$
    language plpgsql stable cost 200;





-- Added on 2.1.0
-- deprecated on 2.3.0
create or replace function pgr_pointstodmatrix(pnts geometry[], mode integer default (0), OUT dmatrix double precision[], OUT ids integer[])
    returns record as
$body$
declare
    r record;
    
begin
    dmatrix := array[]::double precision[];
    ids := array[]::integer[];

    for r in with nodes as (select row_number() over()::integer as id, p from (select unnest(pnts) as p) as foo)
        select i, array_agg(dist) as arow from (
            select a.id as i, b.id as j, 
                case when mode=0
                    then st_distance(a.p, b.p)
                    else st_distance_sphere(a.p, b.p)
                end as dist
              from nodes a, nodes b
             order by a.id, b.id
           ) as foo group by i order by i loop

        dmatrix := array_cat(dmatrix, array[r.arow]);
        ids := ids || array[r.i];
    end loop;
end;
$body$
    language plpgsql stable;

-----------------------------------------------------------------------

-- Added on 2.1.0
-- deprecated on 2.4.0
create or replace function pgr_pointstovids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))
    returns integer[] as
$body$
declare
    v integer[];
    g geometry;
begin
    RAISE NOTICE 'Deperecated function: pgr_pointsToVids';
    for g in select unnest(pnts) loop
        v := v || pgr_pointtoedgenode(edges, g, tol);
    end loop;

    return v;
end;
$body$
language plpgsql stable;



-----------------------------------------------------------------------

-- Added on 2.1.0
-- deprecated on 2.5.0
create or replace function pgr_pointtoedgenode(edges text, pnt geometry, tol float8)
    returns integer as
$body$
declare
    rr record;
    pct float;
    debuglevel text;
    
begin
    execute 'select * from ' || _pgr_quote_ident(edges) || 
            ' where st_dwithin(''' || pnt::text ||
            '''::geometry, the_geom, ' || tol || ') order by st_distance(''' || pnt::text ||
            '''::geometry, the_geom) asc limit 1' into rr;

    if rr.the_geom is not null then
        if geometrytype(rr.the_geom)='MULTILINESTRING' THEN
            rr.the_geom := ST_GeometryN(rr.the_geom, 1);
        end if;

        execute 'show client_min_messages' into debuglevel;
        SET client_min_messages='ERROR';
        pct := st_line_locate_point(rr.the_geom, pnt);
        execute 'set client_min_messages  to '|| debuglevel;

        if pct < 0.5 then
            return rr.source;
        else
            return rr.target;
        end if;
    else
        return -1;
    end if;
end;
$body$
language plpgsql volatile
cost 5;



------------------------------------------------------------------------------
