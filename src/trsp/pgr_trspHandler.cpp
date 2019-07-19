/*PGR-GNU*****************************************************************

File: pgr_trspHandler.cpp

Copyright (c) 2011 pgRouting developers
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

#include "trsp/pgr_trspHandler.h"

#include <functional>
#include <utility>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>
#include <deque>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_assert.h"

namespace pgrouting {
namespace trsp {

// -------------------------------------------------------------------------
Pgr_trspHandler::Pgr_trspHandler(
        pgr_edge_t *edges,
        const size_t edge_count,
        const bool directed,
        const std::vector<Rule> &ruleList) :
    m_ruleTable() {
    initialize_restrictions(ruleList);

    m_min_id = renumber_edges(edges, edge_count);

    construct_graph(
            edges,
            edge_count,
            directed);
}



// -------------------------------------------------------------------------
int64_t Pgr_trspHandler::renumber_edges(
        pgr_edge_t *edges,
        size_t total_edges) const {
        int64_t v_min_id = UINT64_MAX;
        size_t z;
        for (z = 0; z < total_edges; z++) {
            if (edges[z].source < v_min_id)
                v_min_id = edges[z].source;

            if (edges[z].target < v_min_id)
                v_min_id = edges[z].target;
        }

        for (z = 0; z < total_edges; z++) {
            edges[z].source -= v_min_id;
            edges[z].target -= v_min_id;
        }

        return v_min_id;
}




// -------------------------------------------------------------------------
void Pgr_trspHandler::clear() {
    m_parent.clear();
    m_dCost.clear();
    m_path.clear();
}


// -------------------------------------------------------------------------
double Pgr_trspHandler::construct_path(int64_t ed_id, Position pos) {
    pgassert(pos != ILLEGAL);

    if (m_parent[ed_id].isIllegal(pos)) {
        Path_t pelement;
        auto cur_edge = &m_edges[ed_id];
        if (pos == RC_EDGE) {
            pelement.node = cur_edge->startNode();
            pelement.cost = cur_edge->cost();
        } else {
            pelement.node = cur_edge->endNode();
            pelement.cost = cur_edge->r_cost();
        }
        pelement.edge = cur_edge->edgeID();

        m_path.push_back(pelement);
        pgassert(m_path.start_id() == m_start_vertex);
        return pelement.cost;
    }

    double ret = construct_path(m_parent[ed_id].e_idx[pos],
        m_parent[ed_id].v_pos[pos]);
    Path_t pelement;
    auto cur_edge = &m_edges[ed_id];
    if (pos == RC_EDGE) {
        pelement.node = cur_edge->startNode();
        pelement.cost = m_dCost[ed_id].endCost - ret;
        ret = m_dCost[ed_id].endCost;
    } else {
        pelement.node = cur_edge->endNode();
        pelement.cost = m_dCost[ed_id].startCost - ret;
        ret = m_dCost[ed_id].startCost;
    }
    pelement.edge = cur_edge->edgeID();

    m_path.push_back(pelement);

    return ret;
}


// -------------------------------------------------------------------------
double Pgr_trspHandler::getRestrictionCost(
        int64_t edge_ind,
        const EdgeInfo &edge,
        bool isStart) {
    double cost = 0.0;
    int64_t edge_id = edge.edgeID();
    if (m_ruleTable.find(edge_id) == m_ruleTable.end()) {
        return(0.0);
    }
    auto vecRules = m_ruleTable[edge_id];
    int64_t st_edge_ind = edge_ind;
    for (const auto &rule : vecRules) {
        bool flag = true;
        int64_t v_pos = (isStart? C_EDGE : RC_EDGE);
        edge_ind = st_edge_ind;

        pgassert(!(edge_ind == -1));
        for (auto const &precedence : rule.precedencelist()) {
            if (precedence != m_edges[edge_ind].edgeID()) {
                flag = false;
                break;
            }
            auto m_parent_ind = m_parent[edge_ind].e_idx[v_pos];
            v_pos = m_parent[edge_ind].v_pos[v_pos];
            edge_ind = m_parent_ind;
        }
        if (flag)
            cost += rule.cost();
    }
    return cost;
}


double Pgr_trspHandler::get_tot_cost(
        double cost,
        size_t edge_idx,
        bool isStart) {
    if (isStart) {
        return m_dCost[edge_idx].startCost +
            cost;
    }
    return m_dCost[edge_idx].endCost +
        cost;
}


// -------------------------------------------------------------------------
void Pgr_trspHandler::explore(
        int64_t cur_node,
        const EdgeInfo cur_edge,
        bool isStart) {
    double totalCost;

    auto vecIndex = cur_edge.get_idx(isStart);

    for (const auto &index : vecIndex) {
        auto edge = m_edges[index];

        auto extra_cost = getRestrictionCost(
                cur_edge.idx(),
                edge, isStart);

        if ((edge.startNode() == cur_node) && (edge.cost() >= 0.0)) {
            totalCost = get_tot_cost(
                    edge.cost() + extra_cost,
                    cur_edge.idx(),
                    isStart);

            if (totalCost < m_dCost[index].endCost) {
                m_dCost[index].endCost = totalCost;
                m_parent[edge.idx()].v_pos[RC_EDGE] = isStart? C_EDGE : RC_EDGE;
                m_parent[edge.idx()].e_idx[RC_EDGE] =
                    cur_edge.idx();

                add_to_que(totalCost, edge.idx(), true);
            }
        }

       if ((edge.endNode() == cur_node) && (edge.r_cost() >= 0.0)) {
            totalCost = get_tot_cost(
                    edge.r_cost() + extra_cost,
                    cur_edge.idx(),
                    isStart);

            if (totalCost < m_dCost[index].startCost) {
                m_dCost[index].startCost = totalCost;
                m_parent[edge.idx()].v_pos[C_EDGE] = isStart? C_EDGE : RC_EDGE;
                m_parent[edge.idx()].e_idx[C_EDGE] = cur_edge.idx();

                add_to_que(totalCost, edge.idx(), false);
            }
        }
    }  // for
}



// -------------------------------------------------------------------------
int Pgr_trspHandler::initialize_restrictions(
        const std::vector<Rule> &ruleList) {
    for (const auto rule : ruleList) {
        auto dest_edge_id = rule.dest_id();
        if (m_ruleTable.find(dest_edge_id) != m_ruleTable.end()) {
            m_ruleTable[dest_edge_id].push_back(rule);
        } else {
            std::vector<Rule> r;
            r.push_back(rule);
            m_ruleTable.insert(std::make_pair(dest_edge_id, r));
        }
    }

    return true;
}

/** process
 *
 * does the processisng
 *
 */
Path
Pgr_trspHandler::process(
        const int64_t start_vertex,
        const int64_t end_vertex) {
    clear();

    m_start_vertex = start_vertex - m_min_id;
    m_end_vertex = end_vertex - m_min_id;

    Path tmp(m_start_vertex, m_end_vertex);
    m_path = tmp;

    if (m_adjacency.find(m_start_vertex) == m_adjacency.end()) {
        return Path();
    }

    if (m_adjacency.find(m_end_vertex) == m_adjacency.end()) {
        return Path();
    }

    return process_trsp(m_edges.size());
}






/** process
 *
 * does many to many processisng
 *
 */
std::deque<Path>
Pgr_trspHandler::process(
        const std::vector<int64_t> sources,
        const std::vector<int64_t> targets) {
    std::deque<Path> paths;
    for (const auto &s : sources) {
        for (const auto &t : targets) {
            paths.push_back(process(s, t));
        }
    }

    std::sort(paths.begin(), paths.end(),
            [](const Path &e1, const Path &e2)->bool {
            return e1.end_id() < e2.end_id();
            });
    std::stable_sort(paths.begin(), paths.end(),
            [](const Path &e1, const Path &e2)->bool {
            return e1.start_id() < e2.start_id();
            });
    return paths;
}





void  Pgr_trspHandler::add_to_que(
        double cost,
        size_t e_idx,
        bool isStart) {
    que.push(std::make_pair(cost,
                std::make_pair(e_idx, isStart)));
}



// -------------------------------------------------------------------------

void  Pgr_trspHandler::initialize_que() {
    /*
     * For each adjacent edge to the start_vertex
     */
    for (const auto source : m_adjacency[m_start_vertex]) {
        EdgeInfo &cur_edge = m_edges[source];

        if (cur_edge.startNode() == m_start_vertex
                && cur_edge.cost() >= 0.0) {
            m_dCost[cur_edge.idx()].endCost = cur_edge.cost();
            m_parent[cur_edge.idx()].v_pos[0] = ILLEGAL;
            add_to_que(cur_edge.cost(), cur_edge.idx(), true);
        }

        if (cur_edge.endNode() == m_start_vertex
                && cur_edge.r_cost() >= 0.0) {
            m_dCost[cur_edge.idx()].startCost =
                cur_edge.r_cost();
            m_parent[cur_edge.idx()].v_pos[1] = ILLEGAL;
            add_to_que(cur_edge.r_cost(), cur_edge.idx(), false);
        }
    }
}

EdgeInfo Pgr_trspHandler::dijkstra_exploration() {
    EdgeInfo cur_edge;
    pgassert(current_node == m_start_vertex);

    while (!que.empty()) {
        auto cur_pos = que.top();
        que.pop();

        auto cure_idxex = cur_pos.second.first;
        cur_edge = m_edges[cure_idxex];

        if (cur_pos.second.second) {
            /*
             * explore edges connected to end node
             */
            current_node = cur_edge.endNode();
            if (cur_edge.cost() < 0.0) continue;
            if (current_node == m_end_vertex) break;
            explore(current_node, cur_edge, false);
        } else {
            /*
             *  explore edges connected to start node
             */
            current_node = cur_edge.startNode();
            if (cur_edge.r_cost() < 0.0) continue;
            if (current_node == m_end_vertex) break;
            explore(current_node, cur_edge, true);
        }
    }
    return cur_edge;
}




Path
Pgr_trspHandler::process_trsp(
        size_t edge_count) {
    pgassert(m_path.start_id() == m_start_vertex);
    pgassert(m_path.end_id() == m_end_vertex);
    pgassert(m_parent.empty());

    m_parent.resize(edge_count + 1);
    m_dCost.resize(edge_count + 1);

    initialize_que();

    current_node = m_start_vertex;

    pgassert(m_path.start_id() == m_start_vertex);

    auto cur_edge = dijkstra_exploration();

    pgassert(m_path.start_id() == m_start_vertex);
    if (current_node != m_end_vertex) {
        Path result(m_start_vertex, m_end_vertex);
        return result.renumber_vertices(m_min_id);;
    }

    pgassert(m_path.start_id() == m_start_vertex);

    if (current_node == cur_edge.startNode()) {
        construct_path(cur_edge.idx(), C_EDGE);
    } else {
        construct_path(cur_edge.idx(), RC_EDGE);
    }

    Path_t pelement;
    pelement.node = m_end_vertex;
    pelement.edge = -1;
    pelement.cost = 0.0;
    m_path.push_back(pelement);

    m_path.Path::recalculate_agg_cost();
    return m_path.renumber_vertices(m_min_id);
}





// -------------------------------------------------------------------------
void Pgr_trspHandler::construct_graph(
        pgr_edge_t* edges,
        const size_t edge_count,
        const bool directed) {
    for (size_t i = 0; i < edge_count; i++) {
        auto current_edge = &edges[i];

        /*
         * making all costs > 0
         */
        if (current_edge->cost < 0 && current_edge->reverse_cost > 0) {
            std::swap(current_edge->cost, current_edge->reverse_cost);
            std::swap(current_edge->source, current_edge->target);
        }

        if (!directed) {
            if (current_edge->reverse_cost < 0) {
                current_edge->reverse_cost = current_edge->cost;
            }
        }
        addEdge(*current_edge);
    }
    m_mapEdgeId2Index.clear();
}


// -------------------------------------------------------------------------
void Pgr_trspHandler::connectStartEdge(
        size_t firstEdge_idx,
        size_t secondEdge_idx) {
    EdgeInfo &firstEdge = m_edges[firstEdge_idx];
    EdgeInfo &secondEdge = m_edges[secondEdge_idx];

    if (firstEdge.r_cost() >= 0.0) {
        firstEdge.connect_startEdge(secondEdge_idx);
    }

    if (firstEdge.startNode() == secondEdge.startNode()
            && secondEdge.r_cost() >= 0.0) {
            secondEdge.connect_startEdge(firstEdge_idx);
    }

    if (firstEdge.startNode() == secondEdge.endNode()
            &&secondEdge.cost() >= 0.0) {
        secondEdge.connect_endEdge(firstEdge_idx);
    }
}


// -------------------------------------------------------------------------
void Pgr_trspHandler::connectEndEdge(
        size_t firstEdge_idx,
        size_t secondEdge_idx) {
    EdgeInfo &firstEdge = m_edges[firstEdge_idx];
    EdgeInfo &secondEdge = m_edges[secondEdge_idx];

    if (firstEdge.cost() >= 0.0) {
        firstEdge.connect_endEdge(secondEdge_idx);
    }

    if (firstEdge.endNode() == secondEdge.startNode()
            && secondEdge.r_cost() >= 0.0) {
        secondEdge.connect_startEdge(firstEdge_idx);
    }

    if (firstEdge.endNode() == secondEdge.endNode()
            && secondEdge.cost() >= 0.0) {
        secondEdge.connect_endEdge(firstEdge_idx);
    }
}


// -------------------------------------------------------------------------
bool Pgr_trspHandler::addEdge(const pgr_edge_t edgeIn) {
    /*
     * The edge was already inserted
     *
     * If the user has rows with repeated edge id, the subsequent edges will be ignored
     *
     * When changing to boost graph, the additional edges are to be added
     */
    if (m_mapEdgeId2Index.find(edgeIn.id) != m_mapEdgeId2Index.end()) {
        return false;
    }


    /*
     * the index of this new edge in the edges container is
     *  m_edges.size()
     */
    EdgeInfo edge(edgeIn, m_edges.size());

    // Adding edge to the list
    m_mapEdgeId2Index.insert(
            std::make_pair(
                edge.edgeID(),
                m_edges.size()));

    m_edges.push_back(edge);

    EdgeInfo &newEdge = m_edges[m_edges.size()-1];



    /*
     *  Searching the start node for connectivity
     */
    auto itNodeMap = m_adjacency.find(edgeIn.source);

    if (itNodeMap != m_adjacency.end()) {
        for (const auto e_idx : itNodeMap->second) {
            connectStartEdge(edge.idx(), e_idx);
        }
    }


    /*
     *  Searching the end node for connectivity
     */
    itNodeMap = m_adjacency.find(edgeIn.target);
    if (itNodeMap != m_adjacency.end()) {
        for (const auto e_idx : itNodeMap->second) {
            connectEndEdge(edge.idx(), e_idx);
        }
    }


    /*
     * Add the edges to the adjacency list
     */
    m_adjacency[edgeIn.source].push_back(newEdge.idx());
    m_adjacency[edgeIn.target].push_back(newEdge.idx());



    return true;
}



}  // namespace trsp
}  // namespace pgrouting
