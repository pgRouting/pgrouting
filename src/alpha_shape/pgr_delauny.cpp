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

#include <vector>
#include <algorithm>

#include "cpp_common/bpoint.hpp"
#include "cpp_common/pgr_assert.h"
#include "alphaShape/pgr_triangle.hpp"

namespace bg = boost::geometry;

namespace pgrouting {
namespace alphashape {

namespace detail {
void
cleanup_data(std::vector<Delauny_t> &delauny) {
        std::sort(delauny.begin(), delauny.end(),
                [](const Delauny_t &e1, const Delauny_t &e2)->bool {
                    return e1.pid < e2.pid;
                });
        std::stable_sort(delauny.begin(), delauny.end(),
                [](const Delauny_t &e1, const Delauny_t &e2)->bool {
                    return e1.tid < e2.tid;
                });
        delauny.erase(std::unique(delauny.begin(), delauny.end(),
                [](const Delauny_t &e1, const Delauny_t &e2)->bool {
                    return e1.tid == e2.tid && e1.pid == e2.pid;
                }), delauny.end());
}
}  // namespace detail

std::vector<Bpoint>
Pgr_delauny::possible_centers(const Bpoint p1, const Bpoint p2, const double r) const {
    std::vector<Bpoint> centers;
    pgassert(!bg::equals(p1, p2));

    double x1 = p1.x();
    double x2 = p2.x();
    double y1 = p1.y();
    double y2 = p2.y();

    auto rombus_center = p2;
    bg::add_point(rombus_center, p1);
    bg::divide_value(rombus_center, 2);

    auto p_a = p2;
    bg::subtract_point(p_a, p1);
    bg::divide_value(p_a, 2);

    log << "\t p_a " << bg::wkt(p_a);
    Bpoint origin(0, 0);
    log << "\t origin " << bg::wkt(origin);

    auto a1 = bg::distance(p_a, origin);
    log << "\t a " << a1 << "\tr " << r;

    if (!(r > a1)) return centers;
    auto b1 = std::sqrt((r + a1) * (r - a1));
    log << "\t b " << b1;

    auto m = - b1 / a1;
    // recta que pasa por (x0 + x1)/2 , (y0 + y1)/ 2 com pendiente m
    // y = mx + b

    Bpoint c1(rombus_center.x() + m * p_a.y(), rombus_center.y() - m * p_a.x());
    Bpoint c2(rombus_center.x() - m * p_a.y(), rombus_center.y() + m * p_a.x());
    centers.push_back(c1);
    centers.push_back(c2);

    return centers;
}

Pgr_delauny::Pgr_delauny(
             const std::vector<Bpoint> &p_points,
             const std::vector<Delauny_t> &p_delauny) :
   // m_points(p_points),
    m_delauny(p_delauny) {
        for (const auto p : p_points) {
            boost::geometry::append(m_points, p);
        }
        /*
         * Inserting points from delauny inforamtion
         */

        for (auto &d : m_delauny) {
            Bpoint p(d.x, d.y);
            auto point_itr = std::find_if(m_points.begin(), m_points.end(),
                    [&p](const Bpoint &p1)->bool {
                        return boost::geometry::equals(p, p1);
                    });

            if (point_itr == m_points.end()) {
                /*
                 * Point is not found
                 */
                d.pid = m_points.size();
                m_points.push_back(p);
            } else {
                d.pid = point_itr - m_points.begin();
            }
        }

        /*
         * removing duplicate triangles information
         */
        detail::cleanup_data(m_delauny);


        /*
         * creating the triangles
         */
        size_t i(0);
        size_t line_num(0);
        std::vector<size_t> point_idx(3);
        std::vector<Bpoint> triangle_points(3);
        //m_lines.resize(1);
        m_lines.resize(m_delauny.size());

        for (const auto d : m_delauny) {
            point_idx[i] = d.pid;
            triangle_points[i] = Bpoint(d.x, d.y);
            if (i == 2) {
                auto tid = m_triangles.size();
                m_triangles.push_back(Pgr_triangle(triangle_points));
                for (auto p : point_idx) {
                    m_relation[p].push_back(tid);
                }
            }
            /*
             * Saving delauny edges information
             */
            if (i == 1) {
                boost::geometry::append(m_lines[line_num], Bpoint(triangle_points[0]));
                boost::geometry::append(m_lines[line_num], Bpoint(triangle_points[1]));
                ++line_num;
            } else if (i == 2) {
                boost::geometry::append(m_lines[line_num], Bpoint(triangle_points[0]));
                boost::geometry::append(m_lines[line_num], Bpoint(triangle_points[2]));
                ++line_num;
                boost::geometry::append(m_lines[line_num], Bpoint(triangle_points[1]));
                boost::geometry::append(m_lines[line_num], Bpoint(triangle_points[2]));
                ++line_num;
            }

            i = i == 2? 0 : i + 1;
        }

        alpha_edges(1);
}


void
Pgr_delauny::alpha_edges(double alpha) const {
    if (alpha == 0) return;

    auto radius = 1 / alpha;
    std::vector<Bline> not_inalpha;
    std::vector<Bline> inalpha;
    std::vector<Bline> border;

    for (const auto line : m_lines) {
        log << "\n\nsegment " << boost::geometry::wkt(line);
        pgassert(line.size() == 2);
        auto centers = possible_centers(line[0], line[1], radius);
        for(const auto c : centers) {
            log << "\tcenter " << boost::geometry::wkt(c);
        };
        if (centers.empty()) {
            not_inalpha.push_back(line);
        } else {
            inalpha.push_back(line);
            /*
             * Is the segment in a border?
             */
            size_t count0(0);
            size_t count1(0);
            for (const auto p : m_points) {
                if (bg::equals(p, line[0]) || bg::equals(p, line[1])) continue;
                log << "\n" << bg::wkt(p);
                if (bg::distance(p, centers[0]) < radius) {
#if 0
                    log << "within circle" << " at center" << bg::wkt(centers[0]);
#endif
                    ++count0;
                }
                if (bg::distance(p, centers[1]) < radius) {
#if 0
                    log << "\twithin circle" << " at center" << bg::wkt(centers[1]);
#endif
                    ++count1;
                }
            }
            log << "\n" << count0 << "," << count1;
            if ((count0 && !count1) || (!count0 && count1)) {
                log << "**********an external edge" << bg::wkt(line);
                border.push_back(line);
            }
        }
    }

#if 0
    log << "\nNot on alpha ";
    for (const auto line : not_inalpha) {
        log << "\n" << boost::geometry::wkt(line);
    }

    log << "\nOn alpha ";
    for (const auto line : inalpha) {
        log << "\n" << boost::geometry::wkt(line);
    }
#endif

    log << "\nOn external ";
    Blines alpha_edges;

    for (const auto line : border) {
        log << "\n" << boost::geometry::wkt(line);
    }

}



void
Pgr_delauny::clear() {
    m_relation.clear();
    m_delauny.clear();
    m_triangles.clear();
    m_points.clear();
    m_lines.clear();
}



struct compare_points {
    bool operator() (const Bpoint &lhs, const Bpoint &rhs) const {
        if (lhs.x() < rhs.x()) return true;
        if (lhs.x() > rhs.x()) return false;
        return lhs.y() < rhs.y();
    }
};


std::ostream&
operator<<(std::ostream& os, const Pgr_delauny &d) {
    os << "Points\n";
    for (const auto p : d.m_points) {
        os << boost::geometry::wkt(p) << ", ";
    };


    os << "\nDelauny info size" << d.m_delauny.size() <<  "\n";


    os << "\nDelauny triangles\n";
    for (const auto t : d.m_triangles) {
        os << t << ",\n";
    }

    os << "\nrelation\n";
    for (const auto r : d.m_relation) {
        os << "\n" << r.first << "->" << boost::geometry::wkt(d.m_points[r.first]) << ": ";
        for (const auto e : r.second) {
            os << "\n   " << e << "->" << d.m_triangles[e];
        }
    }

#if 0
    os << "\nadjacent\n";
    for (const auto t1 : d.m_triangles) {
        for (const auto t2 : d.m_triangles) {
            auto adj = t1.adjacent_segment(t2);
            os << "adjacent(" << t1 << ", " << t2 << ") =";
            for (const auto a : adj) {
                os << boost::geometry::wkt(a) << "\t";
            }
            os << "\n";
        }
    }

    os << "\nconvexHull\n";
    boost::geometry::model::polygon<Bpoint> hull;
    boost::geometry::convex_hull(d.m_lines, hull);
    os << boost::geometry::wkt(hull) << "\n";

    os << boost::geometry::wkt(d.m_lines) << "\t";
#endif



    return os;
}

}  // namespace alphashape
}  // namespace pgrouting
