/*PGR-GNU*****************************************************************
file: pgr_alphaShape.cpp

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

#include "alphaShape/pgr_alphaShape.h"

#include <visitors/dijkstra_one_goal_visitor.hpp>
#if BOOST_VERSION_OK
#include <boost/graph/dijkstra_shortest_paths.hpp>
#else
#include "boost/dijkstra_shortest_paths.hpp"
#endif
#include <boost/graph/filtered_graph.hpp>

#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>


namespace bg = boost::geometry;

namespace pgrouting {
namespace alphashape {

namespace {


/*
 * Determinant of this matrix
 * r00, r01
 * r10, r11
 */
double
det(double r00, double r01, double r10, double r11) {
    return r00 * r11 - r01 * r10;
}

Bpoint
circumcenter(const Bpoint a, const Bpoint b, const Bpoint c) {
    auto cx = c.x();
    auto cy = c.y();
    auto ax = a.x() - cx;
    auto ay = a.y() - cy;
    auto bx = b.x() - cx;
    auto by = b.y() - cy;

    auto denom = 2 * det(ax, ay, bx, by);
    /*
     * If denom == 0 then points are colinear
     */
    pgassert(denom != 0);

    auto numx = det(ay, ax * ax + ay * ay, by, bx * bx + by * by);
    auto numy = det(ax, ax * ax + ay * ay, bx, bx * bx + by * by);

    return Bpoint {cx - numx / denom, cy + numy / denom};
}

template <typename B_G, typename V>
std::vector<V>
get_predecessors(V source, V target,  const B_G &subg) {
    pgassert(boost::num_vertices(subg));
    std::vector<V> predecessors(boost::num_vertices(subg));
    std::vector<double> distances(num_vertices(subg));
    pgassert(predecessors.size() == boost::num_vertices(subg));
    pgassert(distances.size() == boost::num_vertices(subg));

    try {
        boost::dijkstra_shortest_paths(subg, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&Basic_edge::cost, subg))
                .distance_map(&distances[0])
                .visitor(visitors::dijkstra_one_goal_visitor<V>(target)));
    } catch(found_goals &) {
    } catch (boost::exception const& ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
             throw;
         }
    return predecessors;
}


template <typename B_G, typename V>
Bpoly
get_polygon(V source, V target, const std::vector<V> & predecessors, const B_G &graph, std::set<E> &edges_used) {
        Bpoly polygon;
        /*
         * There is no path -> returning empty polygon
         */
        if (target == predecessors[target]) {
            pgassert(bg::num_points(polygon) == 0);
            return polygon;
        }

        /*
         * the last stop is the target
         */
        bg::append(polygon.outer(), graph[source].point);

        /*
         * get the path
         */
        while (target != source || target != predecessors[target]) {
            bg::append(polygon.outer(), graph[target].point);
            edges_used.insert(boost::edge(predecessors[target], target, graph).first);
            target = predecessors[target];
        }
        bg::correct(polygon);
        return polygon;
}

typedef std::pair<Triangle, double> MyPairType;
struct CompareRadius {
    bool operator()(const MyPairType& lhs, const MyPairType& rhs) const {
        return lhs.second > rhs.second;
    }
};

}  // namespace


/*
 * Constructor
 */
Pgr_alphaShape::Pgr_alphaShape(const std::vector<Pgr_edge_xy_t> &edges) :
graph(UNDIRECTED) {
    graph.insert_edges(edges);
    make_triangles();
}

void
Pgr_alphaShape::make_triangles() {
    /*
     * triangle sides:
     * a_r, b, c  edge descriptor
     */
    BGL_FORALL_EDGES(c, graph.graph, BG) {
        /*
         * triangle vertices:
         * u, v, w vertex descriptor
         */
        auto u = graph.source(c);
        auto v = graph.target(c);

        std::vector<Triangle> adjacent_to_side;

        size_t i = 0;
        BGL_FORALL_OUTEDGES(u, b, graph.graph, BG) {
            ++i;
            auto w = graph.adjacent(u, b);
            if (w == v) {
                pgassert(b == c);
                continue;
            }

            auto a_r = boost::edge(v, w, graph.graph);
            if (!a_r.second) continue;

            Triangle face{{a_r.first, b, c}};
            adjacent_to_side.push_back(face);
        }

        /*
         * All vertices must have at least 2 edges
         * So cycle above must have passed at least twice
         */
        pgassert(i > 1);

        if (adjacent_to_side.size() == 2) {
            m_adjacent_triangles[adjacent_to_side[0]].insert(adjacent_to_side[1]);
            m_adjacent_triangles[adjacent_to_side[1]].insert(adjacent_to_side[0]);
        } else {
            if (m_adjacent_triangles.find(adjacent_to_side[0]) == m_adjacent_triangles.end()) {
                m_adjacent_triangles[adjacent_to_side[0]].clear();
            }
        }
    }
}


/*
 * Radius of triangle's circumcenter
 */
double
Pgr_alphaShape::radius(const Triangle t) const {
        std::vector<E> edges(t.begin(), t.end());
        auto a = graph.source(edges[0]);
        auto b = graph.target(edges[0]);
        auto c = graph.source(edges[1]);
        c = (c == a || c == b)? graph.target(edges[1]) : c;
        pgassert(a != b && a != c && b!= c);

        auto center = circumcenter(graph[a].point, graph[b].point, graph[c].point);
        return bg::distance(center, graph[a].point);
}

/*
 * The whole traingle face belongs to the shape?
 */
bool
Pgr_alphaShape::faceBelongs(const Triangle t, double alpha) const {
        return radius(t) <= alpha;
}

std::vector<Bpoly>
Pgr_alphaShape::build_best_alpha() const {
    std::map<Triangle, double> border_triangles;
    std::map<Triangle, double> inner_triangles;

    size_t i(0);
    for (const auto t : m_adjacent_triangles) {
        if (t.second.size() == 2) {
            border_triangles[t.first] = radius(t.first);
        } else  {
            inner_triangles[t.first] = radius(t.first);
        }
        ++i;
    }
    pgassert(border_triangles.size() + inner_triangles.size() == i);

    auto max_border_triangle = *std::min_element(border_triangles.begin(), border_triangles.end(), CompareRadius());
    auto max_inner_triangle = *std::min_element(inner_triangles.begin(), inner_triangles.end(), CompareRadius());

    double max_border_radius = max_border_triangle.second;
    double max_inner_radius = max_inner_triangle.second;


    auto count = border_triangles.size() + inner_triangles.size();
    while (max_border_radius >= max_inner_radius) {
        auto max_border_triangle = *std::min_element(border_triangles.begin(), border_triangles.end(), CompareRadius());
        auto max_inner_triangle = *std::min_element(inner_triangles.begin(), inner_triangles.end(), CompareRadius());
        /*
         * Removing largest border triangle
         */
        border_triangles.erase(max_border_triangle.first);

        /*
         * Adjacent triangles of a border triangle
         *  - are no longer inner triangles
         *  - are now border triangles
         */
        for (const auto t : m_adjacent_triangles.at(max_border_triangle.first)) {
            if (inner_triangles.find(t) != inner_triangles.end()) {
                inner_triangles.erase(t);
                border_triangles[t] = radius(t);
            }
        }

        auto new_max_border_triangle = *std::min_element(
                border_triangles.begin(), border_triangles.end(), CompareRadius());
        auto new_max_inner_triangle = *std::min_element(
                inner_triangles.begin(), inner_triangles.end(), CompareRadius());

        if (new_max_border_triangle.second < new_max_inner_triangle.second) {
            /*
             * Roll back and exit loop
             */
             for (const auto t : m_adjacent_triangles.at(max_border_triangle.first)) {
                 border_triangles.erase(t);
                 inner_triangles[t] = radius(t);
             }
            border_triangles[max_border_triangle.first] = max_border_triangle.second;
            break;
        }
        max_border_radius = new_max_border_triangle.second;
        max_inner_radius = new_max_inner_triangle.second;
        pgassert(count > (border_triangles.size() + inner_triangles.size()));
        count = border_triangles.size() + inner_triangles.size();;
    }

    pgassert(max_border_radius > 0);
    log << "Using Alpha:" << max_border_radius * max_border_radius;
    return this->operator()(max_border_radius);
}

void
Pgr_alphaShape::recursive_build(
        const Triangle face,
        std::set<Triangle> &used,
        std::set<E> &border_edges,
        double alpha) const {
    /*
     * Do nothing when the face does not belong to the polygon of the alphashape
     */
    if (!faceBelongs(face, alpha)) return;

    /*
     * Do nothing when the face has being processed before
     */
    auto original = used.size();
    used.insert(face);
    if (original == used.size())  return;

    std::set<E> common_sides;
    for (const auto adj_t : m_adjacent_triangles.at(face)) {
        if (!faceBelongs(adj_t, alpha)) {
            /*
             * Adjacent face is not on shape
             * then side is on the border
             */
            std::set_intersection(
                    face.begin(), face.end(),
                    adj_t.begin(), adj_t.end(),
                    std::inserter(border_edges, border_edges.end()));
        }
        std::set_intersection(
                face.begin(), face.end(),
                adj_t.begin(), adj_t.end(),
                std::inserter(common_sides, common_sides.end()));
        recursive_build(adj_t, used, border_edges, alpha);
    }

    if (m_adjacent_triangles.at(face).size() == 2) {
        /*
         * Side without adjacent triangle (in convex hull)
         * is part of the border
         */
        std::set_difference(
                face.begin(), face.end(),
                common_sides.begin(), common_sides.end(),
                std::inserter(border_edges, border_edges.end()));
    }
}

std::vector<Bpoly>
Pgr_alphaShape::operator() (double alpha) const {
    std::vector<Bpoly> shape;

    if (alpha <= 0) return build_best_alpha();

    std::set<Triangle> used;
    using Subgraph = boost::filtered_graph<BG, EdgesFilter, boost::keep_all>;

    for (const auto t : m_adjacent_triangles) {
        EdgesFilter border_edges;
        /*
         * Recurse thru the triangles to get the border sides
         */
        recursive_build(t.first, used, border_edges.edges, alpha);
        /*
         * triangle was already processed
         * or is not part of the shape
         */
        if (border_edges.edges.empty()) continue;

        std::vector<Bpoly> polys;
        Bpoly polygon;
        double area = 0;
        while (!border_edges.edges.empty()) {
            auto first_edge = *border_edges.edges.begin();
            border_edges.edges.erase(first_edge);

            Subgraph subg(graph.graph, border_edges, {});

            auto source = boost::source(first_edge, subg);
            auto target = boost::target(first_edge, subg);


            auto predecessors = get_predecessors(source, target, subg);
            std::set<E> edges_used;
            auto poly = get_polygon(source, target, predecessors, subg, edges_used);

            for (const auto &e : edges_used) {
                border_edges.edges.erase(e);
            }

            pgassert(bg::num_points(poly) >= 3 || bg::num_points(poly) == 0);

            if (bg::num_points(poly) == 0) continue;

            /*
             * A polygon must have at least 3 vertices
             */
            if (bg::num_points(poly) >= 3) {
                if (area == 0) {
                    /*
                     * consider the first polygon as the outer polygon
                     */
                    polygon = poly;
                    area = bg::area(poly);
                } else {
                    auto new_area = bg::area(poly);
                    if (new_area > area) {
                        /*
                         * if new poly is larger than the current outer polygon
                         * - save current outer polygon as inner ring
                         * - save new poly as outer polygon
                         */
                        polys.push_back(polygon);
                        area = new_area;
                        polygon = poly;
                    } else {
                        /*
                         * New poly is inner ring
                         */
                        polys.push_back(poly);
                    }
                }
            }
        }

        /*
         * Add inner rings to the polygon
         */
        if (!polys.empty()) {
            polygon.inners().resize(polys.size());
            int i(0);
            for (const auto inner_ring : polys) {
                bg::assign(polygon.inners()[i++], inner_ring);
            }
        }

        /*
         * Add the polygon to the alpha shape
         */
        shape.push_back(polygon);
    }

    return shape;
}




std::ostream&
operator<<(std::ostream& os, const Pgr_alphaShape &d) {
    os << d.graph;

    return os;
}

}  // namespace alphashape
}  // namespace pgrouting
