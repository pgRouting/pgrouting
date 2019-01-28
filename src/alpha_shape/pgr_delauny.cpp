/*PGR-GNU*****************************************************************
file: pgr_delauny.cpp

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#include "alphaShape/pgr_delauny.hpp"
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/connected_components.hpp>
#include "mst/visitors.hpp"

namespace bg = boost::geometry;

namespace pgrouting {
namespace alphashape {

namespace {

std::vector<Bpoint>
possible_centers(const Bpoint p1, const Bpoint p2, const double r) {
    std::vector<Bpoint> centers;
    /*
     * p1 and p2 are the same point
     */
    if (bg::equals(p1, p2)) return centers;

    auto rombus_center = p2;
    bg::add_point(rombus_center, p1);
    bg::divide_value(rombus_center, 2);

    auto p_a = p2;
    bg::subtract_point(p_a, p1);
    bg::divide_value(p_a, 2);

    Bpoint origin(0, 0);

    auto a = bg::distance(p_a, origin);

    /*
     * The segment is not alpha
     */
    if (!(r > a)) return centers;

    auto m = - std::sqrt((r + a) * (r - a)) / a;

    centers.push_back({rombus_center.x() + m * p_a.y(), rombus_center.y() - m * p_a.x()});
    centers.push_back({rombus_center.x() - m * p_a.y(), rombus_center.y() + m * p_a.x()});

    return centers;
}
}  // namespace


Pgr_delauny::Pgr_delauny(const std::vector<Pgr_edge_xy_t> &edges) :
graph(UNDIRECTED) {
    log << "Working with Undirected Graph\n";
    graph.insert_edges(edges);

    alpha_edges(1);
}


std::vector<Bpoly>
Pgr_delauny::alpha_edges(double alpha) const {
    std::vector<Bpoly> border;

    if (alpha == 0) return border;

    auto radius = 1 / alpha;
    std::vector<Bline> not_inalpha;
    std::vector<Bline> inalpha;

    BGL_FORALL_EDGES(edge, graph.graph, BG) {
        Bpoint source {graph[graph.source(edge)].point};
        Bpoint target {graph[graph.target(edge)].point};

        auto centers = possible_centers(source, target, radius);

        if (centers.empty()) {
            // TODO could be the edge descriptor
            not_inalpha.push_back(Bline{{source, target}});
        } else {
            // TODO could be the edge descriptor
            inalpha.push_back(Bline{{source, target}});
            /*
             * Is the segment in a border?
             */
            size_t count0(0);
            size_t count1(0);
            BGL_FORALL_VERTICES(v, graph.graph, BG) {
                auto p(graph[v].point);
                if (bg::equals(p, source) || bg::equals(p, target)) continue;
                count0 += (bg::distance(p, centers[0]) < radius)? 1 : 0;
                count1 += (bg::distance(p, centers[1]) < radius)? 1 : 0;
            }

            if ((count0 && !count1) || (!count0 && count1)) {
                E e = edge;
                m_spanning_tree.edges.insert(e);
//                border.push_back(Bline{{source, target}});
            }
        }
    }

    using Subgraph = boost::filtered_graph<BG, InSpanning, boost::keep_all>;
    Subgraph subg (graph.graph, m_spanning_tree, {});

    std::vector<V> component(num_vertices(subg));

    auto num_comps = boost::connected_components(
            subg,
            &component[0]);

    struct InComponent {
         std::set<V> vertices;
         bool operator()(V v) const { return vertices.count(v); }
         void clear() { vertices.clear(); }
     };

    std::vector<InComponent> componentFilter(num_comps);
    for (const auto v : boost::make_iterator_range(vertices(subg))) {
        /*
         * Save each vertex in the corresponding component
         * v is in component[v]
         */
        componentFilter[component[v]].vertices.insert(v);
    }

    for (const auto filter : componentFilter) {
        if (filter.vertices.size() < 2) continue;
        log << "\ncomponent size: " << filter.vertices.size();
        boost::filtered_graph<BG, InSpanning, InComponent> subSubG(graph.graph, m_spanning_tree, filter);
        std::vector<E> visited_order;
        /*
         * making a dfs of the component
         */
        using dfs_visitor = visitors::Dfs_visitor<E>;
        try {
            boost::depth_first_search(subSubG, visitor(dfs_visitor(visited_order)));
        } catch (boost::exception const& ex) {
            (void)ex;
            throw;
        } catch (std::exception &e) {
            (void)e;
            throw;
        } catch (...) {
            throw;
        }
        log << "\nvisited order:";

        Bpoly line;
        for (const auto edge : visited_order) {
            log << edge << "->";
            if (edge == visited_order.front()) {
                bg::append(line, graph[graph.source(edge)].point);
            }
            bg::append(line, graph[graph.target(edge)].point);
        };
        border.push_back(line);
    }

    return border;
}



std::ostream&
operator<<(std::ostream& os, const Pgr_delauny &d) {
    os << d.graph;

    auto border = d.alpha_edges(1);
    os << "\nOn external ";
    for (const auto line : border) {
        os << "\n" << boost::geometry::wkt(line);
    }

    return os;
}

}  // namespace alphashape
}  // namespace pgrouting
