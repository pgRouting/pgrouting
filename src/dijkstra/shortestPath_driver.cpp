/*PGR-GNU*****************************************************************
File: shortestPath_driver.cpp

Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copying this file (or a derivative) within pgRouting code add the following:

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
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

#include "drivers/shortestPath_driver.hpp"

#include <algorithm>
#include <sstream>
#include <deque>
#include <vector>
#include <limits>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <cstdint>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/combinations.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "dijkstra/dijkstra.hpp"
#include "withPoints/withPoints.hpp"

namespace {

// TODO(vicky) This should be in its own file
char
estimate_drivingSide(char driving_side, int32_t which) {
    char d_side = static_cast<char>(tolower(driving_side));
    if (!((d_side == 'r') || (d_side == 'l') || (d_side == 'b'))) {
        d_side = ' ';
    }
    if (which == 0) {
        return ' ';
    } else if (which == 1) {
        if (d_side == ' ') {
            throw std::make_pair(std::string("Invalid value of 'driving side'"),
                    std::string("Valid value are 'r', 'l', 'b'"));
        }
    } else {
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

void
post_process(std::deque<pgrouting::Path> &paths, bool only_cost, bool normal, size_t n_goals, bool global) {
    using pgrouting::Path;
    paths.erase(std::remove_if(paths.begin(), paths.end(),
                [](const Path &p) {
                    return p.size() == 0;
                }),
                paths.end());
    using difference_type = std::deque<double>::difference_type;

    if (!normal) {
        for (auto &path : paths) path.reverse();
    }

    if (!only_cost) {
        for (auto &p : paths) {
            p.recalculate_agg_cost();
        }
    }

    if (n_goals != (std::numeric_limits<size_t>::max)()) {
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.start_id() < e2.start_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.tot_cost() < e2.tot_cost();
                });
        if (global && n_goals < paths.size()) {
            paths.erase(paths.begin() + static_cast<difference_type>(n_goals), paths.end());
        }
    } else {
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.start_id() < e2.start_id();
                });
    }
}

}  // namespace


void
do_shortestPath(
        const std::string &edges_sql,
        const std::string &points_sql,
        const std::string &combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        bool directed,
        bool only_cost,
        bool normal,
        int64_t n_goals,
        bool global,
        char driving_side,
        bool details,

        int32_t which,
        bool &is_matrix,
        Path_rt* &return_tuples, size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;

    std::string hint = "";

    try {
        pgassert(!edges_sql.empty());

        using pgrouting::pgget::get_edges;
        using pgrouting::pgget::get_points;
        using pgrouting::utilities::get_combinations;
        using pgrouting::UndirectedGraph;
        using pgrouting::DirectedGraph;

        using pgrouting::algorithms::dijkstra;

        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, normal, is_matrix);
        hint = "";

        if (combinations.empty() && !combinations_sql.empty()) {
            notice << "No (source, target) pairs found";
            log << combinations_sql;
            return;
        }

        std::string enop;
        std::string eofp;
        std::vector<Edge_t> edges;
        std::vector<Edge_t> edges_of_points;
        std::vector<Point_on_edge_t> points;

        if (points_sql.empty()) {
            hint = edges_sql;
            edges = get_edges(edges_sql, normal, false);
            hint = "";
        } else {
            get_new_queries(edges_sql, points_sql, eofp, enop);

            hint = points_sql;
            points = get_points(std::string(points_sql));

            hint = eofp;
            edges_of_points = !eofp.empty()? get_edges(eofp, normal, false) : std::vector<Edge_t>();

            hint = enop;
            edges = !enop.empty()? get_edges(enop, normal, false) : std::vector<Edge_t>();
            hint = "";

            if (edges.empty() && edges_of_points.empty()) {
                notice << "No edges found";
                return;
            }
        }

        /*
         * processing points
         */
        pgrouting::Pg_points_graph pg_graph(points, edges_of_points,
                normal,
                estimate_drivingSide(driving_side, which),
                directed);

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            return;
        }
        auto new_edges = pg_graph.new_edges();

        edges.insert(edges.end(), new_edges.begin(), new_edges.end());

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }

        size_t n = n_goals <= 0? (std::numeric_limits<size_t>::max)() : static_cast<size_t>(n_goals);

        std::deque<Path> paths;
        if (directed) {
            DirectedGraph graph;
            graph.insert_edges(edges);

            switch (which) {
                case 0:
                case 101:
                case 1: {
                            paths =  dijkstra(graph, combinations, only_cost, n);
                        }
            }
        } else {
            UndirectedGraph graph;
            graph.insert_edges(edges);

            switch (which) {
                case 0:
                case 101:
                case 1: {
                            paths =  dijkstra(graph, combinations, only_cost, n);
                        }
            }
        }

        post_process(paths, only_cost, normal, n, global);

        if (!details) {
            for (auto &path : paths) path = pg_graph.eliminate_details(path);
        }

        auto count = count_tuples(paths);

        if (count == 0) {
            log << "No paths found";
            return;
        }

        return_tuples = pgr_alloc(count, return_tuples);
        return_count = (collapse_paths(&return_tuples, paths));
    } catch (AssertFailedException &except) {
        err << except.what();
    } catch (const std::pair<std::string, std::string>& ex) {
        err << ex.first;
        log << ex.second;
    } catch (const std::string &ex) {
        err << ex;
        log << hint;
    } catch (std::exception &except) {
        err << except.what();
    } catch (...) {
        err << "Caught unknown exception!";
    }
}
