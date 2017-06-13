/*PGR-GNU*****************************************************************
File: areaContraction.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Ankur Shukla
Mail: work.ankurshukla@gmail.com

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

CREATE OR REPLACE FUNCTION pgr_areaContraction(
    TEXT, --edges_sql
    ANYARRAY, --border_nodes
    directed BOOLEAN DEFAULT true,
    OUT seq integer,
    OUT type TEXT,
    OUT id BIGINT,
    OUT contracted_vertices BIGINT[],
    OUT source BIGINT,
    OUT target BIGINT,
    OUT cost float)

RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'areaContraction'
LANGUAGE c IMMUTABLE STRICT;
