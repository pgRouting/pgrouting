/*PGR-GNU*****************************************************************

FILE: pgr_trspHandler.h

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

#ifndef INCLUDE_TRSP_PGR_TRSPHANDLER_H_
#define INCLUDE_TRSP_PGR_TRSPHANDLER_H_

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


#include "cpp_common/basePath_SSEC.hpp"
#include "trsp/edgeInfo.h"
#include "cpp_common/rule.h"

namespace pgrouting {
namespace trsp {




class Pgr_trspHandler {
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
             return v_pos[i] == ILLEGAL;}

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
    Pgr_trspHandler(
            pgr_edge_t *edges,
            const size_t edge_count,
            const bool directed,
            const std::vector<Rule> &ruleList);

    Pgr_trspHandler(void) = delete;
    ~Pgr_trspHandler(void) = default;


    Path process(
            const int64_t start_vertex,
            const int64_t end_vertex);

    std::deque<Path> process(
            const std::vector<int64_t> sources,
            const std::vector<int64_t> targets);


    void clear();

 private:
    void construct_graph(
            pgr_edge_t *edges,
            const size_t edge_count,
            const bool directed);

    int initialize_restrictions(
            const std::vector<Rule> &ruleList);

    void initialize_que();

    Path process_trsp(
            size_t edge_count);

    EdgeInfo dijkstra_exploration();



    void explore(
            int64_t cur_node,
            const EdgeInfo cur_edge,
            bool isStart);

    double getRestrictionCost(
            int64_t cur_node,
            const EdgeInfo &new_edge,
            bool isStart);
    bool addEdge(const pgr_edge_t edgeIn);

    void connectStartEdge(
            size_t firstEdge_idx,
            size_t secondEdge_idx);

    void connectEndEdge(
            size_t firstEdge_idx,
            size_t secondEdge_idx);

    double construct_path(int64_t ed_id, Position pos);


    int64_t renumber_edges(
            pgr_edge_t *edges,
            const size_t edge_count) const;

    void  add_to_que(
            double cost,
            size_t e_idx,
            bool isStart);

    double get_tot_cost(
            double cost,
            size_t edge_idx,
            bool isStart);

 private:
    std::vector<EdgeInfo> m_edges;

    /**
     * Used only to veryfy that there are no reppetitions inserted
     * the way it orks, repeating edges id is not allowed
     */
    std::map<int64_t, int64_t> m_mapEdgeId2Index;

    /**
     * m_adjacency[vertex] = {edges}
     */
    std::map<int64_t, std::vector<size_t>> m_adjacency;


    int64_t m_start_vertex;
    int64_t m_end_vertex;

    /*
     * Used in dijkstra_exploration
     */
    int64_t current_node;

    int64_t m_min_id;

    Path m_path;

    std::vector<Predecessor> m_parent;
    std::vector<CostHolder> m_dCost;

    std::map<int64_t, std::vector<Rule>> m_ruleTable;

    /*
     * priority queue
     */
    std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > que;
};


}  // namespace trsp
}  // namespace pgrouting

#endif  // INCLUDE_TRSP_PGR_TRSPHANDLER_H_
