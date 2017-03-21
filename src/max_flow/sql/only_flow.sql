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

/***********************************
        ONE TO ONE
***********************************/

CREATE OR REPLACE FUNCTION pgr_maxFlow(
    edges_sql TEXT,
    source_vertices BIGINT,
    sink_vertices BIGINT
    )
  RETURNS BIGINT AS
  $BODY$
        SELECT coalesce(sum(flow), 0)::BIGINT
        FROM _pgr_maxflow(_pgr_get_statement($1), $2, $3, algorithm := 'push_relabel', only_flow := true);
  $BODY$
  LANGUAGE SQL VOLATILE;

/***********************************
        ONE TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_maxFlow(
    edges_sql TEXT,
    source_vertices BIGINT,
    sink_vertices ANYARRAY
    )
  RETURNS BIGINT AS
  $BODY$
        SELECT flow
        FROM _pgr_maxflow(_pgr_get_statement($1), $2, $3, algorithm := 'push_relabel', only_flow := true);
  $BODY$
  LANGUAGE SQL VOLATILE;

/***********************************
        MANY TO ONE
***********************************/

CREATE OR REPLACE FUNCTION pgr_maxFlow(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices BIGINT
    )
  RETURNS BIGINT AS
  $BODY$
        SELECT flow
        FROM _pgr_maxflow(_pgr_get_statement($1), $2, $3, algorithm := 'push_relabel', only_flow := true);
  $BODY$
  LANGUAGE SQL VOLATILE;

/***********************************
        MANY TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_maxFlow(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices ANYARRAY
    )
  RETURNS BIGINT AS
  $BODY$
        SELECT flow
        FROM _pgr_maxflow(_pgr_get_statement($1), $2, $3, algorithm := 'push_relabel', only_flow := true);
  $BODY$
  LANGUAGE SQL VOLATILE;
