CREATE OR REPLACE FUNCTION rc(link_table varchar, ow_table varchar, link integer,
                              cost varchar,
			      val float8,
                              m integer, d integer, h integer,
                              min integer, dow integer)
       RETURNS DOUBLE PRECISION AS
$$
DECLARE

rec record;
rc float8;
oneway integer;
query text;

BEGIN

 query := 'select t.gid, t.source, t.target, '||cost||' as cost, t.oneway_c as oneway_c,'||
                     ' o.from_month, o.from_day, o.to_month, o.to_day, o.from_hour,'||
		     ' o.from_min, o.to_hour, o.to_min, o.dayofweek, o.linkdir '||
		     ' from '||
		     quote_ident(link_table)||
		     ' t, '||quote_ident(ow_table)||' o where t.gid = o.linkid and '||
		     ' t.gid = '||link||
		     ' and '||m||' > o.from_month AND '||m||' < o.to_month AND '||
		     d||' > o.from_day AND '||d||' < o.to_day AND '||
		     h||' > o.from_hour AND '||h||' < o.to_hour AND '||
		     min||' > o.from_min AND '||min||' < o.to_min ';
		     
	             --TOFIX ' and ('||dow||' = 15 OR '||dow||' = o.dayofweek)'

  FOR rec IN EXECUTE query
  LOOP
    oneway := rec.oneway_c;
  END LOOP;
  
      --RAISE NOTICE 'oneway_c = %, cost = %', oneway, rec.cost;

      IF oneway IS NULL THEN rc := val; END IF;
      IF oneway = 3 THEN rc := 'Infinity'::float8; END IF;
      IF oneway = 4 THEN rc := val; END IF;
      IF oneway = 5 THEN
        IF rec.linkdir = 1 THEN rc := 'Infinity'::float8; END IF;
        IF rec.linkdir = 2 THEN rc := val; END IF;
      END IF;
      
  RETURN rc;

END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;



CREATE OR REPLACE FUNCTION c(link_table varchar, np_table varchar,
                              link integer,
                              cost varchar,
			      val float8,
                              m integer, d integer, h integer,
                              min integer, dow integer)
       RETURNS DOUBLE PRECISION AS
$$
DECLARE

rec record;
c float8;
cst float8;
nopass integer;

BEGIN

  FOR rec IN EXECUTE 'select t.gid, t.source, t.target, t.nopass_c,'||
                     ' o.from_month, o.from_day, o.to_month, o.to_day, o.from_hour,'||
		     ' o.from_min, o.to_hour, o.to_min, o.dayofweek '||
		     ' from '||
		     quote_ident(link_table)||
		     ' t, '||quote_ident(np_table)||' o where t.gid = o.linkid and '||
		     ' t.gid = '||link||
		     ' and '||m||' > o.from_month AND '||m||' < o.to_month AND '||
		     d||' > o.from_day AND '||d||' < o.to_day AND '||
		     h||' > o.from_hour AND '||h||' < o.to_hour AND '||
		     min||' > o.from_min AND '||min||' < o.to_min '
		     
	             --TOFIX ' and ('||dow||' = 15 OR '||dow||' = o.dayofweek)'

  LOOP

    nopass := rec.nopass_c;

  END LOOP;
  
      --RAISE NOTICE 'nopass_c = %', nopass;

      IF nopass IS NULL THEN c := val; END IF;
      IF nopass = 2 THEN c := val; END IF;
      IF nopass = 3 THEN c := 'Infinity'::float8; END IF;
      IF nopass = 4 THEN c := 'Infinity'::float8; END IF;
      IF nopass = 5 THEN c := val; END IF;

  RETURN c;

END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;

CREATE OR REPLACE FUNCTION ipc_shooting_star_route(
       varchar, int4, int4, float8)
RETURNS SETOF GEOMS AS
$$
DECLARE

geom_table ALIAS FOR $1;
sourceid ALIAS FOR $2;
targetid ALIAS FOR $3;
delta ALIAS FOR $4;
	      
rec record;
r record;
path_result record;
v_id integer;
e_id integer;
geom geoms;
														      
srid integer;
															      
source_x float8;
source_y float8;
target_x float8;
target_y float8;
																			      
ll_x float8;
ll_y float8;
ur_x float8;
ur_y float8;

query text;

m integer;
d integer;
h integer;
min integer;
dow integer;

inf float8;

BEGIN

  inf := 100000.0;
  
  FOR rec IN EXECUTE
    'select srid(the_geom) from ' ||
    quote_ident(geom_table) || ' limit 1'
  LOOP
  END LOOP;

  srid := rec.srid;
																															      
  FOR rec IN EXECUTE
    'select x(startpoint(the_geom)) as source_x from ' ||
    quote_ident(geom_table) || ' where gid = '||sourceid
  LOOP
  END LOOP;

  source_x := rec.source_x;

  FOR rec IN EXECUTE
    'select y(startpoint(the_geom)) as source_y from ' ||
    quote_ident(geom_table) || ' where gid = ' ||sourceid
  LOOP
  END LOOP;
																																											      
  source_y := rec.source_y;
																																													      
  FOR rec IN EXECUTE
    'select x(startpoint(the_geom)) as target_x from ' ||
    quote_ident(geom_table) || ' where gid = ' ||targetid
  LOOP
  END LOOP;
																																																			      
  target_x := rec.target_x;
																																																				      
  FOR rec IN EXECUTE
    'select y(startpoint(the_geom)) as target_y from ' ||
    quote_ident(geom_table) || ' where gid = ' ||targetid
  LOOP
  END LOOP;

  target_y := rec.target_y;
  
  FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_x||'<'||target_x||
                     ' THEN '||source_x||' ELSE '||target_x||
                     ' END as ll_x, CASE WHEN '||source_x||'>'||target_x||
                     ' THEN '||source_x||' ELSE '||target_x||' END as ur_x'
  LOOP
  END LOOP;

  ll_x := rec.ll_x;
  ur_x := rec.ur_x;

  FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_y||'<'||
                     target_y||' THEN '||source_y||' ELSE '||
                     target_y||' END as ll_y, CASE WHEN '||
                     source_y||'>'||target_y||' THEN '||
                     source_y||' ELSE '||target_y||' END as ur_y'
  LOOP
  END LOOP;
																																																																													       
  ll_y := rec.ll_y;
  ur_y := rec.ur_y;


  FOR rec IN EXECUTE 'select date_part(''month'', current_date)::integer as m, '||
           'date_part(''day'', current_date)::integer as d, '||
	   'date_part(''hour'', current_time)::integer as h, '||
	   'date_part(''minute'', current_time)::integer as min, '||
           'date_part(''dow'', current_date)::integer as dow'
  LOOP
  END LOOP;
  
  m := rec.m;
  d := rec.d;
  h := rec.h;
  min := rec.min;
  dow := rec.dow;


  FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' ||
           'shortest_path_shooting_star(''select gid as id, source::integer, '||
	   'target::integer, rule::varchar, x1::double precision, '||
	   'x2::double precision, y1::double precision, y2::double precision, '||
	   'nopass_c, '||
           'case when nopass_c=0 then cost::double precision ' ||
	   'when nopass_c=1 then '||inf||'::double precision '||
	   'else c(''''ipc_trn'''', ''''npc'''', gid, ''''cost'''', cost, '||
	   m||', '||d||', '||h||', '||min||', '||dow||') '||
	   'end as cost, '||
           'case when oneway_c=0 then cost::double precision '||
           'when oneway_c=1 then '||inf||'::double precision '||
           'when oneway_c=2 then cost::double precision '||
           'else rc(''''ipc_trn'''', ''''owc'''', gid, ''''cost'''', cost, '||
	   m||', '||d||', '||h||', '||min||', '||dow||') '||
           'end as reverse_cost, '||inf||'::double precision as to_cost '||

           'FROM ipc_trn where setSRID(''''BOX3D('||
           ll_x-delta||' '||ll_y-delta||','||ur_x+delta||' '||
           ur_y+delta||')''''::BOX3D, ' || srid || ') && the_geom order by gid'', ' ||
           quote_literal(sourceid) || ' , ' ||
           quote_literal(targetid) || ' , ''true'', ''true'' ),' ||
           quote_ident(geom_table) || ' where edge_id = gid '

  LOOP

    geom.gid      := path_result.gid;
    geom.the_geom := path_result.the_geom;
  
    RETURN NEXT geom;
																																																																																																								      
  END LOOP;

  RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;

CREATE OR REPLACE FUNCTION ipc_shooting_star_route_simple(
       varchar, int4, int4, float8)
RETURNS SETOF GEOMS AS
$$
DECLARE

geom_table ALIAS FOR $1;
sourceid ALIAS FOR $2;
targetid ALIAS FOR $3;
delta ALIAS FOR $4;
	      
rec record;
r record;
path_result record;
v_id integer;
e_id integer;
geom geoms;
														      
srid integer;
															      
source_x float8;
source_y float8;
target_x float8;
target_y float8;
																			      
ll_x float8;
ll_y float8;
ur_x float8;
ur_y float8;

query text;

m integer;
d integer;
h integer;
min integer;
dow integer;

inf float8;

BEGIN

  inf := 100000.0;
  
  FOR rec IN EXECUTE
    'select srid(the_geom) from ' ||
    quote_ident(geom_table) || ' limit 1'
  LOOP
  END LOOP;

  srid := rec.srid;
																															      
  FOR rec IN EXECUTE
    'select x(startpoint(the_geom)) as source_x from ' ||
    quote_ident(geom_table) || ' where gid = '||sourceid
  LOOP
  END LOOP;

  source_x := rec.source_x;

  FOR rec IN EXECUTE
    'select y(startpoint(the_geom)) as source_y from ' ||
    quote_ident(geom_table) || ' where gid = ' ||sourceid
  LOOP
  END LOOP;
																																											      
  source_y := rec.source_y;
																																													      
  FOR rec IN EXECUTE
    'select x(startpoint(the_geom)) as target_x from ' ||
    quote_ident(geom_table) || ' where gid = ' ||targetid
  LOOP
  END LOOP;
																																																			      
  target_x := rec.target_x;
																																																				      
  FOR rec IN EXECUTE
    'select y(startpoint(the_geom)) as target_y from ' ||
    quote_ident(geom_table) || ' where gid = ' ||targetid
  LOOP
  END LOOP;

  target_y := rec.target_y;
  
  FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_x||'<'||target_x||
                     ' THEN '||source_x||' ELSE '||target_x||
                     ' END as ll_x, CASE WHEN '||source_x||'>'||target_x||
                     ' THEN '||source_x||' ELSE '||target_x||' END as ur_x'
  LOOP
  END LOOP;

  ll_x := rec.ll_x;
  ur_x := rec.ur_x;

  FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_y||'<'||
                     target_y||' THEN '||source_y||' ELSE '||
                     target_y||' END as ll_y, CASE WHEN '||
                     source_y||'>'||target_y||' THEN '||
                     source_y||' ELSE '||target_y||' END as ur_y'
  LOOP
  END LOOP;
																																																																													       
  ll_y := rec.ll_y;
  ur_y := rec.ur_y;


  FOR rec IN EXECUTE 'select date_part(''month'', current_date)::integer as m, '||
           'date_part(''day'', current_date)::integer as d, '||
	   'date_part(''hour'', current_time)::integer as h, '||
	   'date_part(''minute'', current_time)::integer as min, '||
           'date_part(''dow'', current_date)::integer as dow'
  LOOP
  END LOOP;
  
  m := rec.m;
  d := rec.d;
  h := rec.h;
  min := rec.min;
  dow := rec.dow;


  FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' ||
           'shortest_path_shooting_star(''select gid as id, source::integer, '||
	   'target::integer, rule::varchar, x1::double precision, '||
	   'x2::double precision, y1::double precision, y2::double precision, '||
	   'nopass_c, '||
           'case when nopass_c=0 then cost::double precision ' ||
	   'when nopass_c=1 then '||inf||'::double precision '||
	   'when nopass_c=2 then cost::double precision '||
	   'when nopass_c=3 then '||inf||'::double precision '||
	   'when nopass_c=4 then '||inf||'::double precision '||
	   'when nopass_c=5 then '||inf||'::double precision '||
	   'end as cost, '||
           'case when oneway_c=0 then cost::double precision '||
           'when oneway_c=1 then '||inf||'::double precision '||
           'when oneway_c=2 then cost::double precision '||
           'when oneway_c=3 then '||inf||'::double precision '||
           'when oneway_c=4 then cost::double precision '||
           'end as reverse_cost, '||inf||'::double precision as to_cost '||

           'FROM ipc_trn where setSRID(''''BOX3D('||
           ll_x-delta||' '||ll_y-delta||','||ur_x+delta||' '||
           ur_y+delta||')''''::BOX3D, ' || srid || ') && the_geom order by gid'', ' ||
           quote_literal(sourceid) || ' , ' ||
           quote_literal(targetid) || ' , ''true'', ''true'' ),' ||
           quote_ident(geom_table) || ' where edge_id = gid '

  LOOP

    geom.gid      := path_result.gid;
    geom.the_geom := path_result.the_geom;
  
    RETURN NEXT geom;
																																																																																																								      
  END LOOP;

  RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;
