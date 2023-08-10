/*PGR-GNU*****************************************************************
File: ithBandwidth.sql

Generated with Template by:
Copyright (c) 2023 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2023 Sanskar Bhushan
Mail: sbdtu5498@gmail.com

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

----------------------------------
-- pgr_ithBandwidth
----------------------------------


--v3.6.0
CREATE FUNCTION pgr_ithBandwidth(
    TEXT,    -- edges_sql (required)

    OUT vertex_id BIGINT,
    OUT ith_bandwidth BIGINT)
RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN QUERY
    SELECT *
    FROM _pgr_ithBandwidth(_pgr_get_statement($1));
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_ithBandwidth(TEXT)
IS 'pgr_ithBandwidth
- EXPERIMENTAL
- Parameters:
    - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_ithBandwidth.html
';
