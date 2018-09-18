/*PGR-GNU*****************************************************************

Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com

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


-- V2 signature
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    sql TEXT,
    source_vid INTEGER,
    target_vid INTEGER,
    directed BOOLEAN,
    has_reverse_cost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
new_sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated Signature of pgr_bdAstar';
    has_reverse =_pgr_parameter_check('astar', $1, false);
    new_sql = $1;
    IF (has_reverse != $5) THEN
        IF (has_reverse) THEN
            new_sql = 'SELECT id, source, target, cost FROM (' || $1 || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    RETURN query SELECT seq-1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM _pgr_bdAstar(new_sql, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed);
  END
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;

