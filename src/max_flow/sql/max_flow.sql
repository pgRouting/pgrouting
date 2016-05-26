/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
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

CREATE OR REPLACE FUNCTION pgr_maxFlow(
    edges_sql TEXT,
    source BIGINT,
    sink BIGINT,
    OUT id integer,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT residual_capacity integer,
    OUT reverse_residual_capacity integer,
    OUT flow integer,
    OUT reverse_flow integer
    )
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'max_flow'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_pushRelabel(
    edges_sql TEXT,
    source BIGINT,
    sink BIGINT,
    OUT id integer,
    OUT tail BIGINT,
    OUT head BIGINT,
    OUT residual_capacity integer,
    OUT reverse_residual_capacity integer,
    OUT flow integer,
    OUT reverse_flow integer
    )
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'max_flow'
    LANGUAGE c IMMUTABLE STRICT;