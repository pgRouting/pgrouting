/*PGR-GNU*****************************************************************

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2017 Celia Virginia Vergara Castillo
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


------------------------
-- deprecated signatures
-----------------------

COMMENT ON FUNCTION pgr_astar(TEXT, INTEGER, INTEGER, BOOLEAN, BOOLEAN)
    IS 'pgr_astar(Deprecated signature)';

COMMENT ON FUNCTION pgr_bdAstar( TEXT, INTEGER, INTEGER, BOOLEAN, BOOLEAN)
    IS 'pgr_bdAstar(Deprecated signature)';

COMMENT ON FUNCTION pgr_bdDijkstra( TEXT, INTEGER, INTEGER, BOOLEAN, BOOLEAN)
    IS 'pgr_bdDijkstra(Deprecated signature)';

COMMENT ON FUNCTION pgr_dijkstra(TEXT, INTEGER, INTEGER, BOOLEAN, BOOLEAN)
    IS 'pgr_dijkstra(Deprecated signature)';

COMMENT ON FUNCTION pgr_drivingDistance(text,  BIGINT,  FLOAT8,  BOOLEAN,  BOOLEAN)
    IS 'pgr_drivingDistance(Deprecated signature)';

------------------------
-- Renamed /deprecated
-----------------------
COMMENT ON FUNCTION pgr_apspJohnson(TEXT)
    IS 'pgr_apspJohnson(Renamed function) use pgr_Johnson instead';

COMMENT ON FUNCTION pgr_apspWarshall(text, boolean, boolean)
    IS 'pgr_apspWarshall(Renamed function) use pgr_floydWarshall instead';

COMMENT ON FUNCTION pgr_kdijkstraPath( text, INTEGER, INTEGER ARRAY, BOOLEAN, BOOLEAN)
    IS 'pgr_kdijkstraPath(Renamed function) use pgr_dijkstra instead';

COMMENT ON FUNCTION pgr_kdijkstracost( text, INTEGER, INTEGER array, BOOLEAN, BOOLEAN)
    IS 'pgr_kDijkstraCost(Renamed function) use pgr_dijkstraCost instead';

COMMENT ON FUNCTION  pgr_maxFlowPushRelabel(TEXT, BIGINT, BIGINT)
    IS 'pgr_maxFlowPushRelabel(Renamed function) use pgr_pushRelabel instead';
COMMENT ON FUNCTION  pgr_maxFlowPushRelabel(TEXT, BIGINT, ANYARRAY)
    IS 'pgr_maxFlowPushRelabel(Renamed function) use pgr_pushRelabel instead';
COMMENT ON FUNCTION  pgr_maxFlowPushRelabel(TEXT, ANYARRAY, BIGINT)
    IS 'pgr_maxFlowPushRelabel(Renamed function) use pgr_pushRelabel instead';
COMMENT ON FUNCTION  pgr_maxFlowPushRelabel(TEXT, ANYARRAY, ANYARRAY)
    IS 'pgr_maxFlowPushRelabel(Renamed function) use pgr_pushRelabel instead';


COMMENT ON FUNCTION  pgr_maxFlowEdmondsKarp(TEXT, BIGINT, BIGINT)
    IS 'pgr_maxFlowEdmondsKarp(Renamed function) use pgr_edmondsKarp instead';
COMMENT ON FUNCTION  pgr_maxFlowEdmondsKarp(TEXT, BIGINT, ANYARRAY)
    IS 'pgr_maxFlowEdmondsKarp(Renamed function) use pgr_edmondsKarp instead';
COMMENT ON FUNCTION  pgr_maxFlowEdmondsKarp(TEXT, ANYARRAY, BIGINT)
    IS 'pgr_maxFlowEdmondsKarp(Renamed function) use pgr_edmondsKarp instead';
COMMENT ON FUNCTION  pgr_maxFlowEdmondsKarp(TEXT, ANYARRAY, ANYARRAY)
    IS 'pgr_maxFlowEdmondsKarp(Renamed function) use pgr_edmondsKarp instead';

COMMENT ON FUNCTION  pgr_maxFlowBoykovKolmogorov(TEXT, BIGINT, BIGINT)
    IS 'pgr_maxFlowBoykovKolmogorov(Renamed function) use pgr_boykovKolmogorov instead';
COMMENT ON FUNCTION  pgr_maxFlowBoykovKolmogorov(TEXT, BIGINT, ANYARRAY)
    IS 'pgr_maxFlowBoykovKolmogorov(Renamed function) use pgr_boykovKolmogorov instead';
COMMENT ON FUNCTION  pgr_maxFlowBoykovKolmogorov(TEXT, ANYARRAY, BIGINT)
    IS 'pgr_maxFlowBoykovKolmogorov(Renamed function) use pgr_boykovKolmogorov instead';
COMMENT ON FUNCTION  pgr_maxFlowBoykovKolmogorov(TEXT, ANYARRAY, ANYARRAY)
    IS 'pgr_maxFlowBoykovKolmogorov(Renamed function) use pgr_boykovKolmogorov instead';


COMMENT ON FUNCTION pgr_gsoc_vrppdtw( text, INTEGER, INTEGER)
    IS 'pgr_gsoc_vrppdtw(Renamed function) use pgr_pickDeliverEuclidean instead';

------------------------
-- Deprecated functions
-----------------------

COMMENT ON FUNCTION pgr_flipedges(geometry[])
    IS 'pgr_flipedges(Deprecated function)';

COMMENT ON FUNCTION pgr_texttopoints(text,  integer)
    IS 'pgr_texttopoints(Deprecated function)';

COMMENT ON FUNCTION pgr_pointstovids(pnts geometry[], edges text, tol float8)
    IS 'pgr_pointstovids(Deprecated function)';

COMMENT ON FUNCTION pgr_pointtoedgenode(edges text, pnt geometry, tol float8)
    IS 'pgr_pointtoedgenode(Deprecated function)';

COMMENT ON FUNCTION pgr_pointstodmatrix(geometry[], integer)
    IS 'pgr_pointstodmatrix(Deprecated function)';

COMMENT ON FUNCTION pgr_vidstodmatrix( integer[],  geometry[],  text, float8)
    IS 'pgr_vidstodmatrix(Deprecated function)';

COMMENT ON FUNCTION pgr_vidsToDMatrix(TEXT,  INTEGER[], BOOLEAN, BOOLEAN, BOOLEAN)
    IS 'pgr_vidstodmatrix(Deprecated function)';




COMMENT ON FUNCTION pgr_getTableName(IN tab text)
    IS 'pgr_getTableName(Deprecated function)';

COMMENT ON FUNCTION pgr_getColumnName(tab text, col text)
    IS 'pgr_getColumnName(Deprecated function)';

COMMENT ON FUNCTION pgr_isColumnInTable(tab text, col text)
    IS 'pgr_isColumnInTable(Deprecated function)';

COMMENT ON FUNCTION pgr_isColumnIndexed(tab text, col text)
    IS 'pgr_isColumnIndexed(Deprecated function)';


COMMENT ON FUNCTION pgr_quote_ident(idname text)
    IS 'pgr_quote_ident(Deprecated function)';

COMMENT ON FUNCTION pgr_versionless(v1 text, v2 text)
    IS 'pgr_versionless(Deprecated function)';

COMMENT ON FUNCTION pgr_startPoint(g geometry)
    IS 'pgr_startPoint(Deprecated function)';

COMMENT ON FUNCTION pgr_endPoint(g geometry)
    IS 'pgr_endPoint(Deprecated function)';
