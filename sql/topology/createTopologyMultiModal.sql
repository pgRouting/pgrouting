/*PGR-GNU*****************************************************************

Copyright (c) 2019 pgRouting developers
Author: Manuel Fuentes Jiménez <m92fuentes@gmail.com>
Mail: project@pgrouting.org

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
drop type IF EXISTS  pgr_create_top_line_layers_type CASCADE;
create type pgr_create_top_line_layers_type as(
  id INTEGER,
  the_geom geometry,
  z_start FLOAT,
  z_end FLOAT
  );
drop type IF EXISTS pgr_create_top_points_type CASCADE;
create type pgr_create_top_points_type as(
  pos INTEGER,
  id INTEGER,
  layname char(128),
  r INTEGER,
  geom geometry
  );
drop type IF EXISTS pgr_create_top_points_layers_type CASCADE;
CREATE type pgr_create_top_points_layers_type as(
  id INTEGER,
  the_geom geometry, --Puntos 2d o 3d
  z FLOAT
  );
drop type IF EXISTS pgr_create_top_error_report CASCADE;
CREATE type pgr_create_top_error_report as(
  id INTEGER, --identificador de la geometria
  layname text, --nombre de la capa al q pertenece
  error text
  );
drop type IF EXISTS pgr_create_top_buildings_lines CASCADE;
create type pgr_create_top_buildings_lines as (
  geom geometry,
  source INT,
  target INT,
  id INTEGER
  );
CREATE OR REPLACE FUNCTION "pgr_polyfill_json_object_set_path"(
  "jsonb"          jsonb,
  "key_path"      TEXT[],
  "value_to_set"  anyelement
)
  RETURNS jsonb
  LANGUAGE sql
  IMMUTABLE
  STRICT
    AS
    $function$
      SELECT CASE COALESCE(array_length("key_path", 1), 0)
         WHEN 0 THEN to_json("value_to_set")::jsonb
         WHEN 1 THEN "pgr_polyfill_jsonb_object_set_key"("jsonb", "key_path"[l], "value_to_set")
         ELSE "pgr_polyfill_jsonb_object_set_key"(
             "jsonb",
             "key_path"[l],
             "pgr_polyfill_json_object_set_path"(
                 COALESCE(NULLIF(("jsonb" -> "key_path"[l])::text, 'null'), '{}')::jsonb,
                 "key_path"[l+1:u],
                 "value_to_set"
               )
           )
         END
      FROM array_lower("key_path", 1) l,
           array_upper("key_path", 1) u
    $function$;

CREATE OR REPLACE FUNCTION "pgr_polyfill_jsonb_object_set_key"(
  "jsonb"          jsonb,
  "key_to_set"    TEXT,
  "value_to_set"  anyelement
)
  RETURNS jsonb
  LANGUAGE sql
  IMMUTABLE
  STRICT
  AS
  $function$
    SELECT concat('{', string_agg(to_json("key") || ':' || "value"::text, ','), '}')::jsonb
    FROM (SELECT *
          FROM jsonb_each("jsonb")
          WHERE "key" <> "key_to_set"
          UNION ALL
          SELECT "key_to_set", to_json("value_to_set")::jsonb) AS "fields"
  $function$;

create or REPLACE FUNCTION pgr_polyfill_jsonb_set(p_jsonb jsonb, p_path text[], p_value jsonb)
  returns jsonb as
  $$
    BEGIN
      if pg_version_num() < 90500 then
        return pgr_polyfill_json_object_set_path(p_jsonb, p_path, p_value);
      else
        return jsonb_set(p_jsonb, p_path, p_value);
      end if;
    end;
  $$
language plpgsql;

--It is expected p_point to be a point or pointz and 4326
create or REPLACE FUNCTION pgr_create_topo_set_point(z_conn INTEGER, p_point geometry, p_z FLOAT,p_dims INTEGER)
  returns geometry AS
$$
DECLARE
  v_dim integer;
BEGIN

  IF z_conn = 2 THEN
    return ('SRID=4326;POINT('||st_x(p_point)|| ' '||st_y(p_point)||')');
  ELSEIF z_conn = 1 THEN
    if p_dims = 2 THEN
      return ('SRID=4326;POINT('||st_x(p_point)|| ' '||st_y(p_point)||')');
    END IF;
    return  ('SRID=4326;POINT(' || st_x(p_point)|| ' '|| st_y(p_point)|| ' ' || p_z || ')');
  ELSE
    v_dim:=st_ndims(p_point);
    if v_dim = 3 then
      return p_point;
    ELSE
      RETURN ('SRID=4326;POINT(' || st_x(p_point)|| ' '|| st_y(p_point)|| ' ' || 0 || ')');
    END IF;

  END IF;
END;
$$ LANGUAGE plpgsql;

create or REPLACE function pgr_create_topo_check_intersect(p_geom_layer geometry, p_point geometry, p_tolerance FLOAT) returns BOOLEAN AS
$$
DECLARE
  line geometry;
BEGIN

  line = pgr_multiline_to_linestring(p_geom_layer, p_tolerance, TRUE );
  IF line is null THEN
    return FALSE;
  END IF;
  return st_3ddwithin(line, p_point, p_tolerance);
END;
$$ LANGUAGE plpgsql;

--It is expected p_l to be multilinestring or multilinestringZ
create or REPLACE function pgr_multiline_to_linestring(p_l geometry, p_tolerance float, p_drop_z BOOLEAN)
  returns geometry as
$$
DECLARE
  v_p geometry;
  v_p_aux geometry;
  v_line geometry;
  v_l_c int;
  v_p_c int;
  v_p_array geometry[] default '{}';
BEGIN
  v_l_c := st_numgeometries(p_l);
  for v_l_i in 1..v_l_c LOOP
    v_line :=st_geometryn(p_l, v_l_i);
    v_p_c := st_numpoints(v_line);
    for v_p_i in 1..v_p_c LOOP
      v_p_aux = st_pointn(v_line,v_p_i);
      if p_drop_z THEN
        v_p_aux := pgr_create_topo_set_point(2,v_p_aux,0,0);
      END IF;

      --if this point and the previous one are the same.
      if(st_3ddwithin(v_p,v_p_aux,p_tolerance))THEN
        --I ignore it so there aren't duplicate points.
        CONTINUE;
      end if;
      --If this geometry contains more than one linestring
      if( v_p_i = 1 and v_p is not NULL ) THEN
        --And the firs point of this linestring is not the same as the last in the previous linestring
        --these linestring arent connected, so there is an error.
        return null;
      END IF;
      v_p := v_p_aux;
      v_p_array := v_p_array || v_p;
    END LOOP;
  END LOOP;
  return st_makeLine(v_p_array);

END;
$$ LANGUAGE plpgsql;

create or REPLACE function pgr_create_top_get_point_dims(p_point_pos INTEGER, p_geom_dims INTEGER, p_zconn INTEGER)
  returns INTEGER as $$
BEGIN
  return CASE when (p_zconn = 1 and (p_point_pos = 1 or p_point_pos = 2)) or p_zconn = 0 then 3 WHEN p_zconn = 2 then 2 else p_geom_dims END;
END;
$$ LANGUAGE plpgsql;


create or REPLACE function pgr_createtopology_layers (p_lineal_groups jsonb, p_puntual_groups jsonb, p_layers jsonb,
                                                                        p_graph_lines_table text, p_graph_lines_schema text, p_tolerance FLOAT)
  returns setof pgr_create_top_error_report AS
$$
DECLARE
  v_lineal_group_record record;
  v_lineal_layer text;  --Layer being analized
  v_line pgr_create_top_line_layers_type ; --Data type returned by sql from line layers
  v_point_type pgr_create_top_points_layers_type; --Data type returned by sql from point layers
  v_pconn integer;   --Layer's connectivity politicy
  v_zconn INTEGER;   --Z connectivity
  v_z_value FLOAT;   --Z value when geometry'z is not used
  v_point geometry;  --Point to insert
  v_point_intersected pgr_create_top_points_type; -- row del punto que tuvo interseccion con el actual
  v_r INTEGER;        --representante del actual punto, es un id.
  v_n_points INTEGER; --cantidad de puntos en la geometria que se esta analizando
  v_point_pos INTEGER; --posicion del punto en su geom, 1-inicio 2-fin, 3-medio
  v_r_point_id INTEGER default 1; --id del proximo punto representante a insertar
  v_group INTEGER DEFAULT 0;
  v_keyvalue record;
  v_r_table_name text;
  v_lines_table_name text;
  v_p_groups INTEGER[];
  v_source INTEGER;
  v_target INTEGER;
  v_pos INTEGER;
  v_line_intersected pgr_create_top_buildings_lines;
  v_points_make_line geometry[];
  v_line_point geometry;
  v_first BOOLEAN;
  v_geom_dims integer;
  v_point_dims INTEGER; --Point's dimensions, differs from its layer dimensions according to zconn

BEGIN


  /*
    Do all the verification process
    TOdo
  */


  --Table where points needed to build the graph are stored.
  drop table if EXISTS pgr_create_top_graph_ptos;
  CREATE TEMPORARY TABLE pgr_create_top_graph_ptos(
     pos INTEGER, --Point position inside its geometry, 1=start, 2=end, 3=middle.
     id INTEGER,  --Geometry's id where this point belongs to.
     layname char(128), --Layer's name where this point belongs to.
     r INTEGER,   --Representative' id of this point in the graph, it is null in case it doesn't belongs to the graph.FK to v_r_table_name.id
     geom geometry,
     g INTEGER,   --Layer's groups where this point belongs to
     dims INTEGER --Dimensions of this point
  );
  create index on pgr_create_top_graph_ptos(id);
  create index on pgr_create_top_graph_ptos using gist(geom);

  v_r_table_name := quote_ident(p_graph_lines_schema)||'.'||quote_ident(p_graph_lines_table || '_pt');
  v_lines_table_name := quote_ident(p_graph_lines_schema)||'.'||quote_ident(p_graph_lines_table);

  --Table of Representative points.
  EXECUTE 'drop table if exists '|| v_r_table_name;
  EXECUTE 'create table '|| v_r_table_name  ||
          '(id integer primary key, ' ||
          'layname text,' ||
          'id_geom integer,' ||
          'geom geometry)';

  -- Table of graph's edges
  EXECUTE 'drop table if exists '|| v_lines_table_name;
  EXECUTE 'create table '|| v_lines_table_name ||
    --It is linestring because of if there is a layer with multilinestring geometries,
    --in order to them being valid, they must be convertable to linestring completely
          '(geom geometry,' ||
          'source integer,' ||
          'target integer,' ||
          'id_geom integer,' || --id of the geometry that contains this edge
          'layname text,' ||     --name of the layer that has the geometry containing this edge
          'id serial primary key)';
  EXECUTE 'create index on '|| v_lines_table_name||' (id_geom)';



  --Analazing lineal layers
  <<lineal_groups_label>>
  for v_lineal_group_record in SELECT * from jsonb_each(p_lineal_groups)as g LOOP
    v_group = v_group +1;

    <<lineal_layer_label>>
    for v_lineal_layer in SELECT * from jsonb_array_elements_text(v_lineal_group_record.value) LOOP
      v_pconn := p_layers->v_lineal_layer->>'pconn';
      v_zconn := p_layers->v_lineal_layer->>'zconn';

      p_layers := pgr_polyfill_jsonb_set(p_layers,('{'||v_lineal_layer||', group}')::text[], to_json(v_group)::jsonb);

      v_first := true;
      FOR v_line in EXECUTE (p_layers->v_lineal_layer->>'sql') loop
        --Adding points

        --This is needed in order to drop duplicates points in a linestring or multilinestring.
        v_line.the_geom := pgr_multiline_to_linestring(v_line.the_geom,p_tolerance, FALSE );
        if(v_line.the_geom is NULL ) THEN
          return next (v_line.id,v_lineal_layer,'Invalid MultiLinestring. A valid multilinestring is the one conformed by lines connected like they were a single line chopped. Geom wasnt used.' )::pgr_create_top_error_report;
          continue;
        END IF;

        if v_first THEN
          v_geom_dims := st_ndims(v_line.the_geom);
          p_layers := pgr_polyfill_jsonb_set(p_layers,('{'||v_lineal_layer||',dims}')::text[], to_json(v_geom_dims)::jsonb);
          v_first := FALSE;
        END IF;

        v_n_points := st_npoints(v_line.the_geom);
        FOR v_i in 1..(case when v_pconn = 0 then 2 else v_n_points end) LOOP

          IF v_pconn = 0  THEN
            if v_i = 1 THEN
              v_i := 1;
            else
              v_i :=v_n_points;
            END IF;
          END IF;
          case v_i
            when 1          then v_point_pos := 1;
            v_z_value   := v_line.z_start;
            when v_n_points then v_point_pos := 2;
            v_z_value   := v_line.z_end;
            ELSE                 v_point_pos := 3;
            v_z_value   := 0;
            END CASE;

          v_point_dims := pgr_create_top_get_point_dims(v_point_pos, v_geom_dims, v_zconn) ;

          v_point = st_pointn(v_line.the_geom,v_i );
          v_point = pgr_create_topo_set_point(v_zconn, v_point, v_z_value,v_point_dims);
          select into v_point_intersected *
          from pgr_create_top_graph_ptos as g
          WHERE st_dwithin(v_point,g.geom,p_tolerance) and --to use index
            st_3ddwithin(v_point, g.geom, p_tolerance) and g = v_group and dims = v_point_dims --a point with 3d never can be represent by a point in 2d and viceversa
          limit 1;
          --an intermediate point which no intersects with any other point must be inserted with r =null
          --an intermediate point with r = null means it might not be a point in the graph, unless some other point intersects with it.
          --if some point intersects with an intermediate point both of them must have same r
          --the set of point that intersects must have same r
          v_r := NULL;
          if v_point_intersected.geom is not null and v_point_intersected.r is not NULL then
            v_r :=v_point_intersected.r;
          ELSEIF v_point_pos != 3 or (v_point_intersected.geom is not null) THEN
            --create a new representant.
            v_r := v_r_point_id;
            IF v_point_intersected.r is NULL THEN
              UPDATE pgr_create_top_graph_ptos set r = v_r
              where id = v_point_intersected.id --use index
                and st_astext(geom) = st_astext(v_point_intersected.geom);
            END IF;
            EXECUTE 'insert into '|| v_r_table_name ||
                    ' values($1,null,null, $2)' using v_r, v_point;
            v_r_point_id := v_r_point_id + 1;
            /*  ELSEIF v_point_pos = 3 THEN
                v_r := null;   */
          END IF;

          insert into pgr_create_top_graph_ptos values (v_point_pos,v_line.id, v_lineal_layer, v_r, v_point, v_group,v_point_dims);
        END LOOP;



      END LOOP;
    END LOOP lineal_layer_label;

  END loop lineal_groups_label;

  --Analazing point layers
  if p_puntual_groups is not NULL THEN
    FOR v_keyvalue in SELECT * from jsonb_each(p_puntual_groups) loop
      --Add non-inserted points from layers where connectivity policy was overwritten
      v_p_groups = '{}';
      for v_lineal_layer in SELECT v.value from  jsonb_array_elements_text(v_keyvalue.value) as v LOOP

        v_group := p_layers->v_lineal_layer->>'group';
        v_p_groups := v_p_groups || v_group;

        if p_layers->(v_keyvalue.key)->>'pconn' = '1' and
           p_layers->v_lineal_layer->>'pconn' = '0' THEN

          --after this iteration intermediate points where inserted
          --so this is to avoid duplicate insertings
          p_layers := pgr_polyfill_jsonb_set(p_layers, (v_lineal_layer || '{pconn}'::text[]), '1'::jsonb );
          v_zconn := p_layers->v_lineal_layer->>'zconn';
          v_geom_dims := p_layers->v_lineal_layer->>'dims';


          for v_line in EXECUTE p_layers->v_lineal_layer->>'sql' LOOP
            v_line.the_geom := pgr_multiline_to_linestring(v_line.the_geom,p_tolerance, FALSE );
            if(v_line.the_geom is NULL ) THEN --I havent done anything with this line, it wasnt processed because of topological errors.
              continue;
            END IF;
            v_n_points := st_npoints(v_line.the_geom);
            FOR v_i in 2..(v_n_points-1) LOOP
              v_point = st_pointn(v_line.the_geom,v_i );
              v_point = pgr_create_topo_set_point(v_zconn, v_point, 0, v_geom_dims);
              v_point_dims := pgr_create_top_get_point_dims(3, v_geom_dims, v_zconn);

              insert into pgr_create_top_graph_ptos values (3,v_line.id,v_lineal_layer,null,v_point,v_group, v_point_dims);
            END LOOP;
          END LOOP;

        END IF;
      END LOOP;
      --Intersect points from point layers with the inserted ones
      v_zconn := p_layers->(v_keyvalue.key)->>'zconn';
      v_lineal_layer := null;
      v_first :=TRUE;
      FOR v_point_type in EXECUTE p_layers->(v_keyvalue.key)->>'sql' LOOP

        v_point := pgr_create_topo_set_point(v_zconn, v_point_type.the_geom,v_point_type.z, 0);
        if v_first THEN
          --As points from point layers are always converted v_geom_dims = v_point_dims
          v_geom_dims := st_ndims(v_point);
          p_layers = pgr_polyfill_jsonb_set(p_layers,('{'||v_keyvalue.key||', dims}')::text[], to_json(v_geom_dims)::jsonb);
          v_first := FALSE;
        END IF;
        --There may exists multiple points that intersects in the same group, all with equal r, some with same r and the other with null, or all with r = null
        SELECT into v_point_intersected *
        from pgr_create_top_graph_ptos as g
        where st_dwithin(g.geom, v_point, p_tolerance) and --to use index
          st_3ddwithin(g.geom, v_point, p_tolerance) and
          (v_p_groups @> (g.g || ARRAY []::int[])) and dims = v_geom_dims
        ORDER BY r NULLS LAST --Because I order by r with nulls last the first value must be an assigned one if there is one
        limit 1;
        if v_point_intersected.geom is NULL then
          return next (v_point_type.id, v_keyvalue.key,'El punto no intersecta a ningun otro punto del grafo')::pgr_create_top_error_report;
          CONTINUE ;
        END IF;

        EXECUTE 'select layname from ' || v_r_table_name|| ' where id=$1'into v_lineal_layer using v_point_intersected.r;

        if v_lineal_layer is not null THEN
          return next (v_point_type.id, v_keyvalue.key,'El punto se intersecta con otro punto de otra capa puntual en el mismo grupo, no se sabe por cual de los 2 hacer la union.')::pgr_create_top_error_report;
          CONTINUE;
        END IF;

        if v_point_intersected.r is NULL THEN
          EXECUTE 'insert into '||v_r_table_name||' values($1,$2,$3,$4) '
            using v_r_point_id, v_keyvalue.key, v_point_type.id, v_point;
          v_r := v_r_point_id;
          v_r_point_id := v_r_point_id+1;
        else
          v_r = v_point_intersected.r;
          EXECUTE 'update '||v_r_table_name|| ' set layname=$1, id_geom=$2 where id=$3' using v_keyvalue.key,v_point_type.id,v_r;
        END IF;

        UPDATE pgr_create_top_graph_ptos as g set r = v_r where st_dwithin(g.geom, v_point, p_tolerance) and --to use index
          st_3ddwithin(g.geom, v_point, p_tolerance) and
          (v_p_groups @> (g.g || ARRAY []::int[])) and dims = v_geom_dims;

      END LOOP;

    END LOOP;
  END IF;

  --Chopping lines
  for v_keyvalue in select * from jsonb_each(p_layers) LOOP


    if p_layers->(v_keyvalue.key)->'group' ISNULL then
      CONTINUE ;
    END IF;

    v_zconn := p_layers->(v_keyvalue.key)->>'zconn';
    v_geom_dims := p_layers->(v_keyvalue.key)->>'dims';
    v_first := TRUE ;
    for v_line in EXECUTE p_layers->(v_keyvalue.key)->>'sql' LOOP


      v_line.the_geom := pgr_multiline_to_linestring(v_line.the_geom, p_tolerance, (v_geom_dims = 3 and v_zconn = 2));
      if(v_line.the_geom is NULL ) THEN --I havent done anything with this line, it wasnt processed because of topological errors.
        continue;
      END IF;


      --For every line in a layer there is only one start point and end point, so I can do this.
      for v_pos, v_r in SELECT pos,r from pgr_create_top_graph_ptos where id = v_line.id and layname = v_keyvalue.key and (pos=1 or pos =2) LOOP
        if v_pos = 1 THEN
          v_source = v_r;
        ELSEIF v_pos = 2 THEN
          v_target = v_r;
        END IF;
      END LOOP;
      --Insert first line with its source and target values.
      EXECUTE 'INSERT into '||v_lines_table_name||' VALUES ($1,$2,$3,$4,$5)'using v_line.the_geom, v_source, v_target,v_line.id,v_keyvalue.key;

      --      Points are not added so this is not necessary
      --         --if geometries are 2d and z is gotten from query, intermediate points will not have z value, so lines can't be chopped
      --         if v_geom_dims = 2  and v_zconn = 1 THEN
      --           CONTINUE;
      --         END IF;

      --For each intermediate point of this line having a representant
      for v_r,v_point in select geometries.r, geometries.geom
                         from  (SELECT r,geom from pgr_create_top_graph_ptos where id = v_line.id and
                            layname = v_keyvalue.key and
                            pos = 3 and  r is not null) as geometries
                         inner join (select * from st_dumppoints(v_line.the_geom)) as points on -- It's needed that points are
                           (st_3ddwithin(points.geom,geometries.geom,p_tolerance))     -- in order of line drawn
                         order by points.path
        LOOP

        if (v_zconn = 2 and v_geom_dims = 3)  THEN
          EXECUTE 'SELECT  geom,source,target,id from '||v_lines_table_name ||
                  ' where id_geom = $1 and layname = $2 and pgr_create_topo_check_intersect("geom",$3,$4)'
            into v_line_intersected using v_line.id, v_keyvalue.key,st_buffer(v_point, p_tolerance),p_tolerance;
        ELSE
          EXECUTE 'SELECT  geom,source,target,id from '||v_lines_table_name ||
                  ' where id_geom = $1 and layname = $2 and st_3ddwithin(geom, $3,$4)'
            into v_line_intersected using v_line.id, v_keyvalue.key,v_point, p_tolerance  ;
        END IF;
        v_points_make_line := '{}';
        --This always has to execute because if there is a representative point, there is a line that contains it
        FOR v_line_point in SELECT geom from st_dumppoints(v_line_intersected.geom) ORDER BY path LOOP
          --As each point is an intermediate point it is going to have the same dimension as the line that contains it
          if v_zconn = 2 and v_geom_dims = 3 THEN
            v_line_point := pgr_create_topo_set_point(2,v_line_point,0,0);
          END IF;
          if st_astext(v_line_point) = st_astext(v_point) THEN
            v_points_make_line := v_points_make_line || v_line_point;
            EXECUTE 'insert into '||v_lines_table_name||' values ($1,$2,$3,$4,$5)' USING st_makeline(v_points_make_line),v_source,v_r,v_line.id,v_keyvalue.key;
            v_points_make_line :='{}';
            v_source := v_r;
          END IF;
          v_points_make_line := v_points_make_line || v_line_point;

        END LOOP;

        --this will never insert garbage because of if there is an intermediate point that has a representative point
        --there is a line who contains it and therefore it will be chopped just over that point in this iteration, so the
        --final line is going to have that point as the first point and the last point from v_line_intersected's last point
        EXECUTE 'insert into '||v_lines_table_name ||' values($1,$2,$3,$4,$5)' USING st_makeline(v_points_make_line), v_r, v_target,v_line.id,v_keyvalue.key;
        EXECUTE 'DELETE From '||v_lines_table_name ||' where id = $1' USING v_line_intersected.id;

      END LOOP;
    END LOOP;
  END LOOP;

END

$$ LANGUAGE plpgsql;