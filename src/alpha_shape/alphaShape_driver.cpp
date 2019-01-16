/*PGR-GNU*****************************************************************
FILE: alphaShape_driver.cpp

Copyright (c) 2018 Vicky Vergara
Mail: project@pgrouting.org

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

#include "drivers/alpha_shape/alphaShape_driver.h"

#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>


using K     = CGAL::Exact_predicates_inexact_constructions_kernel;
using FT    = K::FT;
using Point = K::Point_2;
using Segment = K::Segment_2;
using Vb    = CGAL::Alpha_shape_vertex_base_2<K>;
using Fb    = CGAL::Alpha_shape_face_base_2<K>;
using Tds   = CGAL::Triangulation_data_structure_2<Vb,Fb>;
using Triangulation_2   = CGAL::Delaunay_triangulation_2<K,Tds>;
using Alpha_shape_2     = CGAL::Alpha_shape_2<Triangulation_2>;


// ---------------------------------------------------------------------

char *
delauny_query(
        char * points_query) {
    try {
        std::ostringstream delauny_sql;

        delauny_sql << "WITH "
            << "vertices AS (" << points_query << "), "
            << "the_geom AS (SELECT ST_makePoint(x, y) AS geom FROM vertices), "
            << "the_points AS (SELECT a.path[1] AS tid, (ST_DumpPoints(a.geom)).* "
            << "FROM (SELECT (ST_Dump(ST_DelaunayTriangles(ST_union(geom), 0 , 0))).* "
            << "FROM the_geom) AS a) "
            << "SELECT tid, path[2] AS pid, ST_X(geom) AS x, ST_y(geom) AS y "
            << "FROM the_points WHERE path[2] != 4 ";

            return pgr_msg(delauny_sql.str().c_str());
    } catch (...) {
        return nullptr;
    }
}


size_t
points_size(
        Pgr_point_t *vertices,
        const size_t count) {
    try {
        std::vector<Pgr_point_t> points(vertices, vertices + count);

        std::sort(points.begin(), points.end(),
                [](const Pgr_point_t &e1, const Pgr_point_t &e2)->bool {
                    return e2.y < e1.y;
                });
        std::stable_sort(points.begin(), points.end(),
                [](const Pgr_point_t &e1, const Pgr_point_t &e2)->bool {
                    return e2.x < e1.x;
                });
        auto position = std::unique(points.begin(), points.end(),
                [](const Pgr_point_t &e1, const Pgr_point_t &e2)->bool {
                    return e2.x == e1.x && e2.y == e1.y;
                }) - points.begin();
        size_t i = 0;
        for (const auto p : points) {
            vertices[++i] = p;
        }
        return position;
    } catch (...) {
        return 0;
    }
}


using Ring = std::vector<Point>;

static
Point
get_source(Ring ring) {
    return ring.front();
}

static
Point
get_target(Ring ring) {
    return ring.back();
}

static
std::vector<Ring>
extract_rings(std::vector<Segment> segments) {
    std::vector<Ring> rings;
    Ring ring;
    while (!segments.empty()) {
        size_t s_size(segments.size());
        if (ring.empty()) {
            ring.push_back(segments.back().source());
            ring.push_back(segments.back().target());
            segments.pop_back();
            continue;
        }

        for (auto it = segments.begin(); it != segments.end(); ++it) {
            if (get_target(ring) == (*it).source()) {
                ring.push_back((*it).target());
                segments.erase(it);
                break;
            }
        }

        if (get_source(ring) == get_target(ring)) {
            rings.push_back(ring);
            ring.clear();
        }
        pgassert(s_size > segments.size());
    }
    return rings;
}

void
do_alphaShape(
        Pgr_point_t *vertices,
        size_t count,
        Delauny_t *delaunyArr,
        size_t delaunyTotal,
        double alpha,

        Pgr_point_t **return_tuples,
        size_t *res_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    try {
        pgassert(vertices);
        pgassert(count);
        pgassert(delaunyArr);
        pgassert(delaunyTotal);

        std::vector<Point> points;

        for (std::size_t j = 0; j < count; ++j) {
            Point p(vertices[j].x, vertices[j].y);
            points.push_back(p);
        }

        std::vector<Delauny_t> delauny(delaunyArr, delaunyArr + delaunyTotal);
        for (const auto d : delauny) {
            log << d.tid << ")" << d.pid << "," << d.x << " " << d.y << "\n";
        }

        size_t i(0);
        for (const auto p : points) {
            log << i++ << ")" << p << "\n";
        }

        std::sort(points.begin(), points.end(),
                [](const Point &e1, const Point &e2)->bool {
                    return e2.y() < e1.y();
                });
        std::stable_sort(points.begin(), points.end(),
                [](const Point &e1, const Point &e2)->bool {
                    return e2.x() < e1.x();
                });
        points.erase(std::unique(points.begin(), points.end()), points.end());
        if (points.size() != count &&  points.size() < 3) {
            err << "After eliminating duplicated points,"
                " less than 3 points remain!!."
                " Alpha shape calculation needs at least 3 vertices.";
                *err_msg = pgr_msg(err.str().c_str());
                return -1;
            }
        log << "points: ";
        i = 0;
        for (const auto p : points) {
            log << i++ << ")" << p << "\n";
        }

        Alpha_shape_2 A(points.begin(), points.end(),
                static_cast<double>(10000),
                Alpha_shape_2::REGULARIZED);


        if (alpha <= 0.0) {
            alpha = *A.find_optimal_alpha(1);
        }
        A.set_alpha(alpha);

        std::vector<Segment> segments;
        for (auto it =  A.alpha_shape_edges_begin();
                it != A.alpha_shape_edges_end();
                ++it) {
            segments.push_back(A.segment(*it));
        }

        if (segments.empty()) {
            *return_tuples = nullptr;
            *res_count = 0;
            return EXIT_SUCCESS;
        }

        auto the_rings = extract_rings(segments);

        size_t result_count = 0;
        for (const auto ring : the_rings) {
            result_count += ring.size();
        }
        result_count += (the_rings.size() - 1);
        *return_tuples = pgr_alloc(result_count, (*return_tuples));
        *res_count = result_count;

        int idx = 0;
        for (const auto ring : the_rings) {
            if (ring != the_rings.front()) {
                (*return_tuples)[idx].x = DBL_MAX;
                (*return_tuples)[idx].y = DBL_MAX;
                idx++;
            }
            for (const auto point : ring) {
                (*return_tuples)[idx].x = point.x();
                (*return_tuples)[idx].y = point.y();
                idx++;
            }
        }

        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*res_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*res_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*res_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
    }
}
