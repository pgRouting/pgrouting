/*PGR-GNU*****************************************************************

File: pgr_trspHandler.cpp

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
aint64_t with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

#include "trsp/pgr_trspHandler.h"

#include <functional>
#include <utility>
#include <queue>
#include <vector>
#include <limits>

#include "cpp_common/pgr_assert.h"

// -------------------------------------------------------------------------
Pgr_trspHandler::Pgr_trspHandler(
        edge_t *edges,
        const size_t edge_count,
        const int64_t start_vertex, 
        const int64_t end_vertex,
        const bool directed,
        const bool has_reverse_cost,
        const std::vector<PDVI> &ruleList) :
    m_lStartEdgeId(-1),
    m_lEndEdgeId(0),
    m_dStartpart(0.0),
    m_dEndPart(0.0),
    isStartVirtual(false),
    isEndVirtual(false),
    m_start_vertex(start_vertex),
    m_end_vertex(end_vertex),
    m_ruleTable(),
    m_bIsturnRestrictOn(false),
    m_bIsGraphConstructed(false)
{
    init();
    initialize_restrictions(
            ruleList);
    construct_graph(edges,
            edge_count,
            directed,
            has_reverse_cost);
    pgassert(m_bIsturnRestrictOn);
    pgassert(m_bIsGraphConstructed);
}

// -------------------------------------------------------------------------
Pgr_trspHandler::Pgr_trspHandler(void) {
    m_lStartEdgeId = -1;
    m_lEndEdgeId = 0;
    m_dStartpart = 0.0;
    m_dEndPart = 0.0;
    m_bIsturnRestrictOn = false;
    m_bIsGraphConstructed = false;
    init();
}


// -------------------------------------------------------------------------
void Pgr_trspHandler::init() {
    max_edge_id = 0;
    max_node_id = 0;
    isStartVirtual = false;
    isEndVirtual = false;
}


// -------------------------------------------------------------------------
void Pgr_trspHandler::clear() {
    m_vecEdgeVector.clear();
    parent.clear();
    m_dCost.clear();
}


// -------------------------------------------------------------------------
double Pgr_trspHandler::construct_path(int64_t ed_id, int64_t v_pos) {
    if (parent[ed_id].ed_ind[v_pos] == -1) {
        path_element_tt pelement;
        auto cur_edge = &m_vecEdgeVector[ed_id];
        if (v_pos == 0) {
            pelement.vertex_id = cur_edge->m_lStartNode;
            pelement.cost = cur_edge->m_dCost;
        } else {
            pelement.vertex_id = cur_edge->m_lEndNode;
            pelement.cost = cur_edge->m_dReverseCost;
        }
        pelement.edge_id = cur_edge->m_lEdgeID;

        m_vecPath.push_back(pelement);
        return pelement.cost;
    }
    double ret = construct_path(parent[ed_id].ed_ind[v_pos],
        parent[ed_id].v_pos[v_pos]);
    path_element_tt pelement;
    auto cur_edge = &m_vecEdgeVector[ed_id];
    if (v_pos == 0) {
        pelement.vertex_id = cur_edge->m_lStartNode;
        pelement.cost = m_dCost[ed_id].endCost - ret;  // cur_edge.m_dCost;
        ret = m_dCost[ed_id].endCost;
    } else {
        pelement.vertex_id = cur_edge->m_lEndNode;
        pelement.cost = m_dCost[ed_id].startCost - ret;
        ret = m_dCost[ed_id].startCost;
    }
    pelement.edge_id = cur_edge->m_lEdgeID;

    m_vecPath.push_back(pelement);

    return ret;
}


// -------------------------------------------------------------------------
double Pgr_trspHandler::getRestrictionCost(
    int64_t edge_ind,
    const GraphEdgeInfo& new_edge,
    bool isStart) {
    double cost = 0.0;
    int64_t edge_id = new_edge.m_lEdgeID;
    if (m_ruleTable.find(edge_id) == m_ruleTable.end()) {
        return(0.0);
    }
    std::vector<Rule> vecRules = m_ruleTable[edge_id];
    int64_t st_edge_ind = edge_ind;
    for (const auto &rule : vecRules) {
        bool flag = true;
        int64_t v_pos = (isStart?0:1);
        edge_ind = st_edge_ind;
        for (auto const &precedence : rule.precedencelist) {
            if (edge_ind == -1) {
                flag = false;
                break;
            }
            if (precedence != m_vecEdgeVector[edge_ind].m_lEdgeID) {
                flag = false;
                break;
            }
            auto parent_ind = parent[edge_ind].ed_ind[v_pos];
            v_pos = parent[edge_ind].v_pos[v_pos];
            edge_ind = parent_ind;
        }
        if (flag)
            cost += rule.cost;
    }
    return cost;
}


// -------------------------------------------------------------------------
void Pgr_trspHandler::explore(
    int64_t cur_node,
    const GraphEdgeInfo cur_edge,
    bool isStart,
    const LongVector &vecIndex) {
    double extCost = 0.0;
    double totalCost;
    for (const auto &index : vecIndex) {
        auto new_edge = &m_vecEdgeVector[index];
        extCost = 0.0;
        if (m_bIsturnRestrictOn) {
            extCost = getRestrictionCost(cur_edge.m_lEdgeIndex,
                 *new_edge, isStart);
        }
        if (new_edge->m_lStartNode == cur_node) {
            if (new_edge->m_dCost >= 0.0) {
                if (isStart)
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].endCost +
                    new_edge->m_dCost + extCost;
                else
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].startCost +
                    new_edge->m_dCost + extCost;
                if (totalCost < m_dCost[index].endCost) {
                    m_dCost[index].endCost = totalCost;
                    parent[new_edge->m_lEdgeIndex].v_pos[0] = (isStart?0:1);
                    parent[new_edge->m_lEdgeIndex].ed_ind[0] =
                     cur_edge.m_lEdgeIndex;
                    que.push(std::make_pair(totalCost,
                        std::make_pair(new_edge->m_lEdgeIndex, true)));
                }
            }
        } else {
            if (new_edge->m_dReverseCost >= 0.0) {
                if (isStart)
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].endCost +
                    new_edge->m_dReverseCost + extCost;
                else
                    totalCost = m_dCost[cur_edge.m_lEdgeIndex].startCost +
                    new_edge->m_dReverseCost + extCost;
                if (totalCost < m_dCost[index].startCost) {
                    m_dCost[index].startCost = totalCost;
                    parent[new_edge->m_lEdgeIndex].v_pos[1] = (isStart?0:1);
                    parent[new_edge->m_lEdgeIndex].ed_ind[1] =
                    cur_edge.m_lEdgeIndex;
                    que.push(std::make_pair(totalCost,
                        std::make_pair(new_edge->m_lEdgeIndex, false)));
                }
            }
        }
    }
}



// -------------------------------------------------------------------------
int Pgr_trspHandler::my_dijkstra1(int64_t start_vertex, int64_t end_vertex,
    size_t edge_count, char **err_msg) {
    if (!m_bIsGraphConstructed) {
        *err_msg = (char *)"Graph not Ready!";
        return -1;
    }
    unsigned int i;
    for (i = 0; i <= edge_count; i++) {
        m_dCost[i].startCost = 1e15;
        m_dCost[i].endCost = 1e15;
    }

    if (m_mapNodeId2Edge.find(start_vertex) == m_mapNodeId2Edge.end()) {
        *err_msg = (char *)"Source Not Found";
        clear();
        return -1;
    }

    if (m_mapNodeId2Edge.find(end_vertex) == m_mapNodeId2Edge.end()) {
        *err_msg = (char *)"Destination Not Found";
        clear();
        return -1;
    }

    std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > que;
    LongVector vecsource = m_mapNodeId2Edge[start_vertex];
    GraphEdgeInfo* cur_edge = NULL;

    for (const auto &source : vecsource) {
        auto cur_edge = &m_vecEdgeVector[source];
        if (cur_edge->m_lStartNode == start_vertex) {
            if (cur_edge->m_dCost >= 0.0) {
                m_dCost[cur_edge->m_lEdgeIndex].endCost = cur_edge->m_dCost;
                parent[cur_edge->m_lEdgeIndex].v_pos[0] = -1;
                parent[cur_edge->m_lEdgeIndex].ed_ind[0] = -1;
                que.push(std::make_pair(cur_edge->m_dCost,
                    std::make_pair(cur_edge->m_lEdgeIndex, true)));
                }
            } else {
                if (cur_edge->m_dReverseCost >= 0.0) {
                    m_dCost[cur_edge->m_lEdgeIndex].startCost =
                    cur_edge->m_dReverseCost;
                    parent[cur_edge->m_lEdgeIndex].v_pos[1] = -1;
                    parent[cur_edge->m_lEdgeIndex].ed_ind[1] = -1;
                    que.push(std::make_pair(cur_edge->m_dReverseCost,
                        std::make_pair(cur_edge->m_lEdgeIndex, false)));
                    }
            }
        }

        int64_t cur_node = -1;

        while (!que.empty()) {
            PDP cur_pos = que.top();
            que.pop();
            int64_t cured_index = cur_pos.second.first;
            auto cur_edge = &m_vecEdgeVector[cured_index];

            if (cur_pos.second.second) {  // explore edges connected to end node
                cur_node = cur_edge->m_lEndNode;
                if (cur_edge->m_dCost < 0.0)
                    continue;
                if (cur_node == end_vertex)
                    break;
                explore(cur_node, *cur_edge, true,
                    cur_edge->m_vecEndConnedtedEdge);
            } else {  // explore edges connected to start node
                cur_node = cur_edge->m_lStartNode;
                if (cur_edge->m_dReverseCost < 0.0)
                    continue;
                if (cur_node == end_vertex)
                    break;
                explore(cur_node, *cur_edge, false,
                    cur_edge->m_vecStartConnectedEdge);
            }
        }
        if (cur_node != end_vertex) {
            *err_msg = (char *)"Path Not Found";
            clear();
            return -1;
        } else {
            if (cur_node == cur_edge->m_lStartNode) {
                construct_path(cur_edge->m_lEdgeIndex, 1);
            } else {
                construct_path(cur_edge->m_lEdgeIndex, 0);
            }
            path_element_tt pelement;
            pelement.vertex_id = end_vertex;
            pelement.edge_id = -1;
            pelement.cost = 0.0;
            m_vecPath.push_back(pelement);
        }
    return 0;
}


// -------------------------------------------------------------------------
int Pgr_trspHandler::my_dijkstra4(edge_t *edges, size_t edge_count,
    int64_t start_edge_id, double start_part, int64_t end_edge_id, double end_part,
    bool directed, bool has_reverse_cost, path_element_tt **path,
    size_t *path_count, char **err_msg, const std::vector<PDVI> &ruleList) {
#if 1
    if (!m_bIsGraphConstructed) {
            init();
            construct_graph(edges, edge_count, has_reverse_cost, directed);
            m_bIsGraphConstructed = true;
        }
#endif
    pgassert(m_bIsGraphConstructed);
        auto start_edge_info =
         &m_vecEdgeVector[m_mapEdgeId2Index[start_edge_id]];
        edge_t start_edge;
        int64_t start_vertex, end_vertex;
        m_dStartpart = start_part;
        m_dEndPart = end_part;
        m_lStartEdgeId = start_edge_id;
        m_lEndEdgeId = end_edge_id;

        if (start_part == 0.0) {
            start_vertex = start_edge_info->m_lStartNode;
        } else if (start_part == 1.0) {
            start_vertex = start_edge_info->m_lEndNode;
        } else {
            isStartVirtual = true;
            m_lStartEdgeId = start_edge_id;
            start_vertex = max_node_id + 1;
            max_node_id++;
            start_edge.id = max_edge_id + 1;
            max_edge_id++;
            start_edge.source = start_vertex;
            start_edge.reverse_cost = -1.0;
            if (start_edge_info->m_dCost >= 0.0) {
                start_edge.target = start_edge_info->m_lEndNode;
                start_edge.cost = (1.0 - start_part) * start_edge_info->m_dCost;
                addEdge(start_edge);
                edge_count++;
            }
            if (start_edge_info->m_dReverseCost >= 0.0) {
                start_edge.id = max_edge_id + 1;
                max_edge_id++;
                start_edge.target = start_edge_info->m_lStartNode;
                start_edge.cost = start_part * start_edge_info->m_dReverseCost;
                addEdge(start_edge);
                edge_count++;
            }
        }

    auto end_edge_info =
    &m_vecEdgeVector[m_mapEdgeId2Index[end_edge_id]];
    edge_t end_edge;

    if (end_part == 0.0) {
        end_vertex = end_edge_info->m_lStartNode;
    } else if (end_part == 1.0) {
        end_vertex = end_edge_info->m_lEndNode;
    } else {
        isEndVirtual = true;
        m_lEndEdgeId = end_edge_id;
        end_vertex = max_node_id + 1;
        max_node_id++;
        end_edge.id = max_edge_id + 1;
        max_edge_id++;
        end_edge.target = end_vertex;
        end_edge.reverse_cost = -1.0;
        if (end_edge_info->m_dCost >= 0.0) {
            end_edge.source = end_edge_info->m_lStartNode;
            end_edge.cost = end_part * end_edge_info->m_dCost;
            addEdge(end_edge);
            edge_count++;
        }
        if (end_edge_info->m_dReverseCost >= 0.0) {
            end_edge.source = end_edge_info->m_lEndNode;
            end_edge.id = max_edge_id + 1;
            end_edge.cost = (1.0 - end_part) * end_edge_info->m_dReverseCost;
            addEdge(end_edge);
            edge_count++;
        }
    }

    return initializeAndProcess(
            edges, edge_count,
            ruleList,
            start_vertex, end_vertex,
            directed, has_reverse_cost,
            path, path_count, err_msg);
}


// -------------------------------------------------------------------------
int Pgr_trspHandler::initialize_restrictions(
        const std::vector<PDVI> &ruleList) {
    m_ruleTable.clear();
    LongVector vecsource;
    for (const auto &rule : ruleList) {
        size_t seq_cnt = rule.second.size();
        std::vector<int64_t> temp_precedencelist;
        temp_precedencelist.clear();
#if 0
        /*
         * TODO(vicky) test the range loop
         */
        for (const auto r : rule.second) {
            if (r ==  rule.second.front()) continue;
            temp_precedencelist.push_back(r);
        }
#else
        for (size_t j = 1; j < seq_cnt; j++) {
            temp_precedencelist.push_back(rule.second[j]);
        }
#endif
        auto dest_edge_id = rule.second[0];
        if (m_ruleTable.find(dest_edge_id) != m_ruleTable.end()) {
            m_ruleTable[dest_edge_id].push_back(
                    Rule(rule.first, temp_precedencelist));
        } else {
            std::vector<Rule> temprules;
            temprules.push_back(Rule(rule.first, temp_precedencelist));
            m_ruleTable.insert(std::make_pair(dest_edge_id, temprules));
        }

        if (isStartVirtual) {
            if (seq_cnt == 2 && rule.second[1] == m_lStartEdgeId) {
                vecsource = m_mapNodeId2Edge[m_start_vertex];
                for (const auto &source : vecsource) {
                    temp_precedencelist.clear();
                    temp_precedencelist.push_back(
                            m_vecEdgeVector[source].m_lEdgeID);
                    m_ruleTable[dest_edge_id].push_back(Rule(rule.first,
                                temp_precedencelist));
                }
            }
        }
    }
    if (isEndVirtual) {
        if (m_ruleTable.find(m_lEndEdgeId) != m_ruleTable.end()) {
            std::vector<Rule> tmpRules = m_ruleTable[m_lEndEdgeId];
            vecsource = m_mapNodeId2Edge[m_end_vertex];
            for (const auto &source : vecsource) {
                m_ruleTable.insert(std::make_pair(
                            m_vecEdgeVector[source].m_lEdgeID, tmpRules));
            }
        }
    }
    m_bIsturnRestrictOn = true;
    return true;
}

/** initializeAndProcess
 *
 * Acts as initializer and processing
 *
 */
int Pgr_trspHandler::initializeAndProcess(
        edge_t *edges,
        size_t edge_count,

        const std::vector<PDVI> &ruleList,

        const int64_t start_vertex,
        const int64_t end_vertex,

        bool directed,
        bool has_reverse_cost,
        path_element_tt **path,
        size_t *path_count,
        char **err_msg) {
    /*
     * Preconditions
     */
    pgassert(!m_bIsturnRestrictOn);
    pgassert(*path == NULL);
    pgassert(*path_count == 0);

    m_start_vertex = start_vertex;
    m_end_vertex = end_vertex;

    initialize_restrictions(ruleList);

    construct_graph(edges, edge_count, has_reverse_cost, directed);

    if (m_mapNodeId2Edge.find(m_start_vertex) == m_mapNodeId2Edge.end()) {
        *err_msg = (char *)"Source Not Found";
        clear();
        return -1;
    }

    if (m_mapNodeId2Edge.find(m_end_vertex) == m_mapNodeId2Edge.end()) {
        *err_msg = (char *)"Destination Not Found";
        clear();
        return -1;
    }

    /*
     * Post-conditions
     */
    pgassert(m_bIsGraphConstructed);
    pgassert(m_bIsturnRestrictOn);

    return process_trsp(
            edge_count,
            path,
            path_count,
            err_msg);
}







// -------------------------------------------------------------------------

void  Pgr_trspHandler::initialize_que() {

    /*
     * For each adjacent edge to the start_vertex
     */
    for (const auto &source : m_mapNodeId2Edge[m_start_vertex]) {
        auto cur_edge = &m_vecEdgeVector[source];
        if (cur_edge->m_lStartNode == m_start_vertex) {
            if (cur_edge->m_dCost >= 0.0) {
                m_dCost[cur_edge->m_lEdgeIndex].endCost = cur_edge->m_dCost;
                parent[cur_edge->m_lEdgeIndex].v_pos[0] = -1;
                parent[cur_edge->m_lEdgeIndex].ed_ind[0] = -1;
                que.push(std::make_pair(cur_edge->m_dCost,
                            std::make_pair(cur_edge->m_lEdgeIndex, true)));
            }
        } else {
            if (cur_edge->m_dReverseCost >= 0.0) {
                m_dCost[cur_edge->m_lEdgeIndex].startCost =
                    cur_edge->m_dReverseCost;
                parent[cur_edge->m_lEdgeIndex].v_pos[1] = -1;
                parent[cur_edge->m_lEdgeIndex].ed_ind[1] = -1;
                que.push(std::make_pair(cur_edge->m_dReverseCost,
                            std::make_pair(cur_edge->m_lEdgeIndex, false)));
            }
        }
    }
}

GraphEdgeInfo* Pgr_trspHandler::dijkstra_exploration(
        GraphEdgeInfo* cur_edge,
        int64_t  &cur_node) {
    while (!que.empty()) {
        auto cur_pos = que.top();
        que.pop();

        auto cured_index = cur_pos.second.first;
        cur_edge = &m_vecEdgeVector[cured_index];

        if (cur_pos.second.second) {
            /*
             * explore edges connected to end node
             */
            cur_node = cur_edge->m_lEndNode;
            if (cur_edge->m_dCost < 0.0) continue;
            if (cur_node == m_end_vertex) break;
            explore(cur_node, *cur_edge, true, cur_edge->m_vecEndConnedtedEdge);
        } else {
            /*
             *  explore edges connected to start node
             */
            cur_node = cur_edge->m_lStartNode;
            if (cur_edge->m_dReverseCost < 0.0) continue;
            if (cur_node == m_end_vertex) break;
            explore(cur_node, *cur_edge, false, cur_edge->m_vecStartConnectedEdge);
        }
    }
    return cur_edge;
}




int Pgr_trspHandler::process_trsp(
        size_t edge_count,
        path_element_tt **path,
        size_t *path_count,
        char **err_msg) {
    pgassert(m_bIsturnRestrictOn);
    pgassert(*path == NULL);
    pgassert(*path_count == 0);
    pgassert(m_bIsGraphConstructed);

    parent.resize(edge_count +1);
    m_dCost.resize(edge_count + 1);
    m_vecPath.clear();

    initialize_que();

    GraphEdgeInfo* cur_edge = NULL;
    int64_t cur_node = -1;

    cur_edge = dijkstra_exploration(cur_edge, cur_node);

    if (cur_node != m_end_vertex) {
        if (m_lStartEdgeId == m_lEndEdgeId) {
            if (get_single_cost(1000.0, path, path_count)) {
                return 0;
            }
        }
        *err_msg = (char *)"Path Not Found";
        clear();
        return -1;
    } 
    
    double total_cost;
    if (cur_node == cur_edge->m_lStartNode) {
        total_cost = m_dCost[cur_edge->m_lEdgeIndex].startCost;
        construct_path(cur_edge->m_lEdgeIndex, 1);
    } else {
        total_cost = m_dCost[cur_edge->m_lEdgeIndex].endCost;
        construct_path(cur_edge->m_lEdgeIndex, 0);
    }

    path_element_tt pelement;
    pelement.vertex_id = m_end_vertex;
    pelement.edge_id = -1;
    pelement.cost = 0.0;
    m_vecPath.push_back(pelement);

    if (m_lStartEdgeId == m_lEndEdgeId) {
        if (get_single_cost(total_cost, path, path_count)) {
            return 0;
        }
    }

    *path = (path_element_tt *) malloc(sizeof(path_element_tt) *
            (m_vecPath.size() + 1));
    *path_count = m_vecPath.size();

    for (size_t i = 0; i < *path_count; i++) {
        (*path)[i].vertex_id = m_vecPath[i].vertex_id;
        (*path)[i].edge_id = m_vecPath[i].edge_id;
        (*path)[i].cost = m_vecPath[i].cost;
    }
    if (isStartVirtual) {
        (*path)[0].vertex_id = -1;
        (*path)[0].edge_id = m_lStartEdgeId;
    }
    if (isEndVirtual) {
        *path_count = *path_count - 1;
        (*path)[*path_count - 1].edge_id = m_lEndEdgeId;
    }

    clear();
    return 0;
}













// -------------------------------------------------------------------------
bool Pgr_trspHandler::get_single_cost(double total_cost, path_element_tt **path,
        size_t *path_count) {
    auto start_edge_info =
        &m_vecEdgeVector[m_mapEdgeId2Index[m_lStartEdgeId]];
    if (m_dEndPart >= m_dStartpart) {
        if (start_edge_info->m_dCost >= 0.0 && start_edge_info->m_dCost *
                (m_dEndPart - m_dStartpart) <= total_cost) {
            *path = (path_element_tt *) malloc(sizeof(path_element_tt) * (1));
            *path_count = 1;
            (*path)[0].vertex_id = -1;
            (*path)[0].edge_id = m_lStartEdgeId;
            (*path)[0].cost = start_edge_info->m_dCost *
                (m_dEndPart - m_dStartpart);

            return true;
        }
    } else {
        if (start_edge_info->m_dReverseCost >= 0.0 &&
                start_edge_info->m_dReverseCost * (m_dStartpart - m_dEndPart) <=
                total_cost) {
            *path = (path_element_tt *) malloc(sizeof(path_element_tt) * (1));
            *path_count = 1;
            (*path)[0].vertex_id = -1;
            (*path)[0].edge_id = m_lStartEdgeId;
            (*path)[0].cost = start_edge_info->m_dReverseCost *
                (m_dStartpart - m_dEndPart);

            return true;
        }
    }
    return false;
}


// -------------------------------------------------------------------------
bool Pgr_trspHandler::construct_graph(
        edge_t* edges,
        const size_t edge_count,
        const bool has_reverse_cost,
        const bool directed) {
    if (m_bIsGraphConstructed) return true;

    init();
    for (size_t i = 0; i < edge_count; i++) {
        auto current_edge = &edges[i];
#if 0
        /*
         * TODO(vicky)
         * test this code to fix how the graph is build
         */
        if (!has_reverse_cost) {
            current_edge->reverse_cost = -1.0;
        }
        if (!directed) {
            if (has_reverse_cost) {
                /*
                 * is undirected and has reverse_cost
                 */
                if (current_edge->cost < 0) {
                    current_edge->cost = current_edge.reverse_cost;
                }
            } else {
                /*
                 * is undirected and does not have reverse_cost
                 */
                current_edge->reverse_cost = current_edge.cost;
            }
        }
#else
        if (!has_reverse_cost) {
            if (directed) {
                current_edge->reverse_cost = -1.0;
            } else {
                current_edge->reverse_cost = current_edge->cost;
            }
        }
#endif
        addEdge(*current_edge);
    }
    m_bIsGraphConstructed = true;
    return true;
}


// -------------------------------------------------------------------------
bool Pgr_trspHandler::connectEdge(GraphEdgeInfo& firstEdge,
        GraphEdgeInfo& secondEdge, bool bIsStartNodeSame) {
    if (bIsStartNodeSame) {
        if (firstEdge.m_dReverseCost >= 0.0)
            firstEdge.m_vecStartConnectedEdge.push_back(
                    secondEdge.m_lEdgeIndex);
        if (firstEdge.m_lStartNode == secondEdge.m_lStartNode) {
            if (secondEdge.m_dReverseCost >= 0.0)
                secondEdge.m_vecStartConnectedEdge.push_back(
                        firstEdge.m_lEdgeIndex);
        } else {
            if (secondEdge.m_dCost >= 0.0)
                secondEdge.m_vecEndConnedtedEdge.push_back(
                        firstEdge.m_lEdgeIndex);
        }
    } else {
        if (firstEdge.m_dCost >= 0.0)
            firstEdge.m_vecEndConnedtedEdge.push_back(secondEdge.m_lEdgeIndex);
        if (firstEdge.m_lEndNode == secondEdge.m_lStartNode) {
            if (secondEdge.m_dReverseCost >= 0.0)
                secondEdge.m_vecStartConnectedEdge.push_back(
                        firstEdge.m_lEdgeIndex);
        } else {
            if (secondEdge.m_dCost >= 0.0)
                secondEdge.m_vecEndConnedtedEdge.push_back(
                        firstEdge.m_lEdgeIndex);
        }
    }
    return true;
}


// -------------------------------------------------------------------------
bool Pgr_trspHandler::addEdge(const edge_t edgeIn) {
    // int64_t lTest;
    auto itMap = m_mapEdgeId2Index.find(edgeIn.id);
    if (itMap != m_mapEdgeId2Index.end())
        return false;


    GraphEdgeInfo newEdge(edgeIn, m_vecEdgeVector.size());

    if (edgeIn.id > max_edge_id) {
        max_edge_id = edgeIn.id;
    }

    if (newEdge.m_lStartNode > max_node_id) {
        max_node_id = newEdge.m_lStartNode;
    }
    if (newEdge.m_lEndNode > max_node_id) {
        max_node_id = newEdge.m_lEndNode;
    }

    // Searching the start node for connectivity
    Long2LongVectorMap::iterator itNodeMap = m_mapNodeId2Edge.find(
            edgeIn.source);
    if (itNodeMap != m_mapNodeId2Edge.end()) {
        // Connect current edge with existing edge with start node
        // connectEdge(
        int64_t lEdgeCount = itNodeMap->second.size();
        int64_t lEdgeIndex;
        for (lEdgeIndex = 0; lEdgeIndex < lEdgeCount; lEdgeIndex++) {
            int64_t lEdge = itNodeMap->second.at(lEdgeIndex);
            connectEdge(newEdge, m_vecEdgeVector[lEdge], true);
        }
    }


    // Searching the end node for connectivity
    itNodeMap = m_mapNodeId2Edge.find(edgeIn.target);
    if (itNodeMap != m_mapNodeId2Edge.end()) {
        // Connect current edge with existing edge with end node
        // connectEdge(
        int64_t lEdgeCount = itNodeMap->second.size();
        int64_t lEdgeIndex;
        for (lEdgeIndex = 0; lEdgeIndex < lEdgeCount; lEdgeIndex++) {
            int64_t lEdge = itNodeMap->second.at(lEdgeIndex);
            connectEdge(newEdge, m_vecEdgeVector[lEdge], false);
        }
    }



    // Add this node and edge into the data structure
    m_mapNodeId2Edge[edgeIn.source].push_back(newEdge.m_lEdgeIndex);
    m_mapNodeId2Edge[edgeIn.target].push_back(newEdge.m_lEdgeIndex);


    // Adding edge to the list
    m_mapEdgeId2Index.insert(std::make_pair(newEdge.m_lEdgeID,
                m_vecEdgeVector.size()));
    m_vecEdgeVector.push_back(newEdge);

    //


    return true;
}
