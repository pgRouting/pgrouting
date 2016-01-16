/* 
Performance tester for pgr_floydWarshall & pgr_Johnson

Not using a bounding box
TODO check that pgr_floydwarshall or pgr_jhonson is passed in fn

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

select * from fw_j_statsQuery(100 , 100, 200, 30,
 'pgr_floydWarshall','select id, source, target, cost, reverse_cost from ways order by id');

select * from fw_j_statsQuery(100 , 100, 200, 30,
 'pgr_johnson','select id, source, target, cost, reverse_cost from ways order by id');

*/



create or replace function fw_j_statsQuery(low integer, delta integer, max integer, n integer, fn text, query_sql text)
returns TABLE (
   seq    INTEGER,
   avg_rows float,
   lim   integer,
   avgt   float,
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
    sql := 'select count(*) as cnt from ' || fn || '(' || quote_literal(query_sql || ' limit ' || lim ) || ')';
    raise notice '%', sql;
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
