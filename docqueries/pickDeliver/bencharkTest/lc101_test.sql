
DROP EXTENSION pgrouting;
CREATE EXTENSION pgrouting;


SET search_path TO testdata, public;
SELECT * FROM pgr_pickDeliver(
    'select nid as id, service as stime, tw_open as etime, tw_close as ltime, pid as pindex, did as dindex, x, y, demand from lc101'::text, 25, 200, 30)
;
