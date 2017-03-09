/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli@gmail.com

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

/*FOR USERS DOCUMENTATION

The following applies to:
    pgr_maxFlowBoykovKolmogorov
    pgr_maxFlowPushRelabel
    pgr_maxFlowEdmondsKarp
    pgr_maxFlow


pgr_flow_parameters_start

Description of the parameters of the signatures
...............................................................................

============== ================== ======== =================================================
Column         Type               Default     Description
============== ================== ======== =================================================
**edges_sql**  ``TEXT``                    SQL query as described above.
**start_vid**  ``BIGINT``                  Identifier of the starting vertex of the flow.
**start_vids** ``ARRAY[BIGINT]``           Array of identifiers of the starting vertices of the flow.
**end_vid**    ``BIGINT``                  Identifier of the ending vertex of the flow.
**end_vids**   ``ARRAY[BIGINT]``           Array of identifiers of the ending vertices of the flow.
============== ================== ======== =================================================

pgr_flow_parameters_end

FOR-USER*/



/***********************************
        ONE TO ONE
-- 1 PushRelabel
-- 2 boykov_kolmogorov
-- 3 edmonds_karp
***********************************/


CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    BIGINT,
    BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    BIGINT,
    BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    BIGINT,
    BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], 3);
  $BODY$
  LANGUAGE sql VOLATILE;

/***********************************
        ONE TO MANY
***********************************/


CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    BIGINT,
    targets ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    BIGINT,
    targets ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    BIGINT,
    targets ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], 3);
  $BODY$
  LANGUAGE sql VOLATILE;

/***********************************
        MANY TO ONE
***********************************/


CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    sources ANYARRAY,
    BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    sources ANYARRAY,
    BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    sources ANYARRAY,
    BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], 3);
  $BODY$
  LANGUAGE sql VOLATILE;

/***********************************
        MANY TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    sources ANYARRAY,
    targets ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    sources ANYARRAY,
    targets ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    sources ANYARRAY,
    targets ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], 3);
  $BODY$
  LANGUAGE sql VOLATILE;

