/*PGR-GNU*****************************************************************
File: utilities.cpp

Copyright (c) 2026-2026 pgRouting developers
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

#include "cpp_common/utilities.hpp"

#include <set>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "c_common/enums.h"
#include "c_types/mst_rt.h"

namespace pgrouting {

std::string
get_name(Which which) {
    switch (which) {
        case SLOAN:
            return "pgr_sloanOrdering";
            break;
        case CUTCHILL:
            return "pgr_cuthillMckeeOrdering";
            break;
        case KING:
            return "pgr_kingOrdering";
            break;
        case TOPOSORT:
            return "pgr_topologicalSort";
            break;
        case FLOYD:
            return "pgr_floydWarshall";
            break;
        case JOHNSON:
            return "pgr_johnson";
            break;
        case BANDWIDTH:
            return "pgr_bandwidth";
            break;
        case KRUSKAL:
            return "pgr_kruskal";
            break;
        case KRUSKALDD:
            return "pgr_kruskalDD";
            break;
        case KRUSKALDFS:
            return "pgr_kruskalDFS";
            break;
        case KRUSKALBFS:
            return "pgr_kruskalBFS";
            break;
        case PRIM:
            return "pgr_prim";
            break;
        case PRIMDD:
            return "pgr_primDD";
            break;
        case PRIMDFS:
            return "pgr_primDFS";
            break;
        case PRIMBFS:
            return "pgr_primBFS";
            break;
        case DFS:
            return "pgr_depthFirstSearch";
            break;
        case BFS:
            return "pgr_breadthFirstSearch";
            break;
        case DIJKSTRADD:
            return "pgr_drivingDistance";
            break;
        default:
            return "unknown";
            break;
    }
    return "unknown";
}

std::string
get_name(Which which, bool is_only_cost, bool is_near, bool is_matrix) {
    std::string base = "";
    std::string suffix = "";
    switch  (which) {
        case DIJKSTRA :
            base = "pgr_dijkstra";
            suffix = std::string(is_near? "Near" : "")
                + (is_only_cost? "Cost" : "")
                + (is_only_cost && is_matrix? "Matrix" : "");
            break;
        case BDDIJKSTRA :
            base = "pgr_bdDijkstra";
            suffix = std::string(is_only_cost? "Cost" : "") + (is_only_cost && is_matrix? "Matrix" : "");
            break;
        case EDWARDMOORE:
            base = "pgr_edwardMoore";
            break;
        case DAGSP :
            base = "pgr_dagShortestPath";
            break;
        case OLD_WITHPOINTS:
        case WITHPOINTS:
            base = "pgr_withPoints";
            suffix = std::string(is_only_cost? "Cost" : "") + (is_only_cost && is_matrix? "Matrix" : "");
            break;
        default : base = "unknown";
    }

    return base + suffix;
}

std::string
get_name(Which which, bool is_only_cost, bool is_matrix) {
    std::string base = "";
    std::string suffix = "";

    switch (which) {
        case ASTAR :
            base = "pgr_aStar";
            suffix = std::string(is_only_cost? "Cost" : "") + (is_only_cost && is_matrix? "Matrix" : "");
            break;
        case BDASTAR :
            base = "pgr_bdAstar";
            suffix = std::string(is_only_cost? "Cost" : "") + (is_only_cost && is_matrix? "Matrix" : "");
            break;
        default : base = "unknown";
    }

    return base + suffix;
}

char
estimate_drivingSide(char driving_side, Which which) {
    char d_side = static_cast<char>(tolower(driving_side));
    if (!((d_side == 'r') || (d_side == 'l') || (d_side == 'b'))) {
        d_side = ' ';
    }
    switch (which) {
        case ASTAR :
        case BDASTAR :
        case DAGSP :
        case EDWARDMOORE:
        case BDDIJKSTRA:
        case DIJKSTRA:
                return ' ';
                break;
        case WITHPOINTS:
                if (d_side == ' ') {
                    throw std::make_pair(std::string("Invalid value of 'driving side'"),
                            std::string("Valid value are 'r', 'l', 'b'"));
                }
                break;
        default:
                /* For the moment its old signature of pgr_withPoints */
                if (!((d_side == 'r') || (d_side == 'l'))) d_side = 'b';
    }
    return d_side;
}

void
get_new_queries(
        const std::string &edges_sql,
        const std::string &points_sql,
        std::string &edges_of_points_query,
        std::string &edges_no_points_query) {
    if (points_sql.empty()) {
        edges_no_points_query = edges_sql;
        return;
    }

    edges_of_points_query = std::string("WITH ")
        + " edges AS (" + edges_sql + "), "
        + " points AS (" + points_sql + ")"
        + " SELECT DISTINCT edges.* FROM edges JOIN points ON (id = edge_id)";

    edges_no_points_query  = std::string("WITH ")
        + " edges AS (" + edges_sql + "), "
        + " points AS (" + points_sql + ")"
        + " SELECT edges.*"
        + " FROM edges"
        + " WHERE NOT EXISTS (SELECT edge_id FROM points WHERE id = edge_id)";
}

std::vector<MST_rt>
only_root_result(const std::set<int64_t> &vids) {
    std::vector<MST_rt> results;
    if (vids.empty()) return results;
    for (auto const root : vids) {
        if (root != 0) results.push_back({root, 0, root, root, -1, 0.0, 0.0});
    }
    return results;
}

}  // namespace pgrouting
