/*PGR-GNU*****************************************************************
file: pgr_triangle.cpp

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

#include "alphaShape/pgr_triangle.hpp"
#include <sstream>
#include "cpp_common/pgr_assert.h"

namespace pgrouting {
namespace alphashape {


namespace detail {
Bpoint
midpoint(Bpoint a, const Bpoint &b) {
    boost::geometry::add_point(a, b);
    boost::geometry::divide_value(a, 2);
    return a;
}

bool
point_within_circle(
        Bpoint p1,  // point on circle
        Bpoint p2,  // point on circle,
        Bpoint p3,  // point within circle?
        double alpha ) {
    if (alpha == 0) return true;
    double radius = 1 / alpha;

    /*
     * Center of the rombus
     */
    auto mid = detail::midpoint(p1, p2);


    auto x_a = mid.x() - p1.x();
    auto y_a = mid.y() - p1.y();

    /*
     * Length of the diagonal of the rombus
     */
    auto a = boost::geometry::distance(p1, p2);

    /*
     * Length of the diagonal other diagonal
     */
    auto b = std::sqrt(radius * radius - a * a);

    auto center1 = Bpoint(
            p1.x() + b * y_a / a,
            p1.y() - b * x_a / a);

    auto center2 = Bpoint(
            p1.x() - b * y_a / a,
            p1.y() + b * x_a / a);

    return boost::geometry::distance(center1, p3) < radius
            && boost::geometry::distance(center2, p3) < radius;
}

}  // namespace detail

#if 0
Pgr_triangle::Pgr_triangle(
             Bpoint p1, Bpoint p2, Bpoint p3) {
    circle_info();
    m_poly.outer().push_back(p1);
    m_poly.outer().push_back(p2);
    m_poly.outer().push_back(p3);
}
#endif

Pgr_triangle::Pgr_triangle(const std::vector<Bpoint> &p_points) {
    circle_info();
    m_poly.outer().push_back(p_points[0]);
    m_poly.outer().push_back(p_points[1]);
    m_poly.outer().push_back(p_points[2]);
}

bool
Pgr_triangle::has_point(const Bpoint &p) const {
    return boost::geometry::within(p, m_poly);
#if 0
    return boost::geometry::equals(p, m_p1)
       || boost::geometry::equals(p, m_p2)
       || boost::geometry::equals(p, m_p3);
#endif
}

bool
Pgr_triangle::has_edge(const Bpoint &p1, const Bpoint &p2) const {
    return has_point(p1) && has_point(p2);
}

std::ostream&
operator<<(std::ostream& os, const Pgr_triangle &t) {
    double alpha = 1;
    os << boost::geometry::wkt(t.m_poly);
#if 0
    os << " Center: " << boost::geometry::wkt(t.m_center) << " radius: " << t.m_radius
        << " alpha: " << t.alpha()
        << " invalid points 0.1: " << t.invalid_points(alpha).size();
    for (auto p : t.invalid_points(alpha)) {
        os << boost::geometry::wkt(p) << ", ";
    }
#endif
    return os;
}

bool
Pgr_triangle::operator==(const Pgr_triangle &p2) {
    return boost::geometry::equals(m_poly, p2.m_poly);
}

void
Pgr_triangle::circle_info() {
#if 0
    Bpoint d1 = m_p1;
    Bpoint d2 = m_p2;
    Bpoint d3 = m_p3;
    boost::geometry::subtract_point(d2, d1);
    boost::geometry::subtract_point(d3, d1);
    auto k = d2.x() * d1.y() - d2.y() * d1.x();
    pgassert((k < -0.00001) || (k > 0.00001));

    auto s1 = detail::midpoint(d1, d2);
    auto s2 = detail::midpoint(d1, d3);

    auto l = d1.x() * (s2.y() - s1.y()) - d1.y() * (s2.x() - s1.x());
    auto m = l / k;

    m_center = Bpoint(s2.x() + m * d2.x(), s2.y() + m * d2.y());

    m_radius = boost::geometry::distance(m_center, d1);
#endif
}

std::vector<Bpoint>
Pgr_triangle::invalid_points(double alpha) const {
    std::vector<Bpoint> points;
#if 0
    if (detail::point_within_circle(m_p1, m_p2, m_p3, alpha)) {
        points.push_back(m_p3);
    }
    if (detail::point_within_circle(m_p1, m_p3, m_p2, alpha)) {
        points.push_back(m_p2);
    }
    if (detail::point_within_circle(m_p2, m_p3, m_p1, alpha)) {
        points.push_back(m_p1);
    }
#endif
    return points;
}

}  // namespace alphashape
}  // namespace pgrouting
