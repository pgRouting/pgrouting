/*
Performance for pgr_floydWarshall & pgr_Johnson

Using a bounding box

create or replace function fw_j_statsQuery(low , delta , max , m , query_sql text)

starts from low edges
each change is with delta edges
ends with max edges
each query is executed M times
query_sql the query

RETURNS
 avg_rows average numbers of rows returned
 lim      limit used
 avgt     average seconds to perform the query
 err      error if any


Example usage

select * from fw_j_BB_statsQuery(0.001 , 0.001, 0.01, 1,
 'pgr_floydWarshall','select gid AS id, source, target, cost, reverse_cost from ways');

select * from fw_j_BB_statsQuery(0.001 , 0.001, 0.01, 1,
 'pgr_johnson','select gid AS id, source, target, cost, reverse_cost from ways');

*/



create or replace function fw_j_BB_statsQuery(low FLOAT, delta FLOAT, max FLOAT, n integer, fn text, query_sql text)
returns TABLE (
   seq    INTEGER,
   avg_rows FLOAT,
   cnt_edge_rows BIGINT,
   lim   FLOAT,
   avgt   FLOAT,
   density FLOAT,
   err text) AS

$body$
DECLARE
  time1 time;
  time2 time;
  sumt float;
  deltaTime time;
  info record;
  sql TEXT;
BEGIN
    lim := low;
    seq := 1;
    LOOP
        EXIT WHEN lim > max;

        sql =
            'WITH  buffer AS (SELECT ST_Buffer(ST_Centroid(ST_Extent(the_geom)),' || lim || ') as geom FROM ways),
                bbox AS (SELECT ST_Envelope(ST_Extent(geom)) as box from buffer),
                query AS (' || query_sql || ' where the_geom && (SELECT box from bbox))
            SELECT count(*) AS cnt FROM query';
        EXECUTE sql INTO info;
        cnt_edge_rows := info.cnt;
        sql =
            'WITH  buffer AS (SELECT ST_Buffer(ST_Centroid(ST_Extent(the_geom)),' || lim || ') as geom FROM ways),
                bbox AS (SELECT ST_Envelope(ST_Extent(geom)) as box from buffer),
                query AS (' || query_sql || ' where the_geom && (SELECT box from bbox)),
                edges1 AS (SELECT count(*) AS cnt1 FROM query where cost > 0),
                edges2 AS (SELECT count(*) AS cnt2 FROM query where reverse_cost > 0)
            SELECT cnt1 + cnt2 AS cnt FROM edges1 , edges2';
        EXECUTE sql INTO info;
        density := info.cnt;
        sql =
            'WITH  buffer AS (SELECT ST_Buffer(ST_Centroid(ST_Extent(the_geom)),' || lim || ') as geom FROM ways),
                bbox AS (SELECT ST_Envelope(ST_Extent(geom)) as box from buffer),
                query AS (' || query_sql || ' where the_geom && (SELECT box from bbox)),
                s_vertices AS (SELECT DISTINCT source AS vertex FROM query),
                t_vertices AS (SELECT DISTINCT target AS vertex FROM query),
                vertices AS ( SELECT vertex FROM (
                    (SELECT * FROM s_vertices)
                    UNION
                    (SELECT * FROM t_vertices)) AS a)
            SELECT count(*) AS cnt FROM vertices';
        EXECUTE sql INTO info;
        density := density / (info.cnt * (info.cnt - 1));

        sql = 'SELECT count(*) as cnt from ' || fn || '(
            ''WITH  buffer AS (SELECT ST_Buffer(ST_Centroid(ST_Extent(the_geom)),' || lim || ') as geom FROM ways),
                bbox AS (SELECT ST_Envelope(ST_Extent(geom)) as box from buffer)
            ' || query_sql || ' where the_geom && (SELECT box from bbox)'')';

        -- RAISE notice '%', sql;
        sumt := 0;
        avg_rows = 0;
        FOR j in 1 .. n LOOP
                -- notice just to inform that we are working because it takes a lot of time
                IF (j % 5 =  0) THEN RAISE NOTICE 'Running test %, % with limit % ', seq, j, lim; END IF;
                time1 := clock_timestamp();

                BEGIN
                    execute sql into info;
                    EXCEPTION WHEN OTHERS THEN
                        err = SQLERRM;
                END;
                time2 := clock_timestamp();
                avg_rows := avg_rows + info.cnt;
                deltaTime := time2 - time1;
                sumt = sumt + extract(epoch from deltaTime);
        END LOOP;
        avgt := sumt/n;
        avg_rows := avg_rows/n;
        raise notice '% % % % % %', seq, avg_rows, cnt_edge_rows, density, lim, avgt;
        RETURN next;
        seq := seq + 1;
        lim = lim + delta;
    END LOOP;

END
$body$ language plpgsql volatile strict   cost 100 rows 100;




/*
Performance for pgr_floydWarshall & pgr_Johnson

Not using a bounding box

create or replace function fw_j_statsQuery(low , delta , max , m , query_sql text)

starts from low edges
each change is with delta edges
ends with max edges
each query is executed M times
query_sql the query

RETURNS
avg_rows average numbers of rows returned
lim      limit used
avgt     average seconds to perform the query
err      error if any


Example usage

select * from fw_j_statsQuery(100 , 100, 200, 1,
    'pgr_floydWarshall','select gid as id, source, target, cost, reverse_cost from ways order by id');

select * from fw_j_statsQuery(100 , 100, 200, 1,
    'pgr_johnson','select gid as id, source, target, cost, reverse_cost from ways order by id');

     */



    create or replace function fw_j_statsQuery(low integer, delta integer, max integer, n integer, fn text, query_sql text)
    returns TABLE (
        seq    INTEGER,
        avg_rows float,
        cnt_edge_rows BIGINT,
        lim   integer,
        avgt   float,
        density FLOAT,
        err text) AS

    $body$
    DECLARE
    time1 time;
    time2 time;
    sumt float;
    deltaTime time;
    info record;
    sql TEXT;
    BEGIN
        lim := low;
        seq := 1;
        LOOP
            EXIT WHEN lim > max;

            sql := '
                WITH
                    edges AS (' || query_sql || ')
                SELECT count(*) as cnt from edges where id <= ' || lim;
            EXECUTE sql INTO info;
            cnt_edge_rows := info.cnt;

            sql := '
                WITH
                    edges AS (' || query_sql || '),
                    query AS ( SELECT * from edges where id <= ' || lim || '),
                    edges1 AS (SELECT count(*) AS cnt1 FROM query where cost > 0),
                    edges2 AS (SELECT count(*) AS cnt2 FROM query where reverse_cost > 0)
                SELECT cnt1 + cnt2 AS cnt FROM edges1 , edges2';
            EXECUTE sql INTO info;
            density := info.cnt;

            sql := '
                WITH
                    edges AS (' || query_sql || '),
                    query AS (SELECT * from edges where id <= ' || lim || '),
                    s_vertices AS ( SELECT DISTINCT source AS vertex FROM edges),
                    t_vertices AS ( SELECT DISTINCT target AS vertex FROM edges),
                    vertices AS ( SELECT vertex FROM (
                        (SELECT * FROM s_vertices)
                        UNION
                        (SELECT * FROM t_vertices)) AS a)
                SELECT count(*) AS cnt FROM vertices';
            EXECUTE sql INTO info;
            density := density / (info.cnt * (info.cnt - 1));

            sql := '
                SELECT count(*) as cnt from ' || fn || '(
                    ''WITH
                        edges AS (' || query_sql || ')
                    SELECT * from edges where id <= ' || lim || ' '' )';
            -- raise notice '%', sql;
            sumt := 0;
            avg_rows = 0;
            FOR j in 1 .. n
                LOOP
                    -- notice just to inform that we are working because it takes a lot of time
                    if (j % 5 =  0) THEN RAISE NOTICE 'Running test %, % with limit % ', seq, j, lim; END IF;
                        time1 := clock_timestamp();

                        BEGIN
                            execute sql into info;
                            EXCEPTION WHEN OTHERS THEN
                                err = SQLERRM;
                    END;
                    avg_rows := avg_rows + info.cnt;
                    time2 := clock_timestamp();
                    deltaTime := time2 - time1;
                    sumt = sumt + extract(epoch from deltaTime);
                    END LOOP;
                    avgt := sumt/n;
                    avg_rows := avg_rows/n;
                    raise notice '% % % %', seq, avg_rows, lim, avgt;
                    return next;
                    seq := seq + 1;
                    lim = lim + delta;
                    END LOOP;

                    END
                    $body$ language plpgsql volatile strict   cost 100 rows 100;
