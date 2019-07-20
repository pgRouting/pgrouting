/*PGR-GNU*****************************************************************

FILE: __FILENAME__

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
\echo Use "CREATE EXTENSION pgrouting" to load this file. \quit




--- -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
---
--- pgRouting provides geospatial routing functionality.
--- http://pgrouting.org
--- copyright
--- -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
---
---
--- This is free software; you can redistribute and/or modify it:
--- the terms of the GNU General Public Licence. See the COPYING file.
--- the terms of the MIT-X Licence. See the COPYING file.
---
--- The following functions have MIT-X licence:
---     pgr_version()
---     pgr_tsp(matrix float8[][], startpt integer, endpt integer DEFAULT -1, OUT seq integer, OUT id integer)
---     _pgr_makeDistanceMatrix(sqlin text, OUT dmatrix double precision[], OUT ids integer[])
---     pgr_analyzegraph(edge_table text,tolerance double precision,the_geom text default 'the_geom',id text default 'id',source text default 'source',target text default 'target',rows_where text default 'true')
---
---
--- All other functions are under GNU General Public Licence.
---
--- -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
--
-- WARNING: Any change in this file must be evaluated for compatibility.
--          Changes cleanly handled by postgis_upgrade.sql are fine,
--          other changes will require a bump in Major version.
--          Currently only function replaceble by CREATE OR REPLACE
--          are cleanly handled.
--
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


