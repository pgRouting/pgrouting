/*PGR-GNU*****************************************************************

Copyright (c) 2018 pgRouting developers
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

\i setup.sql

SELECT plan(24);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

CREATE or REPLACE FUNCTION ComparePrimDDKruskalDD(distance FLOAT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner1_sql TEXT;
inner2_sql TEXT;
prim_sql TEXT;
kruskal_sql TEXT;
vids TEXT;
data TEXT;
BEGIN
    inner1_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
    inner2_sql := 'SELECT id, source, target, cost FROM edge_table';

    data := ' seq, depth, start_vid, node, edge,  cost::text, agg_cost::text ';
    vids := ' ARRAY[2, 5] ';


    -----------------------
    -- Single vertex
    -- with reverse cost
    -----------------------
    prim_sql := 'SELECT ' || data || ' FROM pgr_primDD($$' || inner1_sql || '$$, 2, ' || distance || ')';
    kruskal_sql := 'SELECT ' || data || ' FROM pgr_kruskalDD($$' || inner1_sql || '$$, 2, ' || distance || ')';
    RETURN query SELECT set_eq(prim_sql, kruskal_sql, prim_sql);

    -----------------------
    -- Single vertex
    -- with out reverse cost
    -----------------------
    prim_sql := 'SELECT ' || data || ' FROM pgr_primDD($$' || inner2_sql || '$$, 2, ' || distance || ')';
    kruskal_sql := 'SELECT ' || data || ' FROM pgr_kruskalDD($$' || inner2_sql || '$$, 2, ' || distance || ')';
    RETURN query SELECT set_eq(prim_sql, kruskal_sql, prim_sql);

    -----------------------
    -- Multiple vertex
    -- with reverse cost
    -----------------------
    prim_sql := 'SELECT ' || data || ' FROM pgr_primDD($$' || inner1_sql || '$$, ' || vids || ', ' || distance || ')';
    kruskal_sql := 'SELECT ' || data || ' FROM pgr_kruskalDD($$' || inner1_sql || '$$, ' || vids || ', ' || distance || ')';
    RETURN query SELECT set_eq(prim_sql, kruskal_sql, prim_sql);

    -----------------------
    -- Multiple vertex
    -- with out reverse cost
    -----------------------
    prim_sql := 'SELECT ' || data || ' FROM pgr_primDD($$' || inner2_sql || '$$, ' || vids || ', ' || distance || ')';
    kruskal_sql := 'SELECT ' || data || ' FROM pgr_kruskalDD($$' || inner2_sql || '$$, ' || vids || ', ' || distance || ')';
    RETURN query SELECT set_eq(prim_sql, kruskal_sql, prim_sql);

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from ComparePrimDDKruskalDD(0);
SELECT * from ComparePrimDDKruskalDD(1);
SELECT * from ComparePrimDDKruskalDD(2);
SELECT * from ComparePrimDDKruskalDD(3);
SELECT * from ComparePrimDDKruskalDD(4);
SELECT * from ComparePrimDDKruskalDD(5);


SELECT * FROM finish();
ROLLBACK;
