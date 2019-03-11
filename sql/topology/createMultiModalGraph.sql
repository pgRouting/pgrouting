
CREATE OR REPLACE FUNCTION "pgr_polyfill_jsonb_object_set_key"(
  "jsonb"          JSONB,
  "key_to_set"    TEXT,
  "value_to_set"  JSONB
)
  RETURNS JSONB
  LANGUAGE SQL
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

CREATE OR REPLACE FUNCTION "pgr_polyfill_json_object_set_path"(
  "jsonb"          JSONB,
  "key_path"      TEXT[],
  "value_to_set"  JSONB
)
  RETURNS JSONB
  LANGUAGE SQL
  IMMUTABLE
  STRICT
AS
$function$
SELECT CASE COALESCE(array_length("key_path", 1), 0)
         WHEN 0 THEN "value_to_set"
         WHEN 1 THEN "pgr_polyfill_jsonb_object_set_key"("jsonb", "key_path"[l], "value_to_set")
         ELSE "pgr_polyfill_jsonb_object_set_key"(
             "jsonb",
             "key_path"[l],
             "pgr_polyfill_json_object_set_path"(
                 COALESCE(NULLIF(("jsonb" -> "key_path"[l])::TEXT, 'NULL'), '{}')::jsonb,
                 "key_path"[l+1:u],
                 "value_to_set"
               )
           )
         END
FROM array_lower("key_path", 1) l,
     array_upper("key_path", 1) u
$function$;

CREATE OR REPLACE FUNCTION pgr_polyfill_jsonb_set(p_jsonb JSONB, p_path TEXT[], p_value JSONB)
  RETURNS JSONB AS
$$
BEGIN
  IF current_setting('server_version_num')::INTEGER < 90500 THEN
    RETURN pgr_polyfill_json_object_set_path(p_jsonb, p_path, p_value);
  ELSE
    RETURN jsonb_set(p_jsonb, p_path, p_value);
  END IF;
END;
$$
  language plpgsql;

CREATE OR REPLACE FUNCTION "pgr_polyfill_jsonb_object_insert_key"(
  "jsonb"          JSONB,
  "key_to_set"    TEXT,
  "value_to_set"  JSONB
)
  RETURNS JSONB
  LANGUAGE SQL
  IMMUTABLE
  STRICT
AS
$function$
SELECT CASE jsonb_typeof("jsonb")
         WHEN 'object' THEN
           (SELECT concat('{', string_agg(to_json("key") || ':' || "value"::TEXT, ','), '}')::jsonb
            FROM (SELECT *
                  FROM jsonb_each("jsonb")
                  WHERE "key" <> "key_to_set"
                  UNION ALL
                  SELECT "key_to_set", to_json("value_to_set")::jsonb) AS "fields"
           )
         WHEN 'array' THEN
           (SELECT concat('[',regexp_replace("jsonb"#>>'{}', '[\[\]]', '', 'g'),',',"value_to_set" #>> '{}',']')::jsonb)
         END
$function$;

CREATE OR REPLACE FUNCTION "pgr_polyfill_json_object_insert_path"(
  "jsonb"          JSONB,
  "key_path"      TEXT[],
  "value_to_set"  JSONB
)
  RETURNS JSONB
  LANGUAGE SQL
  IMMUTABLE
  STRICT
AS
$function$
SELECT CASE COALESCE(array_length("key_path", 1), 0)
         WHEN 0 THEN "value_to_set"
         WHEN 1 THEN "pgr_polyfill_jsonb_object_insert_key"("jsonb", "key_path"[l], "value_to_set")
         ELSE "pgr_polyfill_jsonb_object_insert_key"(
             "jsonb",
             "key_path"[l],
             "pgr_polyfill_json_object_insert_path"(
                 COALESCE(NULLIF(("jsonb" -> "key_path"[l])::TEXT, 'NULL'), '{}')::jsonb,
                 "key_path"[l+1:u],
                 "value_to_set"
               )
           )
         END
FROM array_lower("key_path", 1) l,
     array_upper("key_path", 1) u
$function$;


CREATE OR REPLACE FUNCTION pgr_polyfill_jsonb_insert(p_jsonb JSONB, p_path TEXT[], p_value JSONB)
  RETURNS JSONB AS
$$
BEGIN
  IF current_setting('server_version_num')::INTEGER   < 90500 THEN
    RETURN pgr_polyfill_json_object_insert_path(p_jsonb, p_path, p_value);
  ELSE
    RETURN JSONB_insert(p_jsonb, p_path, p_value);
  END IF;
END;
$$
  language plpgsql;
--It is expected p_point to be a point or pointz and 4326
CREATE OR REPLACE FUNCTION pgr_create_topo_set_point(z_conn INTEGER, p_point GEOMETRY, p_z FLOAT,p_dims INTEGER)
  RETURNS GEOMETRY AS
$$
DECLARE
  v_dim INTEGER;
BEGIN

  IF z_conn = 2 THEN
    RETURN ('SRID=4326;POINT('||st_x(p_point)|| ' '||st_y(p_point)||')');
  ELSEIF z_conn = 1 THEN
    IF p_dims = 2 THEN
      RETURN ('SRID=4326;POINT('||st_x(p_point)|| ' '||st_y(p_point)||')');
    END IF;
    RETURN  ('SRID=4326;POINT(' || st_x(p_point)|| ' '|| st_y(p_point)|| ' ' || p_z || ')');
  ELSE
    v_dim:=st_ndims(p_point);
    IF v_dim = 3 THEN
      RETURN p_point;
    ELSE
      RETURN ('SRID=4326;POINT(' || st_x(p_point)|| ' '|| st_y(p_point)|| ' ' || 0 || ')');
    END IF;

  END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION pgr_create_topo_check_intersect(p_geom_layer GEOMETRY, p_point GEOMETRY, p_tolerance FLOAT) RETURNS BOOLEAN AS
$$
DECLARE
  line GEOMETRY;
BEGIN

  line = pgr_multiline_to_linestring(p_geom_layer, p_tolerance, TRUE );
  IF line IS NULL THEN
    RETURN FALSE;
  END IF;
  RETURN st_3ddwithin(line, p_point, p_tolerance);
END;
$$ LANGUAGE plpgsql;

--It is expected p_l to be multilinestring or multilinestringZ
CREATE OR REPLACE FUNCTION pgr_multiline_to_linestring(p_l GEOMETRY, p_tolerance FLOAT, p_drop_z BOOLEAN)
  RETURNS GEOMETRY AS
$$
DECLARE
  v_p GEOMETRY;
  v_p_aux GEOMETRY;
  v_line GEOMETRY;
  v_l_c INT;
  v_p_c INT;
  v_p_array GEOMETRY[] default '{}';
BEGIN
  v_l_c := st_numgeometries(p_l);
  FOR v_l_i IN 1..v_l_c LOOP
    v_line :=st_geometryn(p_l, v_l_i);
    v_p_c := st_numpoints(v_line);
    FOR v_p_i IN 1..v_p_c LOOP
      v_p_aux = st_pointn(v_line,v_p_i);
      IF p_drop_z THEN
        v_p_aux := pgr_create_topo_set_point(2,v_p_aux,0,0);
      END IF;

      --if this point and the previous one are the same.
      IF(st_3ddwithin(v_p,v_p_aux,p_tolerance))THEN
        --I ignore it so there aren't duplicate points.
        CONTINUE;
      END IF;
      --If this geometry contains more than one linestring
      IF( v_p_i = 1 AND v_p IS NOT NULL ) THEN
        --And the firs point of this linestring is not the same as the last in the previous linestring
        --these linestring arent connected, so there is an error.
        RETURN NULL;
      END IF;
      v_p := v_p_aux;
      v_p_array := v_p_array || v_p;
    END LOOP;
  END LOOP;
  v_line = st_makeLine(v_p_array);
  IF st_npoints(v_line) < 2 THEN
    RETURN NULL;
  END IF;
  RETURN  v_line;

END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION pgr_create_top_get_point_dims(p_point_pos INTEGER, p_geom_dims INTEGER, p_zconn INTEGER)
  RETURNS INTEGER AS $$
BEGIN
  RETURN CASE WHEN (p_zconn = 1 AND (p_point_pos = 1 OR p_point_pos = 2)) OR p_zconn = 0 THEN 3 WHEN p_zconn = 2 THEN 2 ELSE p_geom_dims END;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION pgr_create_multimodal_graph (p_lineal_groups JSONB, p_puntual_groups JSONB, p_layers JSONB,
                                                          p_graph_lines_table TEXT, p_graph_lines_schema TEXT, p_tolerance FLOAT,
                                                          OUT ip_out_d BIGINT, OUT p_out_layname TEXT, OUT p_out_error TEXT)
  RETURNS SETOF RECORD AS
$$
DECLARE
  v_lineal_group_record RECORD;
  v_lineal_layer TEXT;  --Layer being analized
  v_point_layer_name TEXT; --Point layer name being analized
  v_group_layer_name_A TEXT; --Lineal layer name to join groups
  v_group_layer_name_B TEXT; --Lisdfsdfneal layer name to join groups
  v_group_r_A BIGINT; -- Representative id of Layer A
  v_group_r_B BIGINT; -- Representative id of Layer B
  --current line layer--------------------------------------------------------------------------------
  v_current_line_layer_id BIGINT; --identifier of current line from line_layer
  v_current_line_layer_the_geom GEOMETRY ; --geom of current line from line_layer
  v_current_line_layer_z_start FLOAT ; --z_start of current line from line_layer
  v_current_line_layer_z_end FLOAT; --z_end of current line from line_layer
  ----------------------------------------------------------------------------------------------------
  --current point layer-------------------------------------------------------------------------------
  v_point_id BIGINT;
  v_point_the_geom GEOMETRY;
  v_point_z FLOAT;
  ----------------------------------------------------------------------------------------------------
  v_pconn INTEGER;   --Layer's connectivity politicy
  v_zconn INTEGER;   --Z connectivity
  v_z_value FLOAT;   --Z value when geometry'z is not used
  v_point GEOMETRY;  --Point to insert
  --representative_point_layer------------------------------------------------------------------------
  v_r_id BIGINT;
  v_r_geom GEOMETRY;
  v_r_r INTEGER;

  ----------------------------------------------------------------------------------------------------
  v_r INTEGER;        --representante del actual punto, es un id.
  v_n_points INTEGER; --cantidad de puntos en la geometria que se esta analizando
  v_point_pos INTEGER; --posicion del punto en su geom, 1-inicio 2-fin, 3-medio
  v_r_point_id BIGINT default 1; --id del proximo punto representante a insertar
  v_group INTEGER DEFAULT -1;
  v_keyvalue RECORD;
  v_r_table_name TEXT; --table of edges joints
  v_lines_table_name TEXT; --table of edges
  v_p_groups INTEGER[];
  v_source BIGINT;
  v_target BIGINT;
  v_pos INTEGER;
  --current line intersecting by intermediate point---------------------------------------------------
  v_intersected_geom GEOMETRY;
  v_intersected_id  BIGINT;
  ----------------------------------------------------------------------------------------------------
  v_points_make_line GEOMETRY[];
  v_line_point GEOMETRY;
  v_first BOOLEAN;
  v_geom_dims INTEGER;
  v_point_dims INTEGER; --Point's dimensions, differs from its layer dimensions according to zconn

BEGIN


  /*
    Do all the verification process
    TOdo
  */


  --Table where points needed to build the graph are stored.
  DROP TABLE IF EXISTS pgr_create_top_graph_ptos;
  CREATE TEMPORARY TABLE pgr_create_top_graph_ptos(
                                                    pos INTEGER, --Point position inside its geometry, 1=start, 2=end, 3=middle.
                                                    id BIGINT,  --GEOMETRY's id where this point belongs to.
                                                    layname CHAR(128), --Layer's name where this point belongs to.
                                                    r INTEGER,   --Representative' id of this point in the graph, it is null in case it doesn't belongs to the graph.FK to v_r_table_name.id
                                                    geom geometry,
                                                    g INTEGER,   --Layer's groups where this point belongs to
                                                    dims INTEGER --Dimensions of this point
  );
  CREATE INDEX ON pgr_create_top_graph_ptos(id);
  CREATE INDEX ON pgr_create_top_graph_ptos USING gist(geom);

  v_r_table_name := quote_ident(p_graph_lines_schema)||'.'||quote_ident(p_graph_lines_table || '_pt');
  v_lines_table_name := quote_ident(p_graph_lines_schema)||'.'||quote_ident(p_graph_lines_table);

  --Table of Representative points.
  EXECUTE 'DROP TABLE IF exists '|| v_r_table_name;
  EXECUTE 'create table '|| v_r_table_name  ||
          '(id BIGINT primary key, ' ||
          'layname TEXT,' ||
          'id_geom BIGINT,' ||
          'geom GEOMETRY,' ||
          'group_id INT' ||
          ')';

  -- Table of graph's edges
  EXECUTE 'DROP TABLE IF exists '|| v_lines_table_name;
  EXECUTE 'create table '|| v_lines_table_name ||
    --It is linestring because of if there is a layer with multilinestring geometries,
    --in order to them being valid, they must be convertible to linestring completely
          '(geom GEOMETRY,' ||
          'source BIGINT,' ||
          'target BIGINT,' ||
          'id_geom BIGINT,' || --id of the geometry that contains this edge
          'layname TEXT,' ||     --name of the layer that has the geometry containing this edge
          'bounded_layers TEXT[] default null,'||--ids of layers being joined
          'group_id INT,' ||   --tells which group this edge belongs to
          'id serial primary key)';
  EXECUTE 'CREATE INDEX ON '|| v_lines_table_name||' (id_geom)';



  --Analazing lineal layers
  <<lineal_groups_label>>
  FOR v_lineal_group_record IN SELECT * FROM jsonb_each(p_lineal_groups) AS g LOOP
    v_group = v_group +1;

    <<lineal_layer_label>>
    FOR v_lineal_layer IN SELECT * FROM jsonb_array_elements_text(v_lineal_group_record.value) LOOP
      v_pconn := p_layers->v_lineal_layer->>'pconn';
      v_zconn := p_layers->v_lineal_layer->>'zconn';

      p_layers := pgr_polyfill_jsonb_set(p_layers,('{'||v_lineal_layer||', group}')::TEXT[], to_json(v_group)::jsonb);

      v_first := TRUE;
      FOR v_current_line_layer_id, v_current_line_layer_the_geom, v_current_line_layer_z_start, v_current_line_layer_z_end in
        EXECUTE (p_layers->v_lineal_layer->>'sql') LOOP
        --Adding points

        --This is needed in order to drop duplicates points in a linestring or multilinestring.
        v_current_line_layer_the_geom := pgr_multiline_to_linestring(v_current_line_layer_the_geom,p_tolerance, FALSE );
        IF(v_current_line_layer_the_geom IS NULL ) THEN
          RETURN QUERY SELECT v_current_line_layer_id::BIGINT, v_lineal_layer::TEXT ,'Invalid MultiLinestring. A valid multilinestring is the one conformed by lines connected like they were a single line chopped. Geom wasnt used.'::TEXT;
          CONTINUE;
        END IF;

        IF v_first THEN
          v_geom_dims := st_ndims(v_current_line_layer_the_geom);
          p_layers := pgr_polyfill_jsonb_set(p_layers,('{'||v_lineal_layer||',dims}')::TEXT[], to_json(v_geom_dims)::jsonb);
          v_first := FALSE;
        END IF;

        v_n_points := st_npoints(v_current_line_layer_the_geom);
        FOR v_i IN 1..(CASE WHEN v_pconn = 0 THEN 2 ELSE v_n_points END) LOOP

          IF v_pconn = 0  THEN
            IF v_i = 1 THEN
              v_i := 1;
            ELSE
              v_i :=v_n_points;
            END IF;
          END IF;
          CASE v_i
            WHEN 1          THEN v_point_pos := 1;
            v_z_value   := v_current_line_layer_z_start;
            WHEN v_n_points THEN v_point_pos := 2;
            v_z_value   := v_current_line_layer_z_end;
            ELSE                 v_point_pos := 3;
            v_z_value   := 0;
            END CASE;

          v_point_dims := pgr_create_top_get_point_dims(v_point_pos, v_geom_dims, v_zconn) ;

          v_point = st_pointn(v_current_line_layer_the_geom,v_i );
          v_point = pgr_create_topo_set_point(v_zconn, v_point, v_z_value,v_point_dims);
          SELECT geom, r, id INTO v_r_geom, v_r_r, v_r_id
          FROM pgr_create_top_graph_ptos AS g
          WHERE st_dwithin(v_point,g.geom,p_tolerance) AND --to use index
            st_3ddwithin(v_point, g.geom, p_tolerance) AND g = v_group AND dims = v_point_dims --a point with 3d never can be represent by a point in 2d and viceversa
          LIMIT 1;
          --an intermediate point which no intersects with any other point must be inserted with r =null
          --an intermediate point with r = null means it might not be a point in the graph, unless some other point intersects with it.
          --if some point intersects with an intermediate point both of them must have same r
          --the set of point that intersects must have same r
          v_r := NULL;
          IF v_r_geom IS NOT NULL AND v_r_r IS NOT NULL THEN
            v_r :=v_r_r;
          ELSEIF v_point_pos != 3 OR (v_r_geom IS NOT NULL) THEN
            --create a new representant.
            v_r := v_r_point_id;
            IF v_r_r IS NULL THEN
              UPDATE pgr_create_top_graph_ptos SET r = v_r
              WHERE id = v_r_id --use index
                AND g = v_group
                AND st_3ddwithin(geom,v_r_geom, p_tolerance);
            END IF;
            EXECUTE 'INSERT INTO '|| v_r_table_name ||
                    ' VALUES($1,NULL,NULL, $2,$3)' USING v_r, v_point, v_group;
            v_r_point_id := v_r_point_id + 1;
            /*  ELSEIF v_point_pos = 3 THEN
                v_r := null;   */
          END IF;

          INSERT INTO pgr_create_top_graph_ptos VALUES (v_point_pos,v_current_line_layer_id, v_lineal_layer, v_r, v_point, v_group,v_point_dims);
        END LOOP;



      END LOOP;
    END LOOP lineal_layer_label;

  END LOOP lineal_groups_label;

  --Analazing point layers
  IF p_puntual_groups IS NOT NULL THEN
    FOR v_keyvalue IN SELECT * FROM jsonb_each(p_puntual_groups) LOOP
      --Add non-inserted points from layers where connectivity policy was overwritten
      v_p_groups = '{}';
      FOR v_lineal_layer IN SELECT v.value FROM  jsonb_array_elements_text(v_keyvalue.value) AS v LOOP

        v_group := p_layers->v_lineal_layer->>'group';
        IF(array_position(v_p_groups, v_group) ISNULL ) THEN
          v_p_groups := v_p_groups || v_group;
        END IF;

        IF (p_layers->(v_keyvalue.key)->>'pconn') = '1' AND
           (p_layers->v_lineal_layer->>'pconn') = '0' THEN

          --after this iteration intermediate points where inserted
          --so this is to avoid duplicate insertings
          p_layers := pgr_polyfill_jsonb_set(p_layers, (v_lineal_layer || '{pconn}'::TEXT[]), '1'::jsonb );
          v_zconn := p_layers->v_lineal_layer->>'zconn';
          v_geom_dims := p_layers->v_lineal_layer->>'dims';


          FOR v_current_line_layer_id, v_current_line_layer_the_geom IN
            EXECUTE p_layers->v_lineal_layer->>'sql' LOOP

            v_current_line_layer_the_geom := pgr_multiline_to_linestring(v_current_line_layer_the_geom,p_tolerance, FALSE );
            IF(v_current_line_layer_the_geom IS NULL ) THEN --I havent done anything with this line, it wasnt processed because of topological errors.
              CONTINUE;
            END IF;
            v_n_points := st_npoints(v_current_line_layer_the_geom);
            FOR v_i IN 2..(v_n_points-1) LOOP
              v_point = st_pointn(v_current_line_layer_the_geom,v_i );
              v_point = pgr_create_topo_set_point(v_zconn, v_point, 0, v_geom_dims);
              v_point_dims := pgr_create_top_get_point_dims(3, v_geom_dims, v_zconn);

              INSERT INTO pgr_create_top_graph_ptos VALUES (3,v_current_line_layer_id,v_lineal_layer,NULL,v_point,v_group, v_point_dims);
            END LOOP;
          END LOOP;

        END IF;
      END LOOP;
      --Intersect points from point layers with the inserted ones
      v_zconn := p_layers->(v_keyvalue.key)->>'zconn';
      v_lineal_layer := NULL;
      v_first :=TRUE;
      FOR v_point_the_geom, v_point_z, v_point_id IN EXECUTE 'SELECT subq.the_geom, subq.z, subq.id from (' || (p_layers->(v_keyvalue.key)->>'sql')||') AS subq' LOOP

        v_point := pgr_create_topo_set_point(v_zconn, v_point_the_geom,v_point_z, 0);
        IF v_first THEN
          --As points from point layers are always converted v_geom_dims = v_point_dims
          v_geom_dims := st_ndims(v_point);
          p_layers = pgr_polyfill_jsonb_set(p_layers,('{'||v_keyvalue.key||', dims}')::TEXT[], to_json(v_geom_dims)::jsonb);
          v_first := FALSE;
        END IF;

        FOR v_group IN  SELECT g FROM unnest(v_p_groups) AS g LOOP

          --There may exists multiple points that intersects in the same group, all with equal r, some with same r and the other with null, or all with r = null
          SELECT geom, r INTO v_r_geom, v_r_r
          FROM pgr_create_top_graph_ptos AS g
          WHERE st_dwithin(g.geom, v_point, p_tolerance) AND --to use index
            st_3ddwithin(g.geom, v_point, p_tolerance) AND
            g.g = v_group  AND dims = v_geom_dims
          ORDER BY r NULLS LAST --Because I order by r with nulls last the first value must be an assigned one if there is one
          LIMIT 1;
          IF v_r_geom IS NULL THEN
            RETURN QUERY SELECT  v_point_id::BIGINT, v_keyvalue.key::TEXT ,'The point from point layer:'|| v_keyvalue.key ||' doesnt intersect any other point in graph in group: '|| v_group ||', skipping'::TEXT;
            CONTINUE ;
          END IF;

          EXECUTE 'SELECT layname FROM ' || v_r_table_name|| ' WHERE id=$1'INTO v_point_layer_name USING v_r_r;

          IF v_point_layer_name NOT LIKE (v_keyvalue.key) THEN
            RETURN QUERY SELECT v_point_id::BIGINT, v_keyvalue.key::TEXT ,'The point intersects with other point from another point layer: '|| v_keyvalue.key ||' , joining groups can not be done with this point, skipping.'::TEXT;
            CONTINUE;
          END IF;

          IF v_r_r IS NULL THEN
            EXECUTE 'INSERT INTO '||v_r_table_name||' VALUES($1,$2,$3,$4,$5) '
              USING v_r_point_id, v_keyvalue.key, v_point_id, v_point, v_group;
            v_r := v_r_point_id;
            v_r_point_id := v_r_point_id+1;
          ELSE
            v_r = v_r_r;
            EXECUTE 'update '||v_r_table_name|| ' SET layname=$1, id_geom=$2 WHERE id=$3' USING v_keyvalue.key,v_point_id,v_r;
          END IF;

        UPDATE pgr_create_top_graph_ptos AS g SET r = v_r WHERE st_dwithin(g.geom, v_point, p_tolerance) AND --to use index
          st_3ddwithin(g.geom, v_point, p_tolerance) AND
          g.g = v_group AND dims = v_geom_dims;

        END LOOP;

        --Adding transference lines to graph
        --IF a point in a point layer join N line layers, there has to be (n*n+1)/2 extra edges representing transference edges from one group to another
         FOR v_group_layer_name_A, v_group_r_A, v_group_layer_name_B, v_group_r_B IN SELECT A.layname, A.r, B.layname, B.r FROM pgr_create_top_graph_ptos A, pgr_create_top_graph_ptos B
                                                        WHERE st_dwithin(v_point,A.geom , p_tolerance) AND
                                                              st_3ddwithin(A.geom, B.geom, p_tolerance) AND
                                                              A.g > B.g
                                                              LOOP
          EXECUTE 'INSERT INTO '||v_lines_table_name|| ' VALUES(NULL,$1,$2,NULL,NULL,$3,$4);'USING v_group_r_A, v_group_r_B, array[v_group_layer_name_A,v_group_layer_name_B], v_group;
         END LOOP;

      END LOOP;

    END LOOP;
  END IF;

  --Chopping lines
  FOR v_keyvalue IN SELECT * FROM jsonb_each(p_layers) LOOP


    IF (p_layers->(v_keyvalue.key)->'group') ISNULL THEN
      CONTINUE ;
    END IF;

    v_group := p_layers->(v_keyvalue.key)->'group';
    v_zconn := p_layers->(v_keyvalue.key)->>'zconn';
    v_geom_dims := p_layers->(v_keyvalue.key)->>'dims';
    FOR v_current_line_layer_id,v_current_line_layer_the_geom IN
      EXECUTE p_layers->(v_keyvalue.key)->>'sql' LOOP


      v_current_line_layer_the_geom := pgr_multiline_to_linestring(v_current_line_layer_the_geom, p_tolerance, (v_geom_dims = 3 AND v_zconn = 2));
      IF(v_current_line_layer_the_geom IS NULL) THEN --I havent done anything with this line, it wasnt processed because of topological errors.
        CONTINUE;
      END IF;


      --For every line in a layer there is only one start point and end point, so I can do this.
      FOR v_pos, v_r IN SELECT pos,r FROM pgr_create_top_graph_ptos WHERE id = v_current_line_layer_id AND layname = v_keyvalue.key AND (pos=1 OR pos =2) LOOP
        IF v_pos = 1 THEN
          v_source = v_r;
        ELSEIF v_pos = 2 THEN
          v_target = v_r;
        END IF;
      END LOOP;
      --Insert first line with its source and target values.
      EXECUTE 'INSERT INTO '||v_lines_table_name||' VALUES ($1,$2,$3,$4,$5,NULL,$6)'USING v_current_line_layer_the_geom, v_source, v_target,v_current_line_layer_id,v_keyvalue.key, v_group;

      --      Points are not added so this is not necessary
      --         --if geometries are 2d and z is gotten from query, intermediate points will not have z value, so lines can't be chopped
      --         if v_geom_dims = 2  and v_zconn = 1 THEN
      --           CONTINUE;
      --         END IF;

      --For each intermediate point of this line having a representant
      FOR v_r,v_point IN SELECT  geometries.r, geometries.geom
                         FROM  (SELECT distinct r,geom FROM pgr_create_top_graph_ptos WHERE id = v_current_line_layer_id AND
                            layname = v_keyvalue.key AND
                            pos = 3 AND  r IS NOT NULL) AS geometries
                         INNER JOIN (SELECT * FROM st_dumppoints(v_current_line_layer_the_geom)) AS points ON -- It's needed that points are
                           (st_3ddwithin(points.geom,geometries.geom,p_tolerance))     -- in order of line drawn
                         ORDER BY points.path
        LOOP

          IF (v_zconn = 2 AND v_geom_dims = 3)  THEN
            EXECUTE 'SELECT  geom,id FROM '||v_lines_table_name ||
                    ' WHERE id_geom = $1 AND layname = $2 AND pgr_create_topo_check_intersect("geom",$3,$4)'
              INTO v_intersected_geom, v_intersected_id USING v_current_line_layer_id, v_keyvalue.key, v_point, p_tolerance;
          ELSE
            EXECUTE 'SELECT  geom,id FROM '||v_lines_table_name ||
                    ' WHERE id_geom = $1 AND layname = $2 AND st_3ddwithin(geom, $3,$4)'
              INTO v_intersected_geom, v_intersected_id USING v_current_line_layer_id, v_keyvalue.key, v_point, p_tolerance  ;
          END IF;
        IF st_3ddwithin(st_startpoint(v_intersected_geom),v_point,p_tolerance) OR
           st_3ddwithin(st_endpoint(v_intersected_geom),v_point,p_tolerance) THEN

          CONTINUE ; --IF there is a point with R at start or end in any line, it means that this line was already chopped by this r point, so there are r
                     --points duplicated on this point.
        END IF;

        --v_source and v_target has to refresh because line selected can be a chopped one so, v_source and v_target change
        SELECT r INTO v_source
        FROM pgr_create_top_graph_ptos
        WHERE id = v_current_line_layer_id
          AND layname = v_keyvalue.key
          AND st_3ddwithin(geom, st_startpoint(v_intersected_geom), p_tolerance);

        SELECT r INTO v_target
        FROM pgr_create_top_graph_ptos
        WHERE id = v_current_line_layer_id
          AND layname = v_keyvalue.key
          AND st_3ddwithin(geom, st_endpoint(v_intersected_geom), p_tolerance);

        v_points_make_line := '{}';
        --This always has to execute because if there is a representative point, there is a line that contains it
        FOR v_line_point IN SELECT geom FROM st_dumppoints(v_intersected_geom) ORDER BY path LOOP
          --As each point is an intermediate point it is going to have the same dimension as the line that contains it
          IF v_zconn = 2 AND v_geom_dims = 3 THEN
            v_line_point := pgr_create_topo_set_point(2,v_line_point,0,0);
          END IF;
          IF st_3ddwithin(v_line_point,v_point,p_tolerance) THEN
            v_points_make_line := v_points_make_line || v_line_point;
            EXECUTE 'INSERT INTO '||v_lines_table_name||' VALUES ($1,$2,$3,$4,$5,NULL,$6)' USING st_makeline(v_points_make_line),v_source,v_r,v_current_line_layer_id,v_keyvalue.key,v_group;
            v_points_make_line :='{}';
            v_source := v_r;
          END IF;
          v_points_make_line := v_points_make_line || v_line_point;

        END LOOP;

        --this will never insert garbage because of if there is an intermediate point that has a representative point
        --there is a line who contains it and therefore it will be chopped just over that point in this iteration, so the
        --final line is going to have that point as the first point and the last point from v_intersected's last point
        EXECUTE 'INSERT INTO '||v_lines_table_name ||' VALUES($1,$2,$3,$4,$5,NULL,$6)' USING st_makeline(v_points_make_line), v_r, v_target,v_current_line_layer_id,v_keyvalue.key,v_group;
        EXECUTE 'DELETE From '||v_lines_table_name ||' WHERE id = $1' USING v_intersected_id;

      END LOOP;
    END LOOP;
  END LOOP;

END

$$ LANGUAGE plpgsql;