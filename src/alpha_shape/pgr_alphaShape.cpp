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

#include "alphaShape/pgr_alphaShape.hpp"
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/connected_components.hpp>
#include "mst/visitors.hpp"

namespace bg = boost::geometry;

namespace pgrouting {
namespace alphashape {

namespace {

    double
    det(double m00, double m01, double m10, double m11) {
        return m00 * m11 - m01 * m10;
    }

    Bpoint
    circumcenter(const Bpoint a, const Bpoint b, const Bpoint c) {
        double cx = c.x();
        double cy = c.y();
        double ax = a.x() - cx;
        double ay = a.y() - cy;
        double bx = b.x() - cx;
        double by = b.y() - cy;

        double denom = 2 * det(ax, ay, bx, by);
        double numx = det(ay, ax * ax + ay * ay, by, bx * bx + by * by);
        double numy = det(ax, ax * ax + ay * ay, bx, bx * bx + by * by);

        return Bpoint {cx - numx / denom, cy + numy / denom};
    }

}  // namespace

std::vector<Bpoint>
Pgr_delauny::possible_centers(const Bpoint p1, const Bpoint p2, const double alpha_radius) const {
    std::vector<Bpoint> centers;
    /*
     * p1 and p2 are the same point
     */
    pgassert(!bg::equals(p1, p2));
    if (bg::equals(p1, p2)) return centers;

    auto rombus_center = p2;
    bg::add_point(rombus_center, p1);
    bg::divide_value(rombus_center, 2);

    auto p_a = p2;
    bg::subtract_point(p_a, p1);
    bg::divide_value(p_a, 2);

    Bpoint origin(0, 0);

    auto a = bg::distance(p_a, origin);
    pgassert(!(a > bg::distance(p1, p2) / 2) && !(a < bg::distance(p_a, origin)));
    log << "distance to mid point" << a;
    log << "- bg distance to mid point" << bg::distance(p1, p2) / 2;
    log << "- alpha" << alpha_radius;

    /*
     * The segment is not alpha
     */
    if (!(alpha_radius > a)) return centers;

    auto m = - std::sqrt((alpha_radius + a) * (alpha_radius - a)) / a;

    centers.push_back({rombus_center.x() + m * p_a.y(), rombus_center.y() - m * p_a.x()});
    centers.push_back({rombus_center.x() - m * p_a.y(), rombus_center.y() + m * p_a.x()});

    return centers;
}


/*
 * Constructor
 */
Pgr_delauny::Pgr_delauny(const std::vector<Pgr_edge_xy_t> &edges) :
graph(UNDIRECTED) {
    graph.insert_edges(edges);
}


std::vector<Bpoly>
Pgr_delauny::operator()(double alpha) const {
    log << "starting calculation\n";
    std::vector<Bpoly> border;
    std::vector<E> hull;

    if (alpha <= 0) return border;

    std::vector<Bline> not_inalpha;
    std::vector<Bline> inalpha;

    /*
     * Thus we inspect every triangle ∆T∈DT(S)
     *  (or every line segment in the case d= 2 respectively)
     *  and checkout whether one of its circumspheres with radius α (thereare two of them) is empty.
     *   If so, we accept such a (d−1)-simplex.
     */
    BGL_FORALL_EDGES(edge, graph.graph, BG) {
        Bpoint source {graph[graph.source(edge)].point};
        Bpoint target {graph[graph.target(edge)].point};
        auto v_source = graph.source(edge);
        auto v_target = graph.target(edge);

#if 1
        log << "\n****** working with" << bg::wkt(Bline{{source, target}});
#endif
        auto centers = possible_centers(source, target, alpha);
#if 1
        for (const auto c : centers) {
            log << "\ncenter:" << bg::wkt(c);
        }
#endif

        if (centers.empty()) {
            log << "\n alpha < d(midpoint)" << alpha << " < " << bg::distance(source, target) / 2;
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

            std::set<V> adjacent1, adjacent2, v_intersection, v_union;
            log << "\nadjacent vertices to target" << graph[v_target] << "\n";
            BGL_FORALL_ADJ(graph.target(edge), v, graph.graph, BG) {
#if 1
                log << graph[v] << ",";
#endif
                adjacent1.insert(v);
            }
            log << "\nadjacent vertices to source" << graph[v_source] << "\n";
            BGL_FORALL_ADJ(graph.source(edge), v, graph.graph, BG) {
                adjacent2.insert(v);
#if 1
                log << graph[v] << ",";
#endif
            }
            std::set_intersection(adjacent1.begin(), adjacent1.end(),
                          adjacent2.begin(), adjacent2.end(),
                          std::inserter(v_intersection, v_intersection.end()));
            std::set_union(adjacent1.begin(), adjacent1.end(),
                          adjacent2.begin(), adjacent2.end(),
                          std::inserter(v_union, v_union.end()));

            bool found0(false);
            bool found1(false);
            if (v_intersection.size() == 1) {
                hull.push_back(edge);
                log << "edge in hull";
            }

            /* Expensive assertion */
            for (const auto v : v_intersection) {
                log << "\ntriangle" << bg::wkt(Bpoly{{source, target, graph[v].point}});
                auto center = circumcenter(source, target, graph[v].point);
                log << " circumcenter" << bg::wkt(center);
                log << " distances" << bg::distance(center, source) << "," << bg::distance(center, target) << bg::distance(center, graph[v].point);
                auto radius = bg::distance(center, source);
                for (const auto u : v_union) {
                    if (u == v_source || u == v_target || u == v) continue;
                    pgassert (bg::distance(center, graph[u].point) < radius)
                }
            }


            for (const auto v : v_intersection) {
                auto p(graph[v].point);
#if 1
                log << "\n-" << graph.graph[v];
                if (!(bg::distance(p, centers[0]) < alpha) && (bg::distance(p, centers[1]) < alpha)) {
                    log << "- Point distance to centers 1 is less than alpha radius";
                }
                if ((bg::distance(p, centers[0]) < alpha) && !(bg::distance(p, centers[1]) < alpha)) {
                    log << "- Point distance to centers 0 is less than alpha radius";
                }

                if ((bg::distance(p, centers[0]) < alpha) && (bg::distance(p, centers[1]) < alpha)) {
                    log << "- Point distance to both centers is less than alpha radius";
                }

                if (v == v_source) log << "is v source";
                if (v == v_target) log << "is v target";
#endif

                if (v == v_source || v == v_target) continue;
                found0 = found0 || (bg::distance(p, centers[0]) < alpha);
                found1 = found1 || (bg::distance(p, centers[1]) < alpha);

                count0 += (bg::distance(p, centers[0]) < alpha)? 1 : 0;
                count1 += (bg::distance(p, centers[1]) < alpha)? 1 : 0;
#if 1
                log << "\ncounts" << count0 << "-" << count1;
                log << "\nfounds" << found0 << "-" << found1;
#endif
            }


            if (!(found0 && found1)) {
                log << "\ninserting " << bg::wkt(Bline{{source, target}});
                E e = edge;
                m_in_border.edges.insert(e);
            }
        }
    }

    using Subgraph = boost::filtered_graph<BG, InBorder, boost::keep_all>;
    Subgraph subg (graph.graph, m_in_border, {});

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
        log << "\ncomponent size: " << filter.vertices.size();
        if (filter.vertices.size() < 2) continue;
        using BSUB = boost::filtered_graph<BG, InBorder, InComponent>;
        BSUB subSubG(graph.graph, m_in_border, filter);

        log << "\nvertices in component\n";
        BGL_FORALL_VERTICES(v, subSubG, BSUB) {
            log << "- " << subSubG[v];
        }

        log << "\nedges in component\n";
        log << "\nCREATE table tbl_1 (gid SERIAL, geom geometry);";
        BGL_FORALL_EDGES(e, subSubG, BSUB) {
            auto source = subSubG[boost::source(e, subSubG)].point;
            auto target = subSubG[boost::target(e, subSubG)].point;
            log << "\nINSERT INTO TABLE tbl_1 (geom) VALUES (ST_geomFromText('"
                << bg::wkt(Bline{{source, target}}) << "'));";
        }

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
        for (const auto edge : visited_order) {
            Bpoint source {graph[graph.source(edge)].point};
            Bpoint target {graph[graph.target(edge)].point};

#if 0
            log << bg::wkt(Bline{{graph[graph.source(edge)].point, Bline{{graph[graph.source(edge)].point}};

            log << boost::source(edge, subSubG);
#endif
        }

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

    for (const auto line : border) {
        log << "\n" << boost::geometry::wkt(line);
    }

    log << border.size() << "\n****";
    return border;
}



std::ostream&
operator<<(std::ostream& os, const Pgr_delauny &d) {
    os << d.graph;

    return os;
}

}  // namespace alphashape
}  // namespace pgrouting
