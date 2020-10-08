/*PGR-GNU*****************************************************************

Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Developer:
Copyright (c) 2019 Celia Virginia Vergara Castillo

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

CREATE OR REPLACE FUNCTION pgr_version()
RETURNS TEXT AS
$BODY$
    SELECT '${PGROUTING_VERSION}${PGROUTING_VERSION_DEV}'::varchar AS pgr_version;
$BODY$
LANGUAGE sql IMMUTABLE;

COMMENT ON FUNCTION pgr_version() IS
'pgr_version
- Documentation
  - ${PGROUTING_DOC_LINK}/pgr_version.html
';
