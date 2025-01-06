/*PGR-GNU*****************************************************************
File: trspHandler.hpp

Copyright (c) 2017 pgRouting developers
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

#ifndef INCLUDE_TRSP_TRSPHANDLER_HPP_
#define INCLUDE_TRSP_TRSPHANDLER_HPP_

#include <stdlib.h>

#include <vector>
#include <deque>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <utility>
#include <functional>
#include <limits>
#include <set>
#include <cstdint>


#include "cpp_common/basePath_SSEC.hpp"
#include "trsp/edgeInfo.hpp"
#include "cpp_common/rule.hpp"
#include "cpp_common/messages.hpp"

namespace pgrouting {
namespace trsp {




class TrspHandler : public pgrouting::Pgr_messages {
    /**
     * Used in the priority queue
     */
    typedef std::pair<double, std::pair<int64_t, bool>> PDP;

    /** @brief predecesor edge
     *
     * because each row represents 2 edges, this enumeration
     * is needed for the predecessors
     * - C_EDGE: If the predecessor comes from the "cost" edge
     * - RC_EDGE: If the predecessor comes from the "reverse_cost" edge
     * - ILLEGAL: Its not a predecessor of anything
     *
     * The "legal" values are indices to vectors
     */
    enum Position {ILLEGAL = -1, RC_EDGE = 0, C_EDGE = 1};


    class Predecessor {
     public:
         Predecessor() :
             e_idx(2),
             v_pos(2) {
             for (auto &p : v_pos) p = ILLEGAL;
         }

         bool isIllegal(size_t i) {return v_pos[i] == ILLEGAL;}
         bool isIllegal(Position i) {
             pgassert(i != ILLEGAL);
             if (i == ILLEGAL) return true;
             return v_pos[static_cast<size_t>(i)] == ILLEGAL;}

         std::vector<size_t> e_idx;
         std::vector<Position> v_pos;
    };


    class CostHolder {
     public:
         CostHolder() {
             endCost =  startCost = (std::numeric_limits<double>::max)();
         }

     public:
         double startCost;
         double endCost;
    };


 public:
    TrspHandler(
            std::vector<Edge_t> &edges,
            const std::vector<Edge_t> &new_edges,
            const bool directed,
            const std::vector<Rule> &ruleList);

    TrspHandler(
            std::vector<Edge_t> &edges,
            const bool directed,
            const std::vector<Rule> &ruleList);

    ~TrspHandler(void) = default;

    std::deque<Path> process(const std::map<int64_t, std::set<int64_t>>&);

    void clear();

 private:
    void construct_graph(const std::vector<Edge_t>&, const std::vector<Edge_t>&, const bool);

    int initialize_restrictions(const std::vector<Rule>&);

    void initialize_que();

    Path process(const int64_t, const int64_t);

    Path process_trsp(size_t edge_count);

    EdgeInfo dijkstra_exploration();

    void explore(int64_t, const EdgeInfo, bool);

    double getRestrictionCost(int64_t, const EdgeInfo&, bool);

    bool addEdge(Edge_t, bool);

    void connectStartEdge(size_t, size_t);

    void connectEndEdge(size_t, size_t);

    double construct_path(int64_t, Position);

    void renumber_edges(std::vector<Edge_t>&);
    void renumber_edges(std::vector<Edge_t>&, std::vector<Edge_t>&);

    void  add_to_que(double, size_t, bool);

    double get_tot_cost(double, size_t, bool);

 private:
    std::vector<EdgeInfo> m_edges;

    /**
     * Used only to veryfy that there are no reppetitions inserted
     * the way it works, repeating edges id is not allowed
     * TODO when using points edges id are repeated
     */
    std::map<int64_t, int64_t> m_mapEdgeId2Index;

    /**
     * m_adjacency[vertex] = {edges}
     */
    std::map<int64_t, std::vector<size_t>> m_adjacency;

    /* id -> idx */
    std::map<int64_t, int64_t> m_id_to_idx;
    /* idx -> id */
    std::map<int64_t, int64_t> m_idx_to_id;

    int64_t m_start_vertex;
    int64_t m_end_vertex;

    /*
     * Used in dijkstra_exploration
     */
    int64_t m_current_node;

    Path m_path;

    std::vector<Predecessor> m_parent;
    std::vector<CostHolder> m_dCost;

    std::map<int64_t, std::vector<Rule>> m_ruleTable;

    /*
     * priority queue
     */
    std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP>> m_que;
};


}  // namespace trsp
}  // namespace pgrouting

#endif  // INCLUDE_TRSP_TRSPHANDLER_HPP_
