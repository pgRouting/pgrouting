/*PGR-GNU*****************************************************************

File: betweennessCentrality.sql

Template:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function developer:
Copyright (c) 2024 Arun Thakur
Mail: bedupako12mas at gmail.com

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

--v3.7
CREATE FUNCTION pgr_betweennessCentrality(
    TEXT,    -- edges_sql (required)
    directed BOOLEAN DEFAULT true,

    OUT vid BIGINT,
    OUT centrality FLOAT)
RETURNS SETOF RECORD AS
$BODY$

    SELECT vid, centrality
    FROM _pgr_betweennesscentrality(_pgr_get_statement($1), $2);

$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_betweennessCentrality(TEXT, BOOLEAN)
IS 'pgr_betweennessCentrality
- Parameters:
    - edges SQL with columns: source, target, cost [,reverse_cost])
- Optional Parameters:
    - directed := true
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_centrality.html
';
