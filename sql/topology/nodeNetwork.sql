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


CREATE OR REPLACE FUNCTION pgr_nodeNetwork(
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
  edge_table TEXT := $1;
  tolerance TEXT := $2;
  p_num int := 0;
  p_ret TEXT := '';
  pgis_ver_old BOOLEAN := _pgr_versionless(postgis_lib_version(), '2.1.0.0');
  vst_line_substring TEXT;
  vst_line_locate_point TEXT;
  intab TEXT;
  outtab TEXT;
  n_pkey TEXT;
  n_geom TEXT;
  naming record;
  sname TEXT;
  tname TEXT;
  outname TEXT;
  srid INTEGER;
  sridinfo record;
  splits BIGINT;
  touched BIGINT;
  untouched BIGINT;
  geomtype TEXT;
  debuglevel TEXT;
  rows_where_out TEXT;
BEGIN
  RAISE NOTICE 'PROCESSING:';
  RAISE NOTICE 'id: %', id;
  RAISE NOTICE 'the_geom: %', the_geom;
  RAISE NOTICE 'table_ending: %', table_ending;
  RAISE NOTICE 'rows_where: %', rows_where;
  RAISE NOTICE 'outall: %', outall;
  RAISE NOTICE 'pgr_nodeNetwork(''%'', %, ''%'', ''%'', ''%'', ''%'',  %)',
    edge_table, tolerance, id,  the_geom, table_ending, rows_where, outall;
  RAISE NOTICE 'Performing checks, please wait .....';
  EXECUTE 'SHOW client_min_messages' INTO debuglevel;

  BEGIN
    RAISE DEBUG 'Checking % exists',edge_table;
    EXECUTE 'SELECT * FROM _pgr_getTableName('||quote_literal(edge_table)||',0)' INTO naming;
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
    RAISE NOTICE '-------> % NOT found',edge_table;
      RETURN 'FAIL';
    ELSE
      RAISE DEBUG '  -----> OK';
    END IF;

    intab=sname||'.'||tname;
    outname=tname||'_'||table_ending;
    outtab= sname||'.'||outname;
    rows_where_out = CASE WHEN length(rows_where) > 2 AND NOT outall THEN ' AND (' || rows_where || ')' ELSE '' END;
    rows_where = CASE WHEN length(rows_where) > 2 THEN ' WHERE (' || rows_where || ')' ELSE '' END;
  END;

  BEGIN
    RAISE DEBUG 'Checking id column "%" columns IN  % ',id,intab;
    EXECUTE 'SELECT _pgr_getColumnName('||quote_literal(intab)||','||quote_literal(id)||')' INTO n_pkey;
    IF n_pkey is NULL THEN
      RAISE NOTICE  'ERROR: id column "%"  NOT found IN %',id,intab;
      RETURN 'FAIL';
    END IF;
  END;

  BEGIN
    RAISE DEBUG 'Checking id column "%" columns IN  % ',the_geom,intab;
    EXECUTE 'SELECT _pgr_getColumnName('||quote_literal(intab)||','||quote_literal(the_geom)||')' INTO n_geom;
    IF n_geom is NULL THEN
      RAISE NOTICE  'ERROR: the_geom  column "%"  NOT found IN %',the_geom,intab;
      RETURN 'FAIL';
    END IF;
  END;

  IF n_pkey=n_geom THEN
    RAISE NOTICE  'ERROR: id AND the_geom columns have the same name "%" IN %',n_pkey,intab;
    RETURN 'FAIL';
  END IF;

  BEGIN
    RAISE DEBUG 'Checking the SRID of the geometry "%"', n_geom;
    EXECUTE 'SELECT ST_SRID(' || quote_ident(n_geom) || ') AS srid '
        || ' FROM ' || _pgr_quote_ident(intab)
        || ' WHERE ' || quote_ident(n_geom)
        || ' IS NOT NULL LIMIT 1' INTO sridinfo;
    IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
      RAISE NOTICE 'ERROR: Can NOT determine the srid of the geometry "%" IN table %', n_geom,intab;
      RETURN 'FAIL';
    END IF;
    srid := sridinfo.srid;
    RAISE DEBUG '  -----> SRID found %',srid;
    EXCEPTION WHEN OTHERS THEN
      RAISE NOTICE 'ERROR: Can NOT determine the srid of the geometry "%" IN table %', n_geom,intab;
      RETURN 'FAIL';
  END;

  BEGIN
    RAISE DEBUG 'Checking "%" column IN % is indexed',n_pkey,intab;
    IF (_pgr_isColumnIndexed(intab,n_pkey)) THEN
      RAISE DEBUG '  ------>OK';
    ELSE
      RAISE DEBUG ' ------> Adding  index "%_%_idx".',n_pkey,intab;

      SET client_min_messages TO warning;
      EXECUTE 'CREATE INDEX '||tname||'_'||n_pkey||'_idx ON '||_pgr_quote_ident(intab)||' USING btree('||quote_ident(n_pkey)||')';
      EXECUTE 'SET client_min_messages TO '|| debuglevel;
    END IF;
  END;

  BEGIN
    RAISE DEBUG 'Checking "%" column IN % is indexed',n_geom,intab;
    IF (_pgr_iscolumnindexed(intab,n_geom)) THEN
      RAISE DEBUG '  ------>OK';
    ELSE
      RAISE DEBUG ' ------> Adding unique index "%_%_gidx".',intab,n_geom;
      SET client_min_messages TO warning;
      EXECUTE 'CREATE INDEX '
        || quote_ident(tname || '_' || n_geom || '_gidx' )
        || ' ON ' || _pgr_quote_ident(intab)
        || ' USING gist (' || quote_ident(n_geom) || ')';
      EXECUTE 'SET client_min_messages  TO '|| debuglevel;
    END IF;
  END;

---------------
  BEGIN
    RAISE DEBUG 'initializing %', outtab;
    EXECUTE 'SELECT * FROM _pgr_getTableName('||quote_literal(outtab)||',0)' INTO naming;
    IF sname=naming.sname  AND outname=naming.tname  THEN
      EXECUTE 'TRUNCATE TABLE '||_pgr_quote_ident(outtab)||' RESTART IDENTITY';
      EXECUTE 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(outname)||','||quote_literal(n_geom)||')';
    ELSE
      SET client_min_messages TO warning;
      EXECUTE 'CREATE TABLE '||_pgr_quote_ident(outtab)||' (
        id bigserial PRIMARY KEY,
        old_id INTEGER,
        sub_id INTEGER,
        source BIGINT,
        target BIGINT)';
    END IF;
    EXECUTE 'SELECT geometrytype('||quote_ident(n_geom)||') FROM '||_pgr_quote_ident(intab)||' limit 1' INTO geomtype;
    RAISE DEBUG '  ------> Create geometry column of type %', geomtype;
    EXECUTE 'SELECT addGeometryColumn('||quote_literal(sname)||','||quote_literal(outname)||','||quote_literal(n_geom)||','|| srid||', '||quote_literal(geomtype)||', 2)';
    EXECUTE 'CREATE INDEX '||quote_ident(outname||'_'||n_geom||'_idx')||' ON '||_pgr_quote_ident(outtab)||' USING GIST ('||quote_ident(n_geom)||')';
    EXECUTE 'SET client_min_messages TO '|| debuglevel;
    RAISE DEBUG '  ------>OK';
  END;
----------------

  RAISE NOTICE 'Processing, please wait .....';

  if pgis_ver_old THEN
    vst_line_substring    := 'st_line_substring';
    vst_line_locate_point := 'st_line_locate_point';
  ELSE
    vst_line_substring    := 'st_linesubstring';
    vst_line_locate_point := 'st_linelocatepoint';
  END IF;

  -- First creates temp table with intersection points
  p_ret = 'CREATE TEMP TABLE intergeom ON COMMIT DROP AS (
    SELECT l1.' || quote_ident(n_pkey) || ' AS l1id,
         l2.' || quote_ident(n_pkey) || ' AS l2id,
         l1.' || quote_ident(n_geom) || ' AS line,
         _pgr_startpoint(l2.' || quote_ident(n_geom) || ') AS source,
         _pgr_endpoint(l2.' || quote_ident(n_geom) || ') AS target,
         st_closestPoint(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ') AS geom
    FROM (SELECT * FROM ' || _pgr_quote_ident(intab) || rows_where || ') AS l1
    JOIN (SELECT * FROM ' || _pgr_quote_ident(intab) || rows_where || ') AS l2
    ON (st_dwithin(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ', ' || tolerance || '))'||
    'WHERE l1.' || quote_ident(n_pkey) || ' <> l2.' || quote_ident(n_pkey)||' AND
    st_equals(_pgr_startpoint(l1.' || quote_ident(n_geom) || '),_pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false AND
    st_equals(_pgr_startpoint(l1.' || quote_ident(n_geom) || '),_pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false AND
    st_equals(_pgr_endpoint(l1.' || quote_ident(n_geom) || '),_pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false AND
    st_equals(_pgr_endpoint(l1.' || quote_ident(n_geom) || '),_pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false )';
  RAISE DEBUG '%', p_ret;
  EXECUTE p_ret;

  -- second temp table with locus (index of intersection point on the line)
  -- to avoid updating the previous table
  -- we keep only intersection points occurring onto the line, not at one of its ends
  -- drop table if exists inter_loc;
  p_ret= 'CREATE TEMP TABLE inter_loc ON COMMIT DROP AS (
    SELECT l1id, l2id, ' || vst_line_locate_point || '(line,point) AS locus FROM (
    SELECT DISTINCT l1id, l2id, line, (ST_DumpPoints(geom)).geom AS point FROM intergeom) AS foo
    WHERE ' || vst_line_locate_point || '(line,point)<>0 and ' || vst_line_locate_point || '(line,point)<>1)';
  RAISE DEBUG '%',p_ret;
  EXECUTE p_ret;

  -- index on l1id
  CREATE INDEX inter_loc_id_idx ON inter_loc(l1id);

  -- Then computes the intersection on the lines subset, which is much smaller than full set
  -- as there are very few intersection points

  --- outab needs to be formally created with id, old_id, subid,the_geom, source,target
  --- so it can be inmediatly be used with createTopology

  P_RET = 'INSERT INTO '||_pgr_quote_ident(outtab)||' (old_id,sub_id,'||quote_ident(n_geom)||') (  WITH cut_locations AS
  (
    SELECT l1id AS lid, locus
    FROM inter_loc
    -- then generates start AND end locus for each line that have to be cut buy a location point
    UNION ALL
    SELECT DISTINCT i.l1id  AS lid, 0 AS locus
    FROM inter_loc i LEFT JOIN ' || _pgr_quote_ident(intab) || ' b ON (i.l1id = b.' || quote_ident(n_pkey) || ')
    UNION ALL
    SELECT DISTINCT i.l1id  AS lid, 1 AS locus
    FROM inter_loc i LEFT JOIN ' || _pgr_quote_ident(intab) || ' b ON (i.l1id = b.' || quote_ident(n_pkey) || ')
    ORDER BY lid, locus
  ),
  -- we generate a row_number index column for each input line
  -- to be able to self-join the table to cut a line between two consecutive locations
  loc_with_idx AS (
    SELECT lid, locus, row_number() OVER (PARTITION BY lid ORDER BY locus) AS idx
    FROM cut_locations
  )
  -- finally, each original line is cut with consecutive locations using linear referencing functions
  SELECT l.' || quote_ident(n_pkey) || ', loc1.idx AS sub_id, ' || vst_line_substring || '(l.' || quote_ident(n_geom) || ', loc1.locus, loc2.locus) AS ' || quote_ident(n_geom) || '
  FROM loc_with_idx loc1 JOIN loc_with_idx loc2 USING (lid) JOIN ' || _pgr_quote_ident(intab) || ' l ON (l.' || quote_ident(n_pkey) || ' = loc1.lid)
  WHERE loc2.idx = loc1.idx+1
    -- keeps only linestring geometries
    AND geometryType(' || vst_line_substring || '(l.' || quote_ident(n_geom) || ', loc1.locus, loc2.locus)) = ''LINESTRING'') ';
  RAISE DEBUG  '%',p_ret;
  EXECUTE p_ret;
  GET DIAGNOSTICS splits = ROW_COUNT;
    EXECUTE 'WITH diff AS (SELECT DISTINCT old_id FROM '||_pgr_quote_ident(outtab)||' )
         SELECT count(*) FROM diff' INTO touched;
  -- here, it misses all original line that did not need to be cut by intersection points: these lines
  -- are already clean
  -- inserts them in the final result: all lines which gid is not in the res table.
  EXECUTE 'INSERT INTO ' || _pgr_quote_ident(outtab) || ' (old_id , sub_id, ' || quote_ident(n_geom) || ')
    ( WITH used AS (SELECT DISTINCT old_id FROM '|| _pgr_quote_ident(outtab)||')
    SELECT ' ||  quote_ident(n_pkey) || ', 1 AS sub_id, ' ||  quote_ident(n_geom) ||
    ' FROM '|| _pgr_quote_ident(intab) ||' WHERE  '||quote_ident(n_pkey)||' NOT IN (SELECT * FROM used)' || rows_where_out || ')';
  GET DIAGNOSTICS untouched = ROW_COUNT;

  RAISE NOTICE '  Split Edges: %', touched;
  RAISE NOTICE ' Untouched Edges: %', untouched;
  RAISE NOTICE '     Total original Edges: %', touched+untouched;
    RAISE NOTICE ' Edges generated: %', splits;
  RAISE NOTICE ' Untouched Edges: %',untouched;
  RAISE NOTICE '       Total New segments: %', splits+untouched;
    RAISE NOTICE ' New Table: %', outtab;
    RAISE NOTICE '----------------------------------';

  DROP TABLE IF EXISTS intergeom;
  DROP TABLE IF EXISTS inter_loc;
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
';
