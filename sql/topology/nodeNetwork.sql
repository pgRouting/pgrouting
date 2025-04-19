/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Author: Nicolas Ribot, 2013
EDITED by Adrien Berchet, 2020

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/


---------------------------
-- pgr_nodeNetwork
---------------------------


--v2.6
CREATE FUNCTION pgr_nodeNetwork(
  TEXT, -- edge table (required)
  DOUBLE PRECISION, -- tolerance (required)
  id TEXT DEFAULT 'id',
  the_geom TEXT DEFAULT 'the_geom',
  table_ending TEXT DEFAULT 'noded',
  rows_where TEXT DEFAULT ''::TEXT,
  outall BOOLEAN DEFAULT FALSE)
RETURNS TEXT AS $BODY$
DECLARE
  /*
   * Author: Nicolas Ribot, 2013 ; Adrien Berchet, 2020
  */
  sname TEXT;  -- schema of tables
  tname TEXT;  -- in table name
  oname TEXT;  -- out table name

  sridinfo record;
  splits BIGINT := 0;
  touched BIGINT := 0;
  untouched BIGINT := 0;
  geomtype TEXT;
  debuglevel TEXT;
  rows_where_out TEXT;

  the_query TEXT;
  the_out_query TEXT;
  the_table TEXT[];
  sqlhint TEXT;
  out_table_exists BOOLEAN;

BEGIN
  RAISE NOTICE 'PROCESSING:';
  RAISE NOTICE 'id: %', id;
  RAISE NOTICE 'the_geom: %', the_geom;
  RAISE NOTICE 'table_ending: %', table_ending;
  RAISE NOTICE 'rows_where: %', rows_where;
  RAISE NOTICE 'outall: %', outall;
  RAISE NOTICE 'pgr_nodeNetwork(''%'', %, ''%'', ''%'', ''%'', ''%'',  %)',
    $1, $2, id,  the_geom, table_ending, rows_where, outall;
  RAISE NOTICE 'Performing checks, please wait .....';
  EXECUTE 'SHOW client_min_messages' INTO debuglevel;

  the_table := parse_ident($1);

  IF array_length(the_table,1) = 1 THEN
    the_table[2] := the_table[1];
    the_table[1] := (SELECT current_schema);
  END IF;

  sname := the_table[1];
  tname := the_table[2];
  oname := the_table[2] || '_' || table_ending;

  rows_where_out = CASE WHEN length(rows_where) > 2 AND NOT outall THEN ' AND (' || rows_where || ')' ELSE '' END;
  rows_where = CASE WHEN length(rows_where) > 2 THEN rows_where ELSE 'true' END;

  -- building query
  BEGIN
    the_query := format(
      $$
      SELECT %s AS id, %s AS the_geom, ST_SRID(%s) AS srid FROM %I.%I WHERE (%s)
      $$,
      id, the_geom, the_geom, sname, tname, rows_where);
    RAISE DEBUG 'Checking: %',the_query;
    the_query := _pgr_checkQuery(the_query);

    EXECUTE format($$SELECT geometrytype(%s) FROM %I.%I limit 1$$, the_geom, sname, tname) INTO geomtype;
    IF geomtype IS NULL THEN
      RAISE WARNING '-------> Table %.% must contain invalid geometries',sname, tname;
      RETURN 'FAIL';
    END IF;

    EXCEPTION WHEN OTHERS THEN
      GET STACKED DIAGNOSTICS sqlhint = PG_EXCEPTION_HINT;
      RAISE EXCEPTION '%', SQLERRM USING HINT = sqlhint, ERRCODE = SQLSTATE;
  END;

  -- checking query columns
  BEGIN
    RAISE DEBUG 'Checking %', $1;

    IF NOT _pgr_checkColumn(the_query, 'id', 'ANY-INTEGER', true) THEN
      RAISE NOTICE  'ERROR: id column "%"  NOT found IN %.%', id, sname, tname;
      RETURN 'FAIL';
    END IF;

    IF NOT _pgr_checkColumn(the_query, 'the_geom', 'geometry', true) THEN
      RAISE NOTICE  'ERROR: the_geom  column "%"  NOT found IN %.%', the_geom, sname, tname;
      RETURN 'FAIL';
    END IF;

    EXECUTE the_query || ' LIMIT 1' INTO sridinfo;

    IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
      RAISE NOTICE 'ERROR: Can NOT determine the srid of the geometry "%" IN table %.%', the_geom, sname, tname;
      RETURN 'FAIL';
    END IF;
    RAISE DEBUG '  -----> SRID found %', sridinfo.srid;
    EXCEPTION WHEN OTHERS THEN
      RAISE NOTICE 'ERROR: Can NOT determine the srid of the geometry "%" IN table %.%', th_geom, sname, tname;
      RETURN 'FAIL';
  END;

---------------
  -- building query to check if table_noded exists
  BEGIN
    the_out_query := format(
      $$
      SELECT id, old_id, sub_id, source, target, %s FROM %I.%I LIMIT 1
      $$,
      the_geom, sname, oname);

    the_out_query := _pgr_checkQuery(the_out_query);
    out_table_exists := true;

    EXCEPTION WHEN OTHERS THEN
      out_table_exists := false;
  END;

---------------
  BEGIN
    RAISE DEBUG 'initializing %.%', sname, oname;
    IF out_table_exists THEN
      EXECUTE format(
        $$TRUNCATE TABLE %I.%I RESTART IDENTITY$$,
        sname, oname);
    ELSE
      SET client_min_messages TO warning;
      EXECUTE format('CREATE TABLE %I.%I (
        id bigserial PRIMARY KEY,
        old_id BIGINT,
        sub_id INTEGER,
        source BIGINT,
        target BIGINT)',
        sname, oname);
      RAISE DEBUG '  ------> Create geometry column of type %', geomtype;
      EXECUTE format($$
        SELECT addGeometryColumn('%I', '%I', '%I', %s, %L, 2)$$,
        sname, oname, the_geom, sridinfo.srid, geomtype);
      EXECUTE 'SET client_min_messages TO '|| debuglevel;
      RAISE DEBUG '  ------>OK';
    END IF;
  END;
----------------

  RAISE NOTICE 'Processing, please wait .....';


  EXECUTE format(
    $$
      INSERT INTO %2$I.%3$I (old_id, sub_id, %4$I)
      SELECT id, sub_id, geom FROM pgr_separateCrossing(replace('%1$s', 'AS the_geom', 'AS geom'), %5$s)
      UNION
      SELECT id, sub_id, geom FROM pgr_separateTouching(replace('%1$s', 'AS the_geom', 'AS geom'), %5$s)
    $$,
    the_query, sname, oname, the_geom, $2);

  GET DIAGNOSTICS splits = ROW_COUNT;

  EXECUTE format(
    $$
    WITH diff AS (SELECT DISTINCT old_id FROM %I.%I)
    SELECT count(*) FROM diff
    $$,
    sname, oname) INTO touched;

  -- here, it misses all original line that did not need to be cut by intersection points: these lines
  -- are already clean
  -- inserts them in the final result: all lines which gid is not in the res table.
  EXECUTE format(
    $$
      INSERT INTO %2$I.%3$I (old_id , sub_id, %5$I) (
        WITH
        original AS (%1$s),
        used AS (SELECT DISTINCT old_id FROM %2$I.%3$I)
        SELECT id, 1 AS sub_id, the_geom
        FROM original
        WHERE id NOT IN (SELECT old_id FROM used) %4$s)
    $$,
  the_query, sname, oname, rows_where_out, the_geom);
  GET DIAGNOSTICS untouched = ROW_COUNT;

  RAISE NOTICE '  Split Edges: %', touched;
  RAISE NOTICE ' Untouched Edges: %', untouched;
  RAISE NOTICE '     Total original Edges: %', touched+untouched;
  RAISE NOTICE ' Edges generated: %', splits;
  RAISE NOTICE ' Untouched Edges: %',untouched;
  RAISE NOTICE '       Total New segments: %', splits+untouched;
  RAISE NOTICE ' New Table: %.%', sname, oname;
  RAISE NOTICE '----------------------------------';

  RETURN 'OK';
END;
$BODY$ LANGUAGE 'plpgsql' VOLATILE STRICT COST 100;

-- COMMENTS
COMMENT ON FUNCTION pgr_nodeNetwork(TEXT, DOUBLE PRECISION, TEXT, TEXT, TEXT, TEXT, BOOLEAN) IS 'pgr_nodeNetwork
- Parameters
  - Edge table name
  - tolerance
- Optional parameters
  - id := ''id''
  - the_geom := ''the_geom''
  - table_ending := ''noded''
  - rows_where := ''''
  - outall := false
- DOCUMENTATION:
  - ${PROJECT_DOC_LINK}/pgr_nodeNetwork.html
';
