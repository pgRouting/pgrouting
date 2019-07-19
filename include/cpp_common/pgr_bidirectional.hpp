/*PGR-GNU*****************************************************************
File: pgr_bdAstar.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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

/*! @file */

#ifndef INCLUDE_CPP_COMMON_PGR_BIDIRECTIONAL_HPP_
#define INCLUDE_CPP_COMMON_PGR_BIDIRECTIONAL_HPP_
#pragma once


#include <boost/config.hpp>

#if BOOST_VERSION_OK
#include <boost/graph/dijkstra_shortest_paths.hpp>
#else
#include "boost/dijkstra_shortest_paths.hpp"
#endif

#include <boost/graph/adjacency_list.hpp>

#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <limits>
#include <functional>
#include <numeric>


#include "cpp_common/pgr_assert.h"

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"


namespace pgrouting {
namespace bidirectional {


template < typename G >
class Pgr_bidirectional {
 protected:
    typedef typename G::V V;
    typedef typename G::E E;

    typedef std::pair<double, V> Cost_Vertex_pair;
    typedef typename std::priority_queue<
        Cost_Vertex_pair,
        std::vector<Cost_Vertex_pair>,
        std::greater<Cost_Vertex_pair> > Priority_queue;


 public:
    explicit Pgr_bidirectional(G &pgraph):
        graph(pgraph),
        INF((std::numeric_limits<double>::max)()),
        best_cost(0) {
        m_log << "constructor\n";
    }

    ~Pgr_bidirectional() = default;

    std::string log() const {return m_log.str();}
    void clean_log() {m_log.clear();}
    void clear() {
        while (!forward_queue.empty()) forward_queue.pop();
        while (!backward_queue.empty()) backward_queue.pop();

        backward_finished.clear();
        backward_edge.clear();
        backward_predecessor.clear();
        backward_cost.clear();

        forward_finished.clear();
        forward_edge.clear();
        forward_predecessor.clear();
        forward_cost.clear();
    }


 protected:
    void initialize() {
        m_log << "initializing\n";
        clear();
        forward_predecessor.resize(graph.num_vertices());
        forward_finished.resize(graph.num_vertices(), false);
        forward_edge.resize(graph.num_vertices(), -1);
        forward_cost.resize(graph.num_vertices(), INF);
        std::iota(forward_predecessor.begin(), forward_predecessor.end(), 0);

        backward_predecessor.resize(graph.num_vertices());
        backward_finished.resize(graph.num_vertices(), false);
        backward_edge.resize(graph.num_vertices(), -1);
        backward_cost.resize(graph.num_vertices(), INF);
        std::iota(backward_predecessor.begin(), backward_predecessor.end(), 0);

        v_min_node = -1;
        best_cost = INF;
    }

    Path bidirectional(bool only_cost) {
        m_log << "bidir_astar\n";

        Pgr_bidirectional< G >::initialize();

        forward_cost[v_source] = 0;
        forward_queue.push(std::make_pair(0.0, v_source));


        backward_cost[v_target] = 0;
        backward_queue.push(std::make_pair(0.0, v_target));

        while (!forward_queue.empty() &&  !backward_queue.empty()) {
            auto forward_node = forward_queue.top();
            auto backward_node = backward_queue.top();
            /*
             * done: there is no path with lower cost
             */
            if (forward_node.first == INF || backward_node.first == INF) {
                break;
            }

            /*
             * Explore from the cheapest side
             */
            if (backward_node.first < forward_node.first) {
                backward_queue.pop();
                if (!backward_finished[backward_node.second]) {
                    explore_backward(backward_node);
                }
                if (found(backward_node.second)) {
                    break;
                }
            } else {
                forward_queue.pop();
                if (!forward_finished[forward_node.second]) {
                    explore_forward(forward_node);
                }
                if (found(forward_node.second)) {
                    break;
                }
            }
        }

        if (best_cost == INF) return Path();

        Path forward_path(
                graph,
                v_source,
                v_min_node,
                forward_predecessor,
                forward_cost,
                false,
                true);
        Path backward_path(
                graph,
                v_target,
                v_min_node,
                backward_predecessor,
                backward_cost,
                false,
                false);
        m_log << forward_path;
        backward_path.reverse();
        m_log << backward_path;
        forward_path.append(backward_path);
        auto p = Path(graph, forward_path, only_cost);
        m_log << forward_path;
        m_log << p;
        return p;
    }



    bool found(const V &node) {
        /*
         * Update common node
         */
        if (forward_finished[node] && backward_finished[node]) {
            if (best_cost >= forward_cost[node] + backward_cost[node]) {
                v_min_node = node;
                best_cost =  forward_cost[node] + backward_cost[node];
                return false;
            } else {
                return true;
            }
        }
        return false;
    }

    virtual
    void explore_forward(const Cost_Vertex_pair &node) = 0;

    virtual
    void explore_backward(const Cost_Vertex_pair &node) = 0;

 protected:
    G &graph;
    V v_source;  //!< source descriptor
    V v_target;  //!< target descriptor
    V v_min_node;  //!< target descriptor

    double INF;  //!< infinity

    double best_cost;
    bool cost_only;

    mutable std::ostringstream m_log;
    Priority_queue forward_queue;
    Priority_queue backward_queue;

    std::vector<bool> backward_finished;
    std::vector<int64_t> backward_edge;
    std::vector<V> backward_predecessor;
    std::vector<double> backward_cost;

    std::vector<bool> forward_finished;
    std::vector<int64_t> forward_edge;
    std::vector<V> forward_predecessor;
    std::vector<double> forward_cost;
};

}  // namespace bidirectional
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_PGR_BIDIRECTIONAL_HPP_
