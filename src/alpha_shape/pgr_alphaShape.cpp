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
        return Bpoly{{graph[a].point, graph[b].point, graph[c].point}};
}

std::string
to_insert(std::set<std::set<E>> name, std::string kind, const G &graph) {
    std::ostringstream str;
    for (const auto face : name) {
        std::vector<E> edges(face.begin(), face.end());
        auto a = graph.source(edges[0]);
        auto b = graph.target(edges[0]);
        auto c = graph.source(edges[1]);
        c = (c == a || c == b)? graph.target(edges[1]) : c;
        pgassert(a != b && a != c && b!= c);
        Bpoly geom{{graph[a].point, graph[b].point, graph[c].point}};
        str << "\ninsert into tbl_2 (geom, kind) values (st_geomfromtext('"
            <<  bg::wkt(geom) << "'), " << kind +");";
    }
    return str.str();
}

std::string
to_insert(std::set<E> name, std::string kind, const G &graph) {
    std::ostringstream str;
    for (const auto edge : name) {
        auto a = graph.source(edge);
        auto b = graph.target(edge);

        Bline geom{{graph[a].point, graph[b].point}};
        str << "\ninsert into tbl_2 (geom, kind) values (st_geomfromtext('"
            <<  bg::wkt(geom) << "'), " << kind +");";
    }
    return str.str();
}


std::string
to_insert(std::deque<V> name, std::string kind, const G &graph) {
    std::ostringstream str;
    Bpoly geom;
    for (const auto v : name) {
        auto a = graph[v].point;
        bg::append(geom,  a);
    }
    str << "\ninsert into tbl_2 (geom, kind) values (st_geomfromtext('"
            <<  bg::wkt(geom) << "'), " << kind +");";
    return str.str();
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
    pgassert(m_adjacent_triangles.size() == m_triangles.size());



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


    if (alpha <= 0) return result;

    std::vector<Bline> not_inalpha;
    std::vector<Bline> inalpha;

#if 0
    double min_r(std::numeric_limits<double>::max());
    double max_r(0);
#endif

    /* triangle:
     * a =  B-C = v-w
     * b =  A-C = u-w
     * c =  A-B = u-v
     *
     * A is geometry  u is vertex descriptor
     * B is geometry  v is vertex descriptor
     * C is geometry  w is vertex descriptor
     */
    for (const auto t : m_triangles) {
        std::vector<E> edges(t.begin(), t.end());
        auto a = graph.source(edges[0]);
        auto b = graph.target(edges[0]);
        auto c = graph.source(edges[1]);
        c = (c == a || c == b)? graph.target(edges[1]) : c;
        pgassert(a != b && a != c && b!= c);

        /*
         * face belongs to alpha shape?
         */
        bool belongs(false);

#if 0
        log << "\n****** working with" << bg::wkt(Bpoly{{graph[a].point, graph[b].point, graph[c].point}});
#endif

        auto center = circumcenter(graph[a].point, graph[b].point, graph[c].point);
        auto radius = bg::distance(center, graph[a].point);

#if 0
        min_r = radius < min_r? radius : min_r;
        max_r = radius > max_r? radius : max_r;
#endif

        if (radius < alpha) {
            set_of_faces.insert(t);
            belongs = true;
        } else {
            exterior.insert(t);
            belongs = false;
        }

        std::set<E> is_incident;

        for (const auto edge : edges) {
            /*
             * working each edge of triangle
             */
            auto u = graph.source(edge);
            auto v = graph.target(edge);
            auto centers = possible_centers(graph[u].point, graph[v].point, alpha);

            pgassert(!(belongs && centers.empty()));

            auto v_intersection = get_intersection(u, v, graph.graph);

            bool in_hull = false;
            if (v_intersection.size() == 1) {
                hull.push_back(edge);
                if (belongs) {
                    in_border.edges.insert(edge);
                    in_hull = true;
                    log << "edge in hull";
                } else if (bg::distance(graph[u].point, graph[v].point) / 2 < alpha) {
                    lone_edges.insert(edge);
                }
                continue;
            }

            for (const auto w : v_intersection) {
                if (w == a || w == b || w == c) continue;
                /*
                 * adjacent triangle (u,v,w)
                 */
                auto center = circumcenter(graph[u].point, graph[v].point, graph[w].point);
                auto radius = bg::distance(center, graph[u].point);
                if (radius < alpha) {
                    /* is incident to a face */
                    is_incident.insert(edge);
                } else {
                    if (belongs) in_border.edges.insert(edge);
                    else if (bg::distance(graph[u].point, graph[v].point) / 2 < alpha) {
                        lone_edges.insert(edge);
                    }
                }
            } //  for each adjacent triangle
        } // for each edge of triangle

        if (belongs) {
            if (is_incident.size() == 3) interior.insert(t);
            if (is_incident.size() == 2) regular_two.insert(t);
            if (is_incident.size() == 1) regular_one.insert(t);
            if (is_incident.size() == 0) {
                singular.insert(t);
                singular_borders.insert(t.begin(), t.end());
            }
        } else {
            if (is_incident.size() == 3) {
                face_is_hole.insert(t);
                hole_edges.insert(is_incident.begin(), is_incident.end());
            }
        }
    } // for each triangle

    std::set<E> v_difference;
    std::set_difference(in_border.edges.begin(), in_border.edges.end(),
            hole_edges.begin(), hole_edges.end(),
            std::inserter(v_difference, v_difference.end()));

    in_border.edges = v_difference;

    v_difference.clear();
    std::set_difference(in_border.edges.begin(), in_border.edges.end(),
            singular_borders.begin(), singular_borders.end(),
            std::inserter(v_difference, v_difference.end()));

    in_border.edges = v_difference;


    log << "\n- singluar.size()" << singular.size();
    log << "\n- regular_one.size()" << regular_one.size();
    log << "\n- regular_two.size()" << regular_two.size();
    log << "\n- interior.size()" << interior.size();
    log << "\n- exterior.size()" << exterior.size();
    log << "\n- m_triangles.size()" << m_triangles.size();
    log << "\n- face_is_hole.size()" << face_is_hole.size();
    log << "\n- hole_edges.size()" << hole_edges.size();
    log << "\n- singular_borders.size()" << singular_borders.size();
    log << "\n- in_border.size()" << in_border.edges.size();

    log << "drop table tbl_2; create table tbl_2 (gid serial, geom geometry, kind integer);";
#if 0
    log << to_insert(singular, "1", graph);
    log << to_insert(regular_one, "2", graph);
    log << to_insert(regular_two, "3", graph);
    log << to_insert(interior, "4", graph);
    log << to_insert(exterior, "5", graph);
    log << to_insert(face_is_hole, "6", graph);
    log << to_insert(in_border.edges, "7", graph);
    log << to_insert(lone_edges, "8", graph);
    log << to_insert(hole_edges, "9", graph);
    log << to_insert(singular_borders, "10", graph);
#endif

    /*
     * Building the polygons from the obtained borders
     */
    std::set<E> used_edges;
    BGL_FORALL_VERTICES(source, graph.graph, BG) {
        /* cycling all vertices to get the shortest path */
        using Subgraph = boost::filtered_graph<BG, EdgesFilter, boost::keep_all>;
        Subgraph subg1 (graph.graph, in_border, {});
        V v_target;
        E edge;
        bool found(false);
        BGL_FORALL_OUTEDGES(source, e, subg1, Subgraph) {
            edge = e;
            v_target = boost::source(e, subg1) == source?
                boost::target(e, subg1) : boost::source(e, subg1);
            found = true;
            break;
        }
        if (!found) continue;

#if 0
        log << "\n" << source << "->" << v_target;
#endif

        /*
         * Removing fron the graph the edge
         */

        in_border.edges.erase(edge);
        Subgraph subg (graph.graph, in_border, {});

        auto predecessors = get_predecessors(source, v_target, subg);
        auto polygon = get_polygon(source, v_target, predecessors, subg);
        if (bg::num_points(polygon) >= 3) border.push_back(polygon);
        log << "\nINSERT INTO tbl_2 (geom, kind) VALUES (st_geomfromtext('" << bg::wkt(polygon) << "'), 11);";
    }

    /*
     * Building faces that are holes
     */
    Bpolys holes;
    for (const auto triangle : face_is_hole) {
        holes.push_back(triangle_to_polygon(triangle, graph));
    }

    /*
     * Removing holes
     */
    std::vector<Bpoly> withoutholes;
    for (const auto p : border) {
        std::vector<Bpoly> the_diff;
        boost::geometry::difference(p, holes, the_diff);
        pgassert(!the_diff.empty());
        for (const auto new_p : the_diff) withoutholes.push_back(new_p);
    }

    /*
     * Building the polygons from the singular triangles
     * Cant have holes
     */
    for (const auto triangle : singular) {
        withoutholes.push_back(triangle_to_polygon(triangle, graph));
        border.push_back(triangle_to_polygon(triangle, graph));
    }

    //for(const auto p : withoutholes) border.push_back(p);


#if 1
    return border;
#else
    return green;
#endif
}




std::ostream&
operator<<(std::ostream& os, const Pgr_delauny &d) {
    os << d.graph;

    return os;
}

}  // namespace alphashape
}  // namespace pgrouting
