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

    BGL_FORALL_EDGES(c, graph.graph, BG) {
        auto u = graph.source(c);
        auto v = graph.target(c);
        Bpoint A {graph[u].point};
        Bpoint B {graph[v].point};

        std::set<E> s_outedges;

        BGL_FORALL_OUTEDGES(u, b, graph.graph, BG) {
            auto w = graph.adjacent(u, b);
            if (w == v) {
                pgassert(b == c);
                continue;
            };

            auto a_r = boost::edge(v, w, graph.graph);
            if (!a_r.second) continue;

            std::set<E> face{{a_r.first, b, c}};
            triangles.insert(face);
        }
    }

    m_triangles.reserve(triangles.size());
    m_triangles.insert(m_triangles.begin(), triangles.begin(),triangles.end());

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


std::vector<Bpoly>
Pgr_delauny::operator()(double alpha) const {
    log << "starting calculation\n";
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
    std::set<E> in_border;
    std::set<E> lone_edge;
    std::set<E> boundry;


    if (alpha <= 0) return border;

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

#if 1
            auto v_intersection = get_intersection(u, v, graph.graph);
#else
            std::set<V> adjacent1, adjacent2, v_intersection;
            BGL_FORALL_ADJ(u, w, graph.graph, BG) {
                adjacent1.insert(w);
            }
            BGL_FORALL_ADJ(v, w, graph.graph, BG) {
                adjacent2.insert(w);
            }
            std::set_intersection(adjacent1.begin(), adjacent1.end(),
                    adjacent2.begin(), adjacent2.end(),
                    std::inserter(v_intersection, v_intersection.end()));
#endif

#if 0
            std::set_union(adjacent1.begin(), adjacent1.end(),
                    adjacent2.begin(), adjacent2.end(),
                    std::inserter(v_union, v_union.end()));
#endif

#if 0
            bool found0(false);
            bool found1(false);
#endif

            bool in_hull = false;
            if (v_intersection.size() == 1) {
                hull.push_back(edge);
                if (belongs) {
                    in_border.insert(edge);
                    in_hull = true;
                    log << "edge in hull";
                }
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
                        if (belongs) in_border.insert(edge);
                        else if (bg::distance(graph[u].point, graph[v].point) / 2 < alpha) {
                            lone_edge.insert(edge);
                        }
                }
            } //  for each adjacent triangle
        } // for each edge of triangle

        if (belongs) {
            if (is_incident.size() == 3) interior.insert(t);
            if (is_incident.size() == 2) regular_two.insert(t);
            if (is_incident.size() == 1) regular_one.insert(t);
            if (is_incident.size() == 0) singular.insert(t);
        } else {
            if (is_incident.size() == 3) face_is_hole.insert(t);
        }
    } // for each triangle

    log << "\n- singluar.size()" << singular.size();
    log << "\n- regular_one.size()" << regular_one.size();
    log << "\n- regular_two.size()" << regular_two.size();
    log << "\n- interior.size()" << interior.size();
    log << "\n- exterior.size()" << exterior.size();
    log << "\n- m_triangles.size()" << m_triangles.size();
    log << "\n- face_is_hole.size()" << face_is_hole.size();
    log << "\n- in_border.size()" << in_border.size();

    log << "drop table tbl_2; create table tbl_2 (gid serial, geom geometry, kind integer);";
    log << to_insert(singular, "1", graph);
    log << to_insert(regular_one, "2", graph);
    log << to_insert(regular_two, "3", graph);
    log << to_insert(interior, "4", graph);
    log << to_insert(exterior, "5", graph);
    log << to_insert(face_is_hole, "6", graph);
    log << to_insert(in_border, "7", graph);
    log << to_insert(lone_edge, "8", graph);

    return border;
}
#if 0
for (const auto f : faces) {
    log << "\nINSERT INTO tbl_2 (geom) VALUES (ST_geomFromText('"
        << bg::wkt(f) << "'));";
}

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

        for (const auto v : v_intersection) {
            log << "\ntriangle" << bg::wkt(Bpoly{{source, target, graph[v].point}});
            auto center = circumcenter(source, target, graph[v].point);
            log << " circumcenter" << bg::wkt(center);
            log << " distances" << bg::distance(center, source) << "," << bg::distance(center, target) << bg::distance(center, graph[v].point);
            auto radius = bg::distance(center, source);
            min_r = radius < min_r ? radius : min_r;
            max_r = radius > max_r ? radius : max_r;
            /* Semi Expensive assertion */

            BGL_FORALL_VERTICES(u, graph.graph, BG) {
                if (u == v_source || u == v_target || u == v) continue;
                if (bg::distance(center, graph[u].point) < radius) log << "ILLEGAL TRIANGLE";
            }
        }


        for (const auto v : v_intersection) {
            auto p(graph[v].point);
            auto center = circumcenter(source, target, p);
            auto radius = bg::distance(center, source);
#if 1
            log << "\n-" << graph.graph[v];
#endif
            if (radius < alpha) {
#if 1
                log << "- radius < alpha" << radius << "<" << alpha;
                log << "\ninserting " << bg::wkt(Bline{{source, target}});
#endif
                E e = edge;
                m_in_border.edges.insert(e);
                break;
            } else {
                log << "- radius > alpha" << radius << ">" << alpha;
            }

#if 1
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
        }
#if 1
        log << "\ncounts" << count0 << "-" << count1;
        log << "\nfounds" << found0 << "-" << found1;
#endif

        if (false && !(found0 && found1)) {
            log << "\ninserting " << bg::wkt(Bline{{source, target}});
            E e = edge;
            m_in_border.edges.insert(e);
        }
    }
}

log << "\nMIN_R" << min_r;
log << "\nMAX_R" << max_r;

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
#endif



    std::ostream&
        operator<<(std::ostream& os, const Pgr_delauny &d) {
            os << d.graph;

            return os;
        }

}  // namespace alphashape
}  // namespace pgrouting
