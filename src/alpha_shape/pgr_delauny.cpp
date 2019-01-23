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
        std::vector<size_t> point_idx(3);
        std::vector<Bpoint> triangle_points(3);
        for (auto d : m_delauny) {
            point_idx[i] = d.pid;
            triangle_points[i] = Bpoint(d.x, d.y);
            if (i == 2) {
                auto tid = m_triangles.size();
                m_triangles.push_back(Pgr_triangle(triangle_points));
                for (auto p : point_idx) {
                    m_relation[p].push_back(tid);
                }
            }
            i = i == 2? 0 : i + 1;
        }

        /*
         * Saving delauny edges information
         */
        for (auto t : m_triangles) {
        }
        log << *this;
}

void
Pgr_delauny::clear() {
    m_relation.clear();
    m_triangles.clear();
    m_points.clear();
}

std::vector<Bpoint>
Pgr_delauny::possible_centers(const Bpoint p1, const Bpoint p2, const double r) {
    std::vector<Bpoint> points;

    double x1 = p1.x();
    double x2 = p2.x();
    double y1 = p1.y();
    double y2 = p2.y();

    double ca = (x1 * x1) + (y1 * y1) - (x2 * x2) - (y2 * y2);


    // This are common stuff
    double x3 = x1 - x2;
    double y3 = y1 - y2;

    Bpoint p3(p1);
    boost::geometry::subtract_point(p3, p2);
    pgassert(p3.x() == x3);
    pgassert(p3.y() == y3);


    double cy = (ca/(2 * y3)) - y1;

    double a = ((x3 * x3) + (y3 * y3)) / (y3 * y3);
    double b = ((2 * x1) - (2 * (cy) * x3/y3));
    double c = ((x1 * x1) + (cy * cy) - (r * r));

    std::vector<double> roots;

    double denominator = 2 * a;
    double bNumerator = -b;
    double underSquare = (b * b) - (4 * a * c);
    if (underSquare < 0 || denominator == 0) {
        // imaginary roots
    } else {
        double sqrt = std::sqrt(underSquare);
        roots = {(bNumerator + sqrt) / denominator, (bNumerator - sqrt) / denominator};
    }

    for (auto root : roots) {
        double x = root;
        double y = -(x3/y3) * x + (c/(2 * y3));
        points.push_back(Bpoint(x, y));
    }

    return points;
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
    boost::geometry::convex_hull(d.m_points, hull);
    os << boost::geometry::wkt(hull) << "\t";


#if 0
    double alpha = 0.6;
    std::set<Bpoint, compare_points> invalid_points;
    for (auto t : d.m_triangles) {
        auto invalid = t.invalid_points(alpha);
        invalid_points.insert(invalid.begin(), invalid.end());
    }

    os << "\nINVALID\n";
    for (auto p : invalid_points) {
        os << boost::geometry::wkt(p) << ", ";
    }
#endif
    return os;
}

}  // namespace alphashape
}  // namespace pgrouting
