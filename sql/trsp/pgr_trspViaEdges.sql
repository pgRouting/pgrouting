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

-- TODO restrictions with one edge are wrong
--v3.0
CREATE FUNCTION pgr_trspViaEdges(
    text,      -- SQL (required)
    integer[], -- eids (required)
    FLOAT[],   -- pcts (required)
    BOOLEAN,   -- directed (required)
    BOOLEAN,   -- has_rcost (requierd)

    turn_restrict_sql text DEFAULT NULL::text,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT id3 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF RECORD AS
$body$
/*
 *  pgr_trsp(sql text, eids integer[], pcts float8[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
 *
 *  Compute TRSP with edge_ids and pposition along edge. We compute the path between eids[i], pcts[i] and eids[i+1], pcts[i+1]
 *  and chain the results together.
 *
 *  NOTE: this is a prototype function, we can gain a lot of efficiencies by implementing this in C/C++
 *
*/
declare
    sql TEXT          := $1;
    eids INTEGER[]    := $2;
    pcts FLOAT[]      := $3;
    directed BOOLEAN  := $4;
    has_rcost BOOLEAN := $5;

    rr RECORD;
    lrr RECORD;
    first boolean := true;
    seq1 integer := 0;

    i integer;
    j INTEGER := 1;
    seq2 integer :=0;
    has_reverse BOOLEAN;
    point_is_vertex BOOLEAN := false;
    edges_sql TEXT;
    f float;
    point_q TEXT;
    points_sql TEXT := '';
    from_v INTEGER;
    to_v INTEGER;
    word TEXT := '';
    combinations TEXT := '';
    x INTEGER[];
    restrictions_query TEXT;
    final_query TEXT;
    vertices INTEGER[];


begin
  IF $1 IS NULL OR $2 IS NULL OR $3 IS NULL OR $4 IS NULL OR $5 IS NULL THEN RETURN; END IF;


    SELECT 0::INTEGER AS seq, NULL::INTEGER AS id1, NULL::INTEGER AS id2, NULL::INTEGER AS id3, NULL::FLOAT AS cost INTO lrr;
    has_reverse =_pgr_parameter_check('dijkstra', sql, false);
    edges_sql := sql;
    IF (has_reverse != has_rcost) THEN
        IF (NOT has_rcost) THEN
            -- user does not want to use reverse cost column
            edges_sql = 'SELECT id, source, target, cost FROM (' || sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    if array_length(eids, 1) != array_length(pcts, 1) then
      raise exception 'The length of arrays eids and pcts must be the same!';
    end if;

    FOREACH f IN ARRAY pcts LOOP
        IF f in (0,1) THEN
           point_is_vertex := true;
        END IF;
    END LOOP;

  FOR i IN 1 .. array_length(eids, 1) LOOP
    point_q := NULL;
    if (j != 1) THEN word = ' UNION '; ELSE word = ''; END IF;
    IF pcts[i] = 0 THEN
      EXECUTE '(SELECT source FROM (' || sql || ') b WHERE id = ' ||  eids[i] || ')' INTO from_v;
    ELSIF pcts[i] = 1 THEN
      EXECUTE '(SELECT target FROM (' || sql || ') b WHERE id = ' || eids[i] || ')' INTO from_v;
    ELSE
      from_v = -j;
      point_q =  '(SELECT '|| j ||' as pid, ' || eids[i] || ' as edge_id, ' || pcts[i] || '::float8 as fraction)';
      j := j + 1;
    END IF;
    IF point_q IS NOT NULL THEN points_sql = point_q || word || points_sql; END IF;

    vertices := vertices || from_v;

  END LOOP;

    FOR i IN 1 .. array_length(vertices, 1) - 1 LOOP
    if (i != 1) THEN word = ' UNION '; ELSE word = ''; END IF;
    combinations := combinations || word || '(' || vertices[i] || ' AS source, ' || vertices[i+1] || ' AS target)';
  END LOOP;

  restrictions_query := format($$
  WITH o_restrictions AS (%1$s),
  old_restrictions AS (SELECT row_number() over () AS _id, to_cost, string_to_array(via_path,',')::INTEGER[]||target_id AS via_path FROM o_restrictions),
  the_values  AS (SELECT _id, to_cost, via_path AS array, s AS subscript, via_path[s] AS value
    FROM (SELECT generate_subscripts(via_path,1) AS s, _id, to_cost, via_path FROM old_restrictions) foo)
  SELECT _id AS id, to_cost AS cost, array_agg(value ORDER BY subscript DESC) AS path FROM the_values GROUP BY _id, to_cost;
  $$, turn_restrict_sql);

  /*
  RAISE WARNING '%', points_sql;
  RAISE WARNING '%', vertices;
  RAISE WARNING '%', combinations;
  RAISE WARNING '%', restrictions_query;

  SELECT pgr_trsp_withPointsVia(
    $$SELECT * FROM edge_table$$,
    $$
    WITH
      o_restrictions AS (SELECT to_cost, target_id::INTEGER, from_edge||coalesce('',''||via_path,'''') AS via_path FROM restrictions),
      old_restrictions AS (SELECT row_number() over () AS _id, to_cost, string_to_array(via_path,',')::INTEGER[] AS via_path FROM o_restrictions),
      the_values  AS (SELECT _id, to_cost, via_path AS array, s AS subscript, via_path[s] AS value
        FROM (SELECT generate_subscripts(via_path,1) AS s, _id, to_cost, via_path FROM old_restrictions) foo)
    SELECT _id AS id, to_cost AS cost, array_agg(value ORDER BY subscript DESC)  AS path
    FROM the_values GROUP BY _id, to_cost
    $$,
    $$(SELECT 3 as pid, 6 as edge_id, 0.5::float8 as fraction)
    UNION
    (SELECT 2 as pid, 11 as edge_id, 0.5::float8 as fraction)
    UNION
    (SELECT 1 as pid, 1 as edge_id, 0.5::float8 as fraction)
    $$,
  '{-1,-2,-3}'::BIGINT[],
  false);
*/

/*
SELECT seq-1::INTEGER, path_id::INTEGER, node::INTEGER, edge::INTEGER, cost::FLOAT FROM pgr_trsp_withPointsVia(
  $query$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table$query$,
  $query$
  WITH o_restrictions AS (SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path),
  old_restrictions AS (SELECT row_number() over () AS _id, to_cost, string_to_array(via_path,',')::INTEGER[] AS via_path FROM o_restrictions),
  the_values  AS (SELECT _id, to_cost, via_path AS array, s AS subscript, via_path[s] AS value
    FROM (SELECT generate_subscripts(via_path,1) AS s, _id, to_cost, via_path FROM old_restrictions) foo)
  SELECT _id AS id, to_cost AS cost, array_agg(value ORDER BY subscript DESC) AS path FROM the_values GROUP BY _id, to_cost;
  $query$,
  $query$(SELECT 3 as pid, 1 as edge_id, 0.5::float8 as fraction) UNION (SELECT 2 as pid, 2 as edge_id, 0.5::float8 as fraction) UNION (SELECT 1 as pid, 1 as edge_id, 0.1::float8 as fraction)$query$,
  '{-1,-2,-3}'::INTEGER[],
  $bool$f$bool$::BOOLEAN)
*/

    IF (turn_restrict_sql IS NULL OR length(turn_restrict_sql) = 0) THEN
        -- no restrictions then its a pgr_withPointsVia
        RAISE NOTICE 'Using pgr_withPointsVia';
        RETURN query SELECT a.seq::INTEGER, path_id::INTEGER AS id1, node::INTEGER AS id2, edge::INTEGER AS id3, a.cost
        FROM pgr_withPointsVia(edges_sql, points_sql, vertices, directed) a;
        RETURN;
    END IF;

    RETURN QUERY
    SELECT a.seq-1::INTEGER, path_id::INTEGER, node::INTEGER, edge::INTEGER, a.cost::FLOAT FROM
    pgr_trsp_withPointsVia(edges_sql, restrictions_query, points_sql, vertices, directed) AS a;
 --RAISE WARNING '%', final_query;



  RETURN;

    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(eids, 1)-1 loop
        seq2 := seq2 + 1;
        for rr in select a.seq, seq2 as id1, a.id1 as id2, a.id2 as id3, a.cost
                    from pgr_trsp(edges_sql,
                                  eids[i], pcts[i],
                                  eids[i+1], pcts[i+1],
                                  directed,
                                  has_rcost,
                                  turn_restrict_sql) as a loop
            -- combine intermediate via costs when cost is split across
            -- two parts of a segment because it stops it and
            -- restarts the next leg also on it
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --
            -- there are multiple condition we have to deal with
            -- between the end of one leg and start of the next
            -- 1. same vertex_id. edge_id=-1; drop record with edge_id=-1
            -- means: path ends on vertex
            -- NOTICE:  rr: (19,1,44570022,-1,0)
            -- NOTICE:  rr: (0,2,44570022,1768045,2.89691196717448)
            -- 2. vertex_id=-1; sum cost components
            -- means: path end/starts with the segment
            -- NOTICE:  rr: (11,2,44569628,1775909,9.32885885148532)
            -- NOTICE:  rr: (0,3,-1,1775909,0.771386350984395)

            --raise notice 'rr: %', rr;
            if first then
                lrr := rr;
                first := false;
            else
                if lrr.id3 = -1 then
                    lrr := rr;
                elsif lrr.id3 = rr.id3 then
                    lrr.cost := lrr.cost + rr.cost;
                    if rr.id2 = -1 then
                        rr.id2 := lrr.id2;
                    end if;
                else
                    seq1 := seq1 + 1;
                    lrr.seq := seq1;

                    seq := lrr.seq;
                    id1 := lrr.id1;
                    id2 := lrr.id2;
                    id3 := lrr.id3;
                    cost := lrr.cost;
                    return next;
                    lrr := rr;
                end if;
            end if;
        end loop;
    end loop;

    seq1 := seq1 + 1;
    lrr.seq := seq1;

    seq := lrr.seq;
    id1 := lrr.id1;
    id2 := lrr.id2;
    id3 := lrr.id3;
    cost := lrr.cost;
    return next;
    return;
end;
$body$
language plpgsql VOLATILE
cost 100
rows 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_trspViaEdges(TEXT, INTEGER[], FLOAT[], BOOLEAN, BOOLEAN, TEXT)
IS 'pgr_trspViaEdges
- PROTOTYPE
- Parameters
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
  - ARRAY[Via edge identifiers]
  - ARRAY[fraction position on via edges]
  - directed
  - has reverse cost
- Optional parameters
  - turn_restrict_sql := NULL
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp.html
';
