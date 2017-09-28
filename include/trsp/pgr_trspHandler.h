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
aint64_t with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#ifndef INCLUDE_TRSP_PGR_TRSPHANDLER_H_
#define INCLUDE_TRSP_PGR_TRSPHANDLER_H_

#include <stdlib.h>

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <utility>
#include <functional>
#include <limits>


#include "cpp_common/basePath_SSEC.hpp"
#include "trsp/edgeInfo.h"
#include "trsp/rule.h"

namespace pgrouting {
namespace trsp {




class Pgr_trspHandler {


    /*
     * Used in the priority queue
     */
    typedef std::pair<double, std::pair<int64_t, bool>> PDP;

    typedef struct {
        int64_t ed_ind[2];
        int64_t v_pos[2];
    } Predecessor;

    typedef std::map<int64_t, std::vector<Rule> > RuleTable;
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
            const int64_t start_vertex,
            const int64_t end_vertex,
            const bool directed,
            const std::vector<Rule> &ruleList);

    Pgr_trspHandler(void);
    ~Pgr_trspHandler(void) = default;

    int my_dijkstra1(int64_t start_vertex, int64_t end_vertex,
                    size_t edge_count, char** err_msg);


    Path initializeAndProcess(
            pgr_edge_t *edges,
            size_t edge_count,

            const std::vector<Rule> &ruleList,

            const int64_t start_vertex,
            const int64_t end_vertex,

            bool directed,

            char **err_msg);


    void clear();

 private:
    bool construct_graph(pgr_edge_t *edges, const size_t edge_count,
                         const bool directed);

    int initialize_restrictions(
            const std::vector<Rule> &ruleList);

    void initialize_que();

    Path process_trsp(
            size_t edge_count,
            char **err_msg);

    EdgeInfo* dijkstra_exploration(
            EdgeInfo* cur_edge,
            int64_t &cur_node);



    void explore(int64_t cur_node, const EdgeInfo cur_edge, bool isStart,
            const std::vector<int64_t> &vecIndex);

    double getRestrictionCost(int64_t cur_node, const EdgeInfo &new_edge,
            bool isStart);
    bool addEdge(const pgr_edge_t edgeIn);
    bool connectEdge(EdgeInfo& firstEdge, EdgeInfo& secondEdge,
            bool bIsStartNodeSame);
    Path get_single_cost(
            double total_cost);

    double construct_path(int64_t ed_id, int64_t v_pos);
    void init();
    int64_t renumber_edges(
            pgr_edge_t *edges,
            const size_t edge_count) const;


 private:
    std::vector<EdgeInfo> m_vecEdgeVector;
    std::map<int64_t, int64_t> m_mapEdgeId2Index;
    std::map<int64_t, std::vector<int64_t>> m_mapNodeId2Edge;

    int64_t max_node_id;
    int64_t max_edge_id;
    int64_t m_lStartEdgeId;
    int64_t m_lEndEdgeId;
    double m_dStartpart;
    double m_dEndPart;
    bool isStartVirtual;
    bool isEndVirtual;
    int64_t m_start_vertex;
    int64_t m_end_vertex;
    int64_t m_min_id;

    Path m_vecPath;

    std::vector<Predecessor> parent;
    std::vector<CostHolder> m_dCost;

    RuleTable m_ruleTable;
    bool m_bIsturnRestrictOn;
    bool m_bIsGraphConstructed;

    /*
     * priority queue
     */
    std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > que;
};


}  // namespace trsp
}  // namespace pgrouting

#endif  // INCLUDE_TRSP_PGR_TRSPHANDLER_H_
