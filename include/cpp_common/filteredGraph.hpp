/*PGR-GNU*****************************************************************
File: filteredGraph.hpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky@erosion.dev

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

/** @file */

#ifndef INCLUDE_CPP_COMMON_FILTEREDGRAPH_HPP_
#define INCLUDE_CPP_COMMON_FILTEREDGRAPH_HPP_
#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>

namespace pgrouting {
namespace graph {

namespace detail {

/** @brief Hides vertices and edge ids for one search; clear() reveals them.
 *
 * Restoration is exact: nothing is mutated, so nothing can be lost.
 * Edges are matched by id, never by a (source, target) pair, so
 * parallel edges survive. State is shared: boost copies the predicate.
 *
 * @tparam B_G the underlying boost graph, not a Pgr_base_graph
 */
template <class B_G>
class RemovalFilter {
 public:
    using V = typename boost::graph_traits<B_G>::vertex_descriptor;
    using E = typename boost::graph_traits<B_G>::edge_descriptor;
    using id_type = int64_t;

    /** @brief Default constructor
     *
     * Kept so the predicate stays a regular type satisfying Boost's implicit requirements
     */
    RemovalFilter() = default;

    /** @brief Constructor
     *
     * @param [in] g        the graph whose edges will be filtered
     */
    RemovalFilter(const B_G &g, bool)
        : m_g(&g) { }

    RemovalFilter(const RemovalFilter &) = default;
    RemovalFilter &operator=(const RemovalFilter &) = default;
    ~RemovalFilter() = default;

    /** @brief Edge predicate
     *
     * @returns false when the edge touches a removed vertex, or its id was
     *          removed
     * @returns true otherwise
     *
     */
    bool operator()(E e) const {
        if (m_g == nullptr) return true;
        if (m_state->m_vertices.empty() && m_state->m_edge_ids.empty()) return true;

        if (!m_state->m_vertices.empty()) {
            auto s_id = (*m_g)[boost::source(e, *m_g)].id;
            auto t_id = (*m_g)[boost::target(e, *m_g)].id;

            if (m_state->m_vertices.count(s_id)) return false;
            if (m_state->m_vertices.count(t_id)) return false;
        }

        return !m_state->m_edge_ids.count((*m_g)[e].id);
    }

    /** @brief Vertex predicate
     *
     * Always true.
     * Kept because boost::filtered_graph requires a vertex predicate type.
     */
    bool vertex(V) const { return true; }

    /** @brief hides every edge incident to vid */
    void remove_vertex(id_type vid) { m_state->m_vertices.insert(vid); }

    /** @brief hides the single edge whose id is eid
     *
     * One edge, not every edge between its endpoints.
     * That distinction is what keeps parallel edges available to a later search.
     */
    void remove_edge_id(id_type eid) { m_state->m_edge_ids.insert(eid); }

    /** @brief reveals everything, equivalent to restoring a mutated graph */
    void clear() {
        m_state->m_vertices.clear();
        m_state->m_edge_ids.clear();
    }

    bool empty() const {
        return m_state->m_vertices.empty() && m_state->m_edge_ids.empty();
    }

    size_t vertex_count() const { return m_state->m_vertices.size(); }

    size_t edge_count() const { return m_state->m_edge_ids.size(); }

 private:
    /* Shared by every copy of this filter, because boost::filtered_graph keeps
     * its own by-value copy of the predicate. See the class documentation. */
    struct State {
        std::set<id_type> m_vertices;
        std::set<id_type> m_edge_ids;
    };

    const B_G *m_g{nullptr};
    std::shared_ptr<State> m_state{std::make_shared<State>()};
};

}  // namespace detail


/** @brief Presents a boost::filtered_graph through the Pgr_base_graph API,
 *
 * so dijkstra() and friends can traverse a filtered view unchanged.
 * Removal stays invisible to has_vertex(), which reports the
 * underlying graph; Path works because operator[] forwards to it.
 *
 * @tparam G the Pgr_base_graph being viewed
 */
template <class G>
class FilteredGraph {
 public:
    using raw_t = typename G::B_G;
    using G_T_E = typename G::G_T_E;
    using V = typename G::V;
    using E = typename G::E;

    using Filter = detail::RemovalFilter<raw_t>;
    using filtered_t = boost::filtered_graph<raw_t, Filter, boost::keep_all>;
    using degree_size_type = typename boost::graph_traits<raw_t>::degree_size_type;

    /** @brief The type dijkstra() instantiates its worker with, which has to
     *         be the type of the `graph` member. */
    using B_G = filtered_t;

 private:
    /* Declared before the public `graph` member on purpose. Members are
     * constructed in declaration order, and `graph` is built from m_filter, so
     * m_filter has to be fully constructed first. */
    G *m_wrapper{nullptr};
    Filter m_filter;

 public:
    /** @brief Constructor
     *
     * The filter and the view refer to the same underlying graph
     * The wrapper outlives this object, which callers create on the stack for the
     * duration of one algorithm run.
     *
     * @param [in] g the graph to view
     */
    explicit FilteredGraph(G &g)
        : m_wrapper(&g),
          m_filter(g.graph, g.is_directed()),
          graph(g.graph, m_filter, boost::keep_all()) {}

    FilteredGraph(const FilteredGraph &) = delete;
    FilteredGraph &operator=(const FilteredGraph &) = delete;
    FilteredGraph(FilteredGraph &&) = delete;
    FilteredGraph &operator=(FilteredGraph &&) = delete;
    ~FilteredGraph() = default;

    /** @brief The filtered view.
     *
     * Public, and deliberately named `graph`, because that is the member name
     * the dijkstra() and Pgrouting::Path code reaches through.
     */
    filtered_t graph;

    size_t num_vertices() const { return m_wrapper->num_vertices(); }

    bool has_vertex(int64_t vid) const { return m_wrapper->has_vertex(vid); }

    V get_V(int64_t vid) const { return m_wrapper->get_V(vid); }

    int64_t get_edge_id(V from, V to, double &weight) const {
        return m_wrapper->get_edge_id(from, to, weight);
    }

    bool is_directed() const { return m_wrapper->is_directed(); }

    /** @brief Out degree of a vertex
     *
     * Reports the degree on the underlying graph, not on the view, so it is the
     * count before anything was suppressed.
     *
     */
    degree_size_type out_degree(int64_t vertex_id) const {
        return m_wrapper->out_degree(vertex_id);
    }

    /** @name removal, replaces graph mutation */
    ///@{

    void remove_vertex(int64_t vid) { m_filter.remove_vertex(vid); }

    void remove_edge_id(int64_t eid) { m_filter.remove_edge_id(eid); }

    void clear_filter() { m_filter.clear(); }

    bool filter_empty() const { return m_filter.empty(); }

    ///@}

    void restore_graph() { m_filter.clear(); }
};

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_FILTEREDGRAPH_HPP_
