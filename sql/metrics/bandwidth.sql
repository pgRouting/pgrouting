/*PGR-GNU*****************************************************************

File: bandwidth.sql

Template:
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Function developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

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

--v4.0
CREATE FUNCTION pgr_bandwidth(
    TEXT,    -- edges_sql (required)

    OUT bandwidth BIGINT   
)
RETURNS BIGINT
AS
$BODY$
    SELECT bandwidth FROM _pgr_bandwidth(_pgr_get_statement($1));
$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_bandwidth(TEXT)
IS 'pgr_bandwidth
- EXPERIMENTAL
- Calculates the bandwidth of graph components based on current vertex ordering.
- Parameters:
    - edges SQL with columns: id, source, target, cost [, reverse_cost]
- Returns:
    - bandwidth BIGINT
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_bandwidth.html
';
