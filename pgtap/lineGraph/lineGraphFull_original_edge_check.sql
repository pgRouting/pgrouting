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

\i setup.sql

SELECT plan(1);

CREATE or REPLACE FUNCTION lineGraphFullOriginalEdgeCheck(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
turnpenalty_sql TEXT;
original_sql TEXT;
inner_sql TEXT;
BEGIN

  original_sql := 'WITH going_edges AS (SELECT count(*) FROM edge_table WHERE cost > 0), coming_edges AS (SELECT count(*) FROM edge_table WHERE reverse_cost > 0) SELECT a.count + b.count FROM going_edges AS a, coming_edges AS b';

  inner_sql := 'SELECT id AS id, source AS source, target AS target, cost, reverse_cost FROM edge_table';

  turnpenalty_sql := 'SELECT count(*) FROM pgr_lineGraphFull($$'||inner_sql||'$$) where cost = 1';

  RETURN query SELECT set_eq(original_sql, turnpenalty_sql, turnpenalty_sql);

END
$BODY$
language plpgsql;

SELECT * from lineGraphFullOriginalEdgeCheck();


SELECT * FROM finish();
ROLLBACK;

