/*PGR-GNU*****************************************************************
File: alpha_drivedist.cpp

Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
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
/*
 * As a special exception, you have permission to link this program
 * with the CGAL library and distribute executables, as long as you
 * follow the requirements of the GNU GPL in regard to all of the
 * software in the executable aside from CGAL.
 *
 */


/***********************************************************************
Takes a list of points and returns a list of segments
corresponding to the Alpha shape.
************************************************************************/

#include "drivers/alpha_shape/alpha_driver.h"

#include <vector>
#include <list>
#include <cmath>
#include <utility>
#include <algorithm>
#include <set>

#include "cpp_common/pgr_assert.h"

#include "CGAL/Simple_cartesian.h"
#include "CGAL/Filtered_kernel.h"
#include "CGAL/algorithm.h"

#include "CGAL/Polygon_2.h"
#include "CGAL/Delaunay_triangulation_2.h"
#include "CGAL/Triangulation_2.h"
#include "CGAL/Triangulation_hierarchy_vertex_base_2.h"
#include "CGAL/Triangulation_hierarchy_2.h"
#include "CGAL/Triangulation_face_base_2.h"
#include "CGAL/Triangulation_euclidean_traits_2.h"
#include "CGAL/Alpha_shape_2.h"
#include "CGAL/Alpha_shape_face_base_2.h"
#include "CGAL/Alpha_shape_vertex_base_2.h"


#include "cpp_common/pgr_alloc.hpp"

typedef double coord_type;

typedef CGAL::Simple_cartesian<coord_type>  SC;
typedef CGAL::Filtered_kernel<SC> K;
typedef K::Point_2  Point;
typedef K::Segment_2  Segment;
typedef K::Vector_2 Vector;
typedef CGAL::Polygon_2<K> Polygon_2;

typedef CGAL::Alpha_shape_vertex_base_2<K> Avb;
typedef CGAL::Triangulation_hierarchy_vertex_base_2<Avb> Av;

typedef CGAL::Triangulation_face_base_2<K> Tf;
typedef CGAL::Alpha_shape_face_base_2<K, Tf> Af;

typedef CGAL::Triangulation_default_data_structure_2<K, Av, Af> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Dt;
typedef CGAL::Triangulation_hierarchy_2<Dt> Ht;
typedef CGAL::Alpha_shape_2<Ht> Alpha_shape_2;

typedef Alpha_shape_2::Face_circulator  Face_circulator;
typedef Alpha_shape_2::Vertex_circulator  Vertex_circulator;

typedef Alpha_shape_2::Alpha_iterator Alpha_iterator;
typedef Alpha_shape_2::Alpha_shape_edges_iterator Alpha_shape_edges_iterator;

// ---------------------------------------------------------------------

double get_angle(Point p, Point q, Point r) {
    double m_pi(3.14159265358979323846);
    Vector v1(q, p);
    Vector v2(q, r);
    double cross = v1.x() * v2.y() - v1.y() * v2.x();
    double dot = v1.x() * v2.x() + v1.y() * v2.y();
    double angle = atan2(cross, dot);
    if (angle < 0.0) {
        angle += 2 * m_pi;
    }
    return angle;
}

size_t prev_size = 0;
void find_next_edge(Segment s, std::vector<Segment>& segments,
        std::set<int>& unusedIndexes, std::vector<Polygon_2>& rings) {
    if (unusedIndexes.empty()
            || prev_size == unusedIndexes.size()) {
        return;
    }

    prev_size = unusedIndexes.size();

    Point start = s.source();
    Point end = s.target();
    rings.back().push_back(end);

    std::vector<int> nextIndexes;
    for (unsigned int i = 0; i < segments.size(); i++) {
        if (unusedIndexes.find(i) != unusedIndexes.end()) {
            Point source = segments.at(i).source();
            if (source == end) {
                nextIndexes.push_back(i);
            }
        }
    }
    if (nextIndexes.size() == 1) {
        int i = nextIndexes.at(0);
        unusedIndexes.erase(i);
        find_next_edge(segments.at(i), segments, unusedIndexes, rings);
    } else if (nextIndexes.size() > 1) {
        std::vector< std::pair<double, int> > nextAngles;
        for (unsigned int i = 0; i < nextIndexes.size(); i++) {
            int j = nextIndexes.at(i);
            Point target = segments.at(j).target();
            double angle = get_angle(start, end, target);
            nextAngles.push_back(std::pair<double, int>(angle, j));
        }
        std::sort(nextAngles.begin(), nextAngles.end());
        int i = nextAngles.begin()->second;
        unusedIndexes.erase(i);
        find_next_edge(segments.at(i), segments, unusedIndexes, rings);
    }

    if (!unusedIndexes.empty()) {
        for (unsigned int i = 0; i < segments.size(); i++) {
            if (unusedIndexes.find(i) != unusedIndexes.end()) {
                Polygon_2 ring;
                ring.push_back(segments.at(i).source());
                rings.push_back(ring);
                unusedIndexes.erase(i);
                find_next_edge(segments.at(i), segments, unusedIndexes, rings);
            }
        }
    }
}

template <class OutputIterator>
void
alpha_edges(const Alpha_shape_2& A,
        OutputIterator out) {
    for (Alpha_shape_edges_iterator it =  A.alpha_shape_edges_begin();
            it != A.alpha_shape_edges_end();
            ++it) {
        *out++ = A.segment(*it);
    }
}


int alpha_shape(vertex_t *vertices, size_t count, double alpha,
        vertex_t **return_tuples, size_t *res_count, char **err_msg) {
    std::ostringstream err;

    try {
        std::list<Point> points;
        {
            std::vector<Point> pv;

            for (std::size_t j = 0; j < count; ++j) {
                Point p(vertices[j].x, vertices[j].y);
                pv.push_back(p);
            }

            std::sort(pv.begin(), pv.end(),
                    [](const Point &e1, const Point &e2)->bool {
                    return e2.y() < e1.y();
                    });
            std::stable_sort(pv.begin(), pv.end(),
                    [](const Point &e1, const Point &e2)->bool {
                    return e2.x() < e1.x();
                    });
            pv.erase(std::unique(pv.begin(), pv.end()), pv.end());
            if (pv.size() != count &&  pv.size() < 3) {
                err << "After eliminating duplicated points,"
                   " less than 3 points remain!!."
                   " Alpha shape calculation needs at least 3 vertices.";
                *err_msg = pgr_msg(err.str().c_str());
                return -1;
            }
            points.insert(points.begin(), pv.begin(), pv.end());
        }

        Alpha_shape_2 A(points.begin(), points.end(),
                coord_type(10000),
                Alpha_shape_2::REGULARIZED);

        std::vector<Segment> segments;
        //  std::vector<Segment> result;

        //  Alpha_shape_2::Alpha_shape_vertices_iterator vit;
        //  Alpha_shape_2::Vertex_handle vertex;
        //  Alpha_shape_2::Alpha_shape_edges_iterator eit;
        //  Alpha_shape_2::Edge edge;
        //  Alpha_shape_2::Face_iterator fit;
        //  Alpha_shape_2::Face_handle face;

        if (alpha <= 0.0) {
            alpha = *A.find_optimal_alpha(1);
        }
        A.set_alpha(alpha);

        alpha_edges(A, std::back_inserter(segments));

        //  Segment s = segments.at(0);
        //  find_next_edge(s, segments, result);
        if (segments.empty()) {
            *return_tuples = NULL;
            *res_count = 0;
        } else {
            std::set<int> unusedIndexes;
            for (unsigned int i = 0; i < segments.size(); i++) {
                unusedIndexes.insert(i);
            }

            std::vector<Polygon_2> rings;
            Polygon_2 ring;
            ring.push_back(segments.at(0).source());
            rings.push_back(ring);
            unusedIndexes.erase(0);
            find_next_edge(segments.at(0), segments, unusedIndexes, rings);

            size_t result_count = 0;
            for (unsigned int i = 0; i < rings.size(); i++) {
                Polygon_2 ring = rings.at(i);
                result_count += ring.size();
            }
            result_count += rings.size() - 1;
            *return_tuples = pgr_alloc(result_count, (*return_tuples));
            *res_count = result_count;

            int idx = 0;
            for (unsigned int i = 0; i < rings.size(); i++) {
                if (i > 0) {
                    (*return_tuples)[idx].x = DBL_MAX;
                    (*return_tuples)[idx].y = DBL_MAX;
                    idx++;
                }
                Polygon_2 ring = rings.at(i);
                for (unsigned int j = 0; j < ring.size(); j++) {
                    Point point = ring.vertex(j);
                    (*return_tuples)[idx].x = point.x();
                    (*return_tuples)[idx].y = point.y();
                    idx++;
                }
            }
        }
        *err_msg = NULL;

        return EXIT_SUCCESS;
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
    return -1;
}
