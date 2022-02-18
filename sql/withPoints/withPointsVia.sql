/*PGR-GNU*****************************************************************
File: withPointsVia.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo

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

--v3.4
CREATE FUNCTION pgr_withPointsVia(
  TEXT,     -- edges SQL
  TEXT,     -- points SQL
  ANYARRAY, -- via vids

  directed BOOLEAN DEFAULT true,

  -- via parameters
  strict BOOLEAN DEFAULT false,
  U_turn_on_edge BOOLEAN DEFAULT true,

  -- withPoints parameters
  driving_side CHAR DEFAULT 'b', -- 'r'/'l'/'b'/NULL
  details BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT,
  OUT route_agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
  SELECT *
  FROM _pgr_withPointsVia( _pgr_get_statement($1), _pgr_get_statement($2), $3, $4, $5, $6, $7, $8);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

COMMENT ON FUNCTION pgr_withPointsVia(TEXT, TEXT, ANYARRAY, BOOLEAN, BOOLEAN, BOOLEAN, CHAR, BOOLEAN)
IS 'pgr_withPointsVia
- PROPOSED
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - Points SQL with columns: [pid], edge_id, fraction [,side]
  - ARRAY[via vertices identifiers]
- Optional Parameters
  - directed := true
  - strict := false
  - U_turn_on_edge := true
  - driving_side := ''b''
  - details := ''false''
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_withPointsVia.html
';

--v2.6
CREATE FUNCTION  _pgr_withPointsVia(
    sql TEXT,
    via_edges BIGINT[],
    fraction FLOAT[],
    directed BOOLEAN DEFAULT TRUE,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT,
    OUT route_agg_cost FLOAT)

  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  sql_new_vertices text := ' ';
  sql_on_vertex text;
  v_union text := ' ';
  dummyrec record;
  rec1 record;
  via_vertices int[];
  sql_safe text;
  new_edges text;
  BEGIN
     BEGIN
        sql_safe = 'SELECT id, source, target, cost, reverse_cost FROM ('|| sql || ') AS __a';

        EXECUTE 'select reverse_cost, pg_typeof(reverse_cost)::text as rev_type  from ('||sql_safe||' ) AS __b__ limit 1 ' INTO rec1;
        has_rcost := true;
        EXCEPTION
          WHEN OTHERS THEN
            has_rcost = false;
     END;


      IF array_length(via_edges, 1) != array_length(fraction, 1) then
        RAISE EXCEPTION 'The length of via_edges is different of length of via_edges';
      END IF;

      FOR i IN 1 .. array_length(via_edges, 1)
      LOOP
          IF fraction[i] = 0 THEN
              sql_on_vertex := 'SELECT source FROM ('|| sql || ') __a where id = ' || via_edges[i];
              EXECUTE sql_on_vertex into dummyrec;
              via_vertices[i] = dummyrec.source;
          ELSE IF fraction[i] = 1 THEN
              sql_on_vertex := 'SELECT target FROM ('|| sql || ') __a where id = ' || via_edges[i];
              EXECUTE sql_on_vertex into dummyrec;
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
                   orig AS ( ' || sql || '),
                   original AS (SELECT id, source, target, cost, reverse_cost FROM orig),
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
                   orig AS ( ' || sql || '),
                   original AS (SELECT id, source, target, cost FROM orig),
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

     sql_new_vertices := sql_new_vertices || v_union || ' (' || sql || ')';

     RETURN query SELECT *
         FROM pgr_dijkstraVia(new_edges, via_vertices, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT
  COST 100
  ROWS 1000;


-- COMMENTS


COMMENT ON FUNCTION _pgr_withPointsVia(TEXT, BIGINT[], FLOAT[], BOOLEAN)
IS 'pgRouting deprecated internal function';
