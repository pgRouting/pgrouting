/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
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
/*PGR

Template:
Copyright (c) 2015 pgRouting developers

Function developer:
Copyright (c) 2015 Vicky Vergara
vicky_vergara@hotmail.com

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

*/


CREATE OR REPLACE FUNCTION _pgr_dijkstraViaVertices(sql text, vertices anyarray, directed boolean, has_rcost boolean,
  OUT seq integer, OUT start_v bigint, OUT end_v bigint, OUT vertex bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/lib${PGROUTING_LIBRARY_NAME}', 'pgr_dijkstra_via_vertices'  -- must match name in .c file
    LANGUAGE c IMMUTABLE STRICT;

-- signature
CREATE OR REPLACE FUNCTION pgr_dijkstraViaVertices(sql text, vertices anyarray, directed boolean default true,
  OUT seq integer, OUT start_v bigint, OUT end_v bigint, OUT vertex bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('dijkstra', sql, true); 
      RETURN query SELECT * 
         FROM _pgr_dijkstraViaVertices(sql, vertices, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

CREATE OR REPLACE FUNCTION  pgr_dijkstraViaEdges(sql text, via_edges bigint[], fraction float[], directed boolean default true,
  OUT seq integer, OUT start_v bigint, OUT end_v bigint, OUT vertex bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  sql_new_vertices text := ' ';
  sql_on_vertex text;
  v_union text := ' ';
  dummyrec record;
  via_vertices int[];
  sql_safe text;
  new_edges text;
  BEGIN
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      IF array_length(via_edges, 1) != array_length(fraction, 1) then
        RAISE EXCEPTION 'The length of via_edges is different of length of via_edges';
      END IF;
      sql_safe = 'SELECT id, source, target, cost, reverse_cost FROM ('|| sql || ') AS __a';
      FOR i IN 1 .. array_length(via_edges, 1)
      LOOP
          IF fraction[i] = 0 THEN
              sql_on_vertex := 'SELECT source FROM ('|| sql || ') __a where id = ' || via_edges[i];
              execute sql_on_vertex into dummyrec; 
              via_vertices[i] = dummyrec.source;
          ELSE IF fraction[i] = 1 THEN
              sql_on_vertex := 'SELECT target FROM ('|| sql || ') __a where id = ' || via_edges[i];
              execute sql_on_vertex into dummyrec; 
              via_vertices[i] = dummyrec.target;
          ELSE
              via_vertices[i] = -i;
              IF has_rcost THEN
                   sql_new_vertices = sql_new_vertices || v_union ||
                          '(SELECT id, source, ' ||  -i || ' AS target, cost * ' || fraction[i] || ' AS cost,
                              reverse_cost * (1 - ' || fraction[i] || ')  AS reverse_cost
                          FROM (SELECT * FROM (' || sql || ') __b' || i || ' WHERE id = ' || via_edges[i] || ') __a' || i ||')
                             UNION
                          (SELECT id, ' ||  -i || ' AS source, target, cost * (1 -' || fraction[i] || ') AS cost,
                              reverse_cost *  ' || fraction[i] || '  AS reverse_cost
                          FROM (SELECT * FROM (' || sql || ') __b' || i || ' where id = ' || via_edges[i] || ') __a' || i ||')';
                      v_union = ' UNION ';
               ELSE 
                   sql_new_vertices = sql_new_vertices || v_union ||
                          '(SELECT id, source, ' ||  -i || ' AS target, cost * ' || fraction[i] || ' AS cost
                          FROM (SELECT * FROM (' || sql || ') __b' || i || ' WHERE id = ' || via_edges[i] || ') __a' || i ||')
                             UNION
                          (SELECT id, ' ||  -i || ' AS source, target, cost * (1 -' || fraction[i] || ') AS cost
                          FROM (SELECT * FROM (' || sql || ') __b' || i || ' WHERE id = ' || via_edges[i] || ') __a' || i ||')';
                      v_union = ' UNION ';
               END IF;
          END IF;
          END IF;
     END LOOP;


     IF sql_new_vertices = ' ' THEN
         new_edges := sql; 
     ELSE
         IF has_rcost THEN
            new_edges:= 'WITH
                   original AS ( ' || sql || '),
                   the_union AS ( ' || sql_new_vertices || '),
                   first_part AS ( SELECT * FROM (SELECT id, target AS source,  lead(target) OVER w  AS target,
                         lead(cost) OVER w  - cost AS cost,
                         lead(cost) OVER w  - cost AS reverse_cost
                      FROM  the_union  WHERE source > 0 AND cost > 0
                      WINDOW w AS (PARTITION BY id  ORDER BY cost ASC) ) as n2
                      WHERE target IS NOT NULL),
                   second_part AS ( SELECT * FROM (SELECT id, lead(source) OVER w  AS source, source as target,
                         reverse_cost - lead(reverse_cost) OVER w  AS cost,
                         reverse_cost - lead(reverse_cost) OVER w  AS reverse_cost
                      FROM  the_union  WHERE target > 0 and reverse_cost > 0
                      WINDOW w AS (PARTITION BY id  ORDER BY reverse_cost ASC) ) as n2
                      WHERE source IS NOT NULL),
                   more_union AS ( SELECT * from (
                       (SELECT * FROM original) 
                             UNION 
                       (SELECT * FROM the_union) 
                             UNION 
                       (SELECT * FROM first_part) 
                             UNION
                       (SELECT * FROM second_part) ) _union )
                  SELECT *  FROM more_union';
         ELSE
            new_edges:= 'WITH
                   original AS ( ' || sql || '),
                   the_union AS ( ' || sql_new_vertices || '),
                   first_part AS ( SELECT * FROM (SELECT id, target AS source,  lead(target) OVER w  AS target,
                         lead(cost) OVER w  - cost AS cost
                      FROM  the_union  WHERE source > 0 AND cost > 0
                      WINDOW w AS (PARTITION BY id  ORDER BY cost ASC) ) as n2
                      WHERE target IS NOT NULL ),
                   more_union AS ( SELECT * from (
                       (SELECT * FROM original) 
                             UNION 
                       (SELECT * FROM the_union) 
                             UNION 
                       (SELECT * FROM first_part) ) _union )
                  SELECT *  FROM more_union';
          END IF;
      END IF;

 -- raise notice '%', new_edges;
     sql_new_vertices := sql_new_vertices || v_union || ' (' || sql || ')';
     RETURN query SELECT *
         FROM _pgr_dijkstraViaVertices(new_edges, via_vertices, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


CREATE OR REPLACE FUNCTION  pgr_dijkstraViaCompleteEdges(sql text, via_edges bigint[], directed boolean default true,
  OUT seq integer, OUT start_e bigint, OUT end_e bigint, OUT vertex bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  sql_new_vertices text := ' ';
  sql_on_vertex text;
  sql_temp_table text;
  v_union text := ' ';
  dummyrec record;
  via_vertices int[];
  source int[];
  target int[];
  this_record record;
  prev_record record;
  case_start_sql text := 'start_v = CASE ';
  case_end_sql text := 'end_v = CASE ';
  update_sql text;
  choose_sql text;
  i integer;
  k integer;
  BEGIN
      has_rcost :=_pgr_parameter_check('dijkstra', sql, true);
      sql_temp_table := 'CREATE TEMPORARY TABLE _my_temp1 ON COMMIT DROP AS SELECT id, source, target, cost, reverse_cost FROM (' || sql || ') a
                            WHERE id = ANY( ' || quote_literal(via_edges) || '::bigint[] ) ';
      EXECUTE sql_temp_table;
      FOR i IN 1 .. array_length(via_edges, 1)
      LOOP
              via_vertices[i] = -i;
              case_start_sql := case_start_sql || ' WHEN start_v = ' || -i || ' THEN ' || via_edges[i];
              case_end_sql := case_end_sql || ' WHEN end_v = ' || -i || ' THEN ' || via_edges[i];
              IF has_rcost THEN

                   sql_new_vertices = sql_new_vertices || v_union ||
                          '(SELECT id, source, ' ||  -i || ' AS target, cost * 0.5  AS cost,
                              reverse_cost * 0.5  AS reverse_cost
                          FROM (' || sql || '  where id = ' || via_edges[i] || ') a' || i ||')
                             UNION
                          (SELECT id, ' ||  -i || ' AS source, target, cost * 0.5 AS cost,
                              reverse_cost * 0.5  AS reverse_cost
                          FROM (' || sql || '  where id = ' || via_edges[i] || ') a' || i ||')';
                      v_union = ' UNION ';
               ELSE 
                   sql_new_vertices = sql_new_vertices || v_union ||
                          '(SELECT id, source, ' ||  -i || ' AS target, cost * 0.5 as cost
                          FROM (' || sql || '  where id = ' || via_edges[i] || ') a' || i ||')
                             UNION
                          (SELECT id, ' ||  -i || ' AS source, target, cost * 0.5 as cost
                          FROM (' || sql || '  where id = ' || via_edges[i] || ') a' || i ||')';
                      v_union = ' UNION ';
               END IF;
      END LOOP;

     sql_new_vertices := sql_new_vertices || v_union || ' (' || sql || ')';
     CREATE TEMPORARY TABLE result_tab ON COMMIT DROP AS  SELECT * FROM _pgr_dijkstraViaVertices(sql_new_vertices, via_vertices, directed, has_rcost);
     
     update_sql := 'UPDATE result_tab set ' || case_start_sql || ' END, ' || case_end_sql || 'END ';
     EXECUTE update_sql;
     i := 1;  -- cycle the records
     k := 1;  -- cycle the via_edges
     FOR this_record IN SELECT result_tab.seq, result_tab.start_v AS start_e, result_tab.end_v AS end_e, result_tab.vertex, result_tab.edge, result_tab.cost, result_tab.agg_cost  from result_tab order by seq
     LOOP
         IF i = 1 THEN prev_record = this_record; i:= 1 + 1; CONTINUE; END IF; 
         IF via_edges[k] = prev_record.edge and prev_record.vertex < 0 THEN
            choose_sql := 'select source, target from _my_temp1 where id = ' || via_edges[k] ;
            execute choose_sql into dummyrec;
            IF this_record.vertex = dummyrec.source THEN
                update_sql := 'UPDATE result_tab SET vertex = '|| dummyrec.target || ', cost = cost * 2 WHERE seq = '|| prev_record.seq;
                EXECUTE update_sql;
            ELSE
                update_sql := 'UPDATE result_tab SET vertex = '|| dummyrec.source|| ', cost = cost * 2 WHERE seq = '|| prev_record.seq;
                EXECUTE update_sql;
            END IF;

         ELSIF this_record.edge < 0 and this_record.vertex < 0 THEN
            choose_sql := 'select source, target from _my_temp1 where id = ' || via_edges[k+1];
            execute choose_sql into dummyrec;
            IF prev_record.vertex = dummyrec.source THEN
                update_sql := 'UPDATE result_tab SET vertex = '|| dummyrec.target || ' WHERE seq = '|| this_record.seq;
                EXECUTE update_sql;

                update_sql := 'UPDATE result_tab SET cost = cost * 2 WHERE seq = '|| prev_record.seq;
                EXECUTE update_sql;

            ELSE
                update_sql := 'UPDATE result_tab SET vertex = '|| dummyrec.source|| ' WHERE seq = '|| this_record.seq;
                EXECUTE update_sql;

                update_sql := 'UPDATE result_tab SET cost = cost * 2 WHERE seq = '|| prev_record.seq;
                EXECUTE update_sql;

            END IF;
            k := k + 1;

         ELSE
         END IF;
         prev_record = this_record;
         i := i + 1;
     END LOOP;
     DELETE FROM result_tab WHERE result_tab.edge = -1 AND result_tab.seq <> (i - 1);
     RETURN query SELECT row_number() over()::integer as seq,
                         res.start_v AS start_e,
                         res.end_v AS end_e,
                         res.vertex,
                         res.edge,
                         res.cost,
                         (select sum(res1.cost) from result_tab res1 where res1.seq <= res.seq) AS agg_cost
                         from result_tab res order by seq;
  END
  $BODY$ LANGUAGE plpgsql VOLATILE COST 100 ROWS 1000;


