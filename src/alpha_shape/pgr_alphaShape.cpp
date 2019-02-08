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
#include <limits>
#include <sstream>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/connected_components.hpp>
#include "mst/visitors.hpp"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/geometry/algorithms/union.hpp>

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

#if 0
std::vector<Bpoint>
possible_centers(const Bpoint p1, const Bpoint p2, const double alpha_radius) {
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

    /*
     * The segment is not alpha
     */
    if (!(alpha_radius > a)) return centers;

    auto m = - std::sqrt((alpha_radius + a) * (alpha_radius - a)) / a;

    centers.push_back({rombus_center.x() + m * p_a.y(), rombus_center.y() - m * p_a.x()});
    centers.push_back({rombus_center.x() - m * p_a.y(), rombus_center.y() + m * p_a.x()});

    return centers;
}

Bpoly
triangle_to_polygon(std::set<E> triangle, const G &graph) {
        std::vector<E> edges(triangle.begin(), triangle.end());
        auto a = graph.source(edges[0]);
        auto b = graph.target(edges[0]);
        auto c = graph.source(edges[1]);
        c = (c == a || c == b)? graph.target(edges[1]) : c;
        pgassert(a != b && a != c && b!= c);
        Bpoly tri {{graph[a].point, graph[b].point, graph[c].point}};
        bg::correct(tri);
        return tri;
}



std::set<V>
get_intersection(V u, V v, const BG& graph) {
            std::set<V> adjacent1, adjacent2, v_intersection;
            BGL_FORALL_ADJ(u, w, graph, BG) {
                adjacent1.insert(w);
            }
            BGL_FORALL_ADJ(v, w, graph, BG) {
                adjacent2.insert(w);
            }
            std::set_intersection(adjacent1.begin(), adjacent1.end(),
                    adjacent2.begin(), adjacent2.end(),
                    std::inserter(v_intersection, v_intersection.end()));
            return v_intersection;
}
#endif

template <typename V>
class dijkstra_one_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_one_goal_visitor(V goal) : m_goal(goal) {}
          template <class B_G>
          void examine_vertex(V &u, B_G &) {
              if (u == m_goal) throw found_goals();
          }
      private:
          V m_goal;
};

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
                .visitor(dijkstra_one_goal_visitor<V>(target)));
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
get_polygon(V source, V target, const std::vector<V> & predecessors, const B_G &graph) {
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
            target = predecessors[target];
        }
        bg::correct(polygon);
        return polygon;
}

}  // namespace

/*
 * Constructor
 */
Pgr_delauny::Pgr_delauny(const std::vector<Pgr_edge_xy_t> &edges) :
graph(UNDIRECTED) {
    graph.insert_edges(edges);
    get_triangles();
}

/* triangle:
 * a =  B-C = v-w
 * b =  A-C = u-w
 * c =  A-B = u-v
 *
 * A is geometry  u is vertex descriptor
 * B is geometry  v is vertex descriptor
 * C is geometry  w is vertex descriptor
 */
void
Pgr_delauny::get_triangles() {
    std::set< std::set<E> > triangles;
    std::map<E, std::set<Triangle>> adjacent_triangles;
    std::map<Triangle, std::set<Triangle>> adjacent_triangles1;

    BGL_FORALL_EDGES(c, graph.graph, BG) {
        auto u = graph.source(c);
        auto v = graph.target(c);
        Bpoint A {graph[u].point};
        Bpoint B {graph[v].point};

        std::set<E> s_outedges;
        std::vector<Triangle> adjacent_to_edge;

        size_t i = 0;
        BGL_FORALL_OUTEDGES(u, b, graph.graph, BG) {
            ++i;
            auto w = graph.adjacent(u, b);
            if (w == v) {
                pgassert(b == c);
                continue;
            };

            auto a_r = boost::edge(v, w, graph.graph);
            if (!a_r.second) continue;

            std::set<E> face{{a_r.first, b, c}};
            triangles.insert(face);
            adjacent_triangles[a_r.first].insert(face);
            adjacent_triangles[b].insert(face);
            adjacent_triangles[c].insert(face);
            adjacent_to_edge.push_back(face);
        }
        pgassert(i > 1);
        if (adjacent_to_edge.size() == 2) {
            m_adjacent_triangles[adjacent_to_edge[0]].insert(adjacent_to_edge[1]);
            m_adjacent_triangles[adjacent_to_edge[1]].insert(adjacent_to_edge[0]);
        }
    }

#if 0
    size_t i = 0;
    for (const auto faces : m_adjacent_triangles) {
        log << "\n" << i++;
        for (const auto e : faces.first) {
            log << e << ":";
        }
        log << faces.second.size() << "\t";
        for (const auto e : faces.second) {
            log << e << ",";
        }
    }
#endif


#if 0
    size_t i = 0;
    for (const auto faces : adjacent_triangles) {
        log << "\n" << i++;
        size_t j = 0;
        for (const auto f : faces.second) {
            log << "\t" << j++;
            for (const auto e : f) {
                log << e << ",";
            }
        }
    }
#endif
    m_triangles.reserve(triangles.size());
    m_triangles.insert(m_triangles.begin(), triangles.begin(),triangles.end());
    //pgassert(m_adjacent_triangles.size() == m_triangles.size());



    /*
     * calculating center & radius
     */
    for (const auto t : m_triangles) {
        std::vector<E> edges(t.begin(), t.end());
        auto a = graph.source(edges[0]);
        auto b = graph.target(edges[0]);
        auto c = graph.source(edges[1]);
        c = (c == a || c == b)? graph.target(edges[1]) : c;

        auto center = circumcenter(graph[a].point, graph[b].point, graph[c].point);
        auto radius = bg::distance(center, graph[a].point);

        m_centers.push_back(center);
        m_radius.push_back(radius);
    }
}

void
Pgr_delauny::remove(const Triangle from, const Triangle del) {
    m_adjacent_triangles[from].erase(del);
}

/*
 * The whole face belongs to the shape?
 */
bool
Pgr_delauny::isIncident(const Triangle t, double alpha) const {
        std::vector<E> edges(t.begin(), t.end());
        auto a = graph.source(edges[0]);
        auto b = graph.target(edges[0]);
        auto c = graph.source(edges[1]);
        c = (c == a || c == b)? graph.target(edges[1]) : c;
        pgassert(a != b && a != c && b!= c);

        auto center = circumcenter(graph[a].point, graph[b].point, graph[c].point);
        auto radius = bg::distance(center, graph[a].point);

        return radius < alpha;
}

void
Pgr_delauny::recursive_build(const Triangle face, std::set<Triangle> &used, std::set<E> &border_edges, double alpha) const {
    auto original = used.size();
    used.insert(face);

    if (original == used.size())  return;
    if (!isIncident(face, alpha)) {
#if 1
        /*
         * if length(edge) = 2 * alpha
         * then edge is in border
         */
        for (const auto e : face) {
            auto u = graph.source(e);
            auto v = graph.target(e);
            Bpoint A {graph[u].point};
            Bpoint B {graph[v].point};
            if (bg::distance(A, B) < 2 * alpha) {
                m_lone_edges.insert(e);
            }
        }
#endif
        return;
    }
    std::set<E> e_intersection;

    for (const auto adj_t : m_adjacent_triangles[face]) {
        if (!isIncident(adj_t, alpha)) {
#if 0
            log << "\ncurrent edges: ";
            for (const auto e : face) {
                log << e << ", ";
            }
            log << "\tadjacent edges:";
            for (const auto e : adj_t) {
                log << e << ", ";
            }
#endif
            std::set_intersection(
                    face.begin(), face.end(),
                    adj_t.begin(), adj_t.end(),
                    std::inserter(border_edges, border_edges.end()));
#if 0
            log << "\n border edges: ";
            for ( const auto e : border_edges) {
                log << e << ", ";
            }
            std::set_intersection(
                    face.begin(), face.end(),
                    adj_t.begin(), adj_t.end(),
                    std::inserter(e_intersection, e_intersection.end()));
            continue;
#endif
        }
        recursive_build(adj_t, used, border_edges, alpha);
        std::set_intersection(
                face.begin(), face.end(),
                adj_t.begin(), adj_t.end(),
                std::inserter(e_intersection, e_intersection.end()));
    }

    if (m_adjacent_triangles[face].size() == 2) {
        /*
         * One edge is on the Hull edge
         * No adjacent triangle
         */
        std::set_difference(
                face.begin(), face.end(),
                e_intersection.begin(), e_intersection.end(),
                std::inserter(border_edges, border_edges.end()));
#if 0
        log << "\n border edges: ";
        for ( const auto e : border_edges) {
            log << e << ", ";
        }
#endif
    }

#if 0
    log << "\n exit current edges: ";
    for (const auto e : face) {
        log << e << ", ";
    }
#endif
}

#if 1
std::vector<Bpoly>
#else
Bpolys
#endif
Pgr_delauny::operator() (double alpha) const {
    log << "starting calculation\n";
    Bpolys result;
    std::vector<Bpoly> border;
    std::vector<E> hull;
    std::vector<Bpoly> faces;
    std::set< std::set<E> > set_of_faces;
    std::set< std::set<E> > alpha_complex;
    std::set< std::set<E> > not_alpha_complex;
    std::set< std::set<E> > interior;
    std::set< std::set<E> > exterior;
    std::set< std::set<E> > regular_one;
    std::set< std::set<E> > regular_two;
    std::set< std::set<E> > singular;
    std::set< std::set<E> > face_is_hole;
    EdgesFilter in_border;
    std::set<E> singular_borders;
    std::set<E> lone_edges;
    std::set<E> hole_edges;
    std::set<E> boundry;

    m_lone_edges.clear();

    if (alpha <= 0) return result;

    std::vector<Bline> not_inalpha;
    std::vector<Bline> inalpha;

    std::set<Triangle> used;
    using Subgraph = boost::filtered_graph<BG, EdgesFilter, boost::keep_all>;

    std::set<E> all_border_edges;
    for (const auto t : m_adjacent_triangles) {
        EdgesFilter border_edges;
        recursive_build(t.first, used, border_edges.edges, alpha);
        if (border_edges.edges.empty()) continue;

        all_border_edges.insert(border_edges.edges.begin(), border_edges.edges.end());

        log << "\n found:" << border_edges.edges.size();
        for (const auto edge : border_edges.edges) {
            log << edge << ",";
        }

        std::vector<Bpoly> polys;
        Bpoly polygon;
        double area = 0;
        while (!border_edges.edges.empty()) {
            auto first_edge = *border_edges.edges.begin();
            border_edges.edges.erase(first_edge);

            Subgraph subg (graph.graph, border_edges, {});
            auto source = boost::source(first_edge, subg);
            auto target = boost::target(first_edge, subg);


            auto predecessors = get_predecessors(source, target, subg);
            auto poly = get_polygon(source, target, predecessors, subg);

            if (bg::num_points(poly) >= 3) {
                if (area == 0) {
                    polygon = poly;
                    area = bg::area(poly);
                } else {
                    auto new_area = bg::area(poly);
                    if (new_area > area) {
                        polys.push_back(polygon);
                        area = new_area;
                        polygon = poly;
                    } else {
                        polys.push_back(poly);
                    }

                }
            }
        }
        if (!polys.empty()) {
            polygon.inners().resize(polys.size());
            int i(0);
            for (const auto inner_ring : polys) {
                bg::assign(polygon.inners()[i++], inner_ring);
            }
        }
        border.push_back(polygon);
        log << "\nINSERT INTO tbl_2 (geom, kind) VALUES (st_geomfromtext('" << bg::wkt(polygon) << "'), 12);";
    }
    for (const auto e : m_lone_edges) {
         auto a = graph.source(e);
         auto b = graph.target(e);
        Bline line{{graph[a].point, graph[b].point}};
        log << "\nINSERT INTO tbl_2 (geom, kind) VALUES (st_geomfromtext('" << bg::wkt(line) << "'), 16);";

    }
    return border;
}




std::ostream&
operator<<(std::ostream& os, const Pgr_delauny &d) {
    os << d.graph;

    return os;
}

}  // namespace alphashape
}  // namespace pgrouting
