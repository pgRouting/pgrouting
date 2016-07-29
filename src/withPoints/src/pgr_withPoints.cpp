/*PGR-GNU*****************************************************************
File: pgr_withPoints.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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


#if defined(__MINGW32__) ||  defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <cassert>
#include "./pgr_withPoints.hpp"
#include "./msg_logger.hpp"


extern "C" {
#include "./../../common/src/pgr_types.h"
}

/*
 * 0 = success
 * non 0 = error code
 */

int check_points(std::vector< Point_on_edge_t > &points,
        std::ostringstream &log) {
    PGR_LOG_POINTS(log, points, "original points");
    /*
     * deleting duplicate points
     */
    std::sort(points.begin(), points.end(),
            [](const Point_on_edge_t &a, const Point_on_edge_t &b)
           -> bool {
            if (a.pid != b.pid) return a.pid < b.pid;
            if (a.edge_id != b.edge_id) return a.edge_id < b.edge_id;
            if (a.fraction != b.fraction) return a.fraction < b.fraction;
            return a.side < b.side;
            });
    PGR_LOG_POINTS(log, points, "after sorting");
    auto last = std::unique(points.begin(), points.end(),
            [](const Point_on_edge_t &a, const Point_on_edge_t &b) {
            return a.pid == b.pid &&
            a.edge_id == b.edge_id &&
            a.fraction == b.fraction &&
            a.side == b.side;
            });
    points.erase(last, points.end());
    size_t total_points = points.size();

    PGR_LOG_POINTS(log, points, "after deleting repetitions");
    log << "We have " << total_points << " different points";

    last = std::unique(points.begin(), points.end(),
            [](const Point_on_edge_t &a, const Point_on_edge_t &b) {
            return a.pid == b.pid;
            });
    points.erase(last, points.end());
    PGR_LOG_POINTS(log, points, "after deleting points with same id");

    if (points.size() != total_points) {
        return 1;
    }
    return 0;
}


void
eliminate_details_dd(
        Path &path) {
    /*
     * There is no path nothing to do
     */
    if (path.empty()) return;

    Path newPath(path.start_id(), path.end_id());
    for (const auto &pathstop : path) {
        if ((pathstop.node == path.start_id())
                 || (pathstop.node == path.end_id())
                 || (pathstop.node > 0)) {
            newPath.push_back(pathstop);
        }
    }

    path = newPath;
}


void
eliminate_details(
        Path &path,
        const std::vector< pgr_edge_t > &point_edges) {
    /*
     * There is no path nothing to do
     */
    if (path.empty()) return;

    Path newPath(path.start_id(), path.end_id());
    double cost = 0.0;
    for (const auto &pathstop : path) {
        if ((pathstop.node == path.start_id())
                 || (pathstop.node == path.end_id())
                 || (pathstop.node > 0)) {
            newPath.push_back(pathstop);
            if (pathstop.node != path.end_id()) cost = 0.0;
            continue;
        }
        cost += pathstop.cost;
    }

    newPath[0].cost = newPath[1].agg_cost;
    for (unsigned int i = 1; i < newPath.size() - 2; ++i) {
        /* newPath[i] has: node, edge, cost, agg_cost
         * pgr_type_t has: id, source, target, cost, reverse_cost
         *
         * find the edge where the pathstop.edge == edge.id */

        int64_t edge_to_find = newPath[i].edge;
        auto edge_ptr = std::find_if(point_edges.begin(), point_edges.end(),
                [&edge_to_find](const pgr_edge_t &edge)
                {return edge_to_find == edge.id;});
        if (edge_ptr != point_edges.end()) {
            newPath[i].cost = edge_ptr->target == newPath[i+1].node ?
                edge_ptr->cost : edge_ptr->reverse_cost;
        }
    }
    newPath[newPath.size()-2].cost += cost;


    path = newPath;
}


static void
adjust_pids(
        const std::vector< Point_on_edge_t > &points,
        const int64_t &start_pid,
        const int64_t &end_pid,
        Path &path) {
    if (path.empty()) return;
    path.start_id(start_pid);
    path.end_id(end_pid);

    for (auto &path_stop : path) {
        for (const auto point : points) {
            if (point.vertex_id == path_stop.node) {
                path_stop.node = -point.pid;
                break;
            }
        }
    }
}

void
adjust_pids(
        const std::vector< Point_on_edge_t > &points,
        Path &path) {
    /*
     * There is no path nothing to do
     */
    if (path.empty()) return;
    /* from, to:
     *      * are constant along the path
     *           */
    int64_t start_vid = path.start_id();
    int64_t end_vid = path.end_id();

    int64_t start_pid = 0;
    int64_t end_pid = 0;

    for (auto &p : points) {
        if (p.vertex_id == start_vid) {
            start_pid = -p.pid;
        }
        if (p.vertex_id == end_vid) {
            end_pid = -p.pid;
        }
    }
    adjust_pids(points, start_pid, end_pid, path);
}


struct pointCompare {
    bool operator() (const Point_on_edge_t& lhs, const Point_on_edge_t& rhs) const
    { return lhs.fraction < rhs.fraction? true : lhs.pid < rhs.pid;}
};

bool
create_new_edges(
        std::vector< Point_on_edge_t > &points,
        const std::vector< pgr_edge_t > &edges,
        char driving_side,
        std::vector< pgr_edge_t > &new_edges) {
    std::ostringstream log;
    return create_new_edges( points, edges, driving_side, new_edges, log);
}



bool
create_new_edges(
        std::vector< Point_on_edge_t > &points,
        const std::vector< pgr_edge_t > &edges,
        char driving_side,
        std::vector< pgr_edge_t > &new_edges,
        std::ostringstream &log) {
    for (const auto &point : points) {
        log << "point: "
            << point.pid << "\t"
            << point.edge_id << "\t"
            << point.fraction << "\t"
            << point.side << "\t"
            << point.vertex_id << "\n";
    }

    int64_t vertex_id = 1;
    std::vector< Point_on_edge_t > new_points;
    for (const auto edge : edges) {
        std::set< Point_on_edge_t, pointCompare> points_on_edge;
        for (const auto point : points) {
            if (edge.id == point.edge_id) {
                points_on_edge.insert(point);
                log << "working points: "
                    << point.pid << "\t"
                    << point.edge_id << "\t"
                    << point.fraction << "\t"
                    << point.side << "\t"
                    << point.vertex_id << "\n";
            }
        }
        if (points_on_edge.empty()) {
            log << "For some reason we didn't find a point belonging to the edge, must be an error\n";
            return false;
        }
#if 0
        log << "breaking: \n"
            << edge.id << "\t"
            << edge.source << "\t"
            << edge.target << "\t"
            << edge.cost << "\t"
            << edge.reverse_cost << "\n";
#endif
        int64_t prev_target = edge.source;
        int64_t prev_rtarget = edge.source;
        double prev_fraction = 0;
        double prev_rfraction = 0;
        double agg_cost = 0;
        double agg_rcost = 0;
        double last_cost = 0;
        double last_rcost = 0;
        std::vector< Point_on_edge_t> the_points(points_on_edge.begin(), points_on_edge.end());

        for (auto &point : the_points) {
            /* the point either has
             * vertex_id = source
             * vertex_id = target
             * vertex_id = -newnumber
             */
            log << "\npid"
                << point.pid
                << "\teid" << point.edge_id
                << "/t" << point.fraction
                << "\t" << point.side << "\n";
            if (point.fraction <= 0 ||  point.fraction >= 1) {
                log << "For some reason an invalid fraction was accepted, must be an error\n";
                return false;
            }
            if (point.fraction == 0) {
                log << "vertex_id of the point is the source" << edge.source << "\n";
                point.vertex_id = edge.source;
            }
            if (point.fraction == 1) {
                log << "vertex_id of the point is the target" << edge.target << "\n";
                point.vertex_id = edge.target;
            }
            if (point.fraction > 0 &&  point.fraction < 1) {
                log << "vertex_id of the point is " << -point.pid << "\n";
                point.vertex_id = -point.pid;
                ++vertex_id;
            }
            new_points.push_back(point);

            double deltaFraction = point.fraction - prev_fraction;
            double deltarFraction = point.fraction - prev_rfraction;
            if ((edge.cost < 0 ||  edge.reverse_cost < 0)
                     || driving_side == 'b'
                     || point.side == 'b') {
                log << "Edge is one way or driving side is both or point side is both\n";
                log << "Edge is one way: " << (edge.cost < 0 ||  edge.reverse_cost < 0) << "\n";
                log << "driving side: " << driving_side << "\n";
                log << "point side: " << point.side << "\n";
                if (point.fraction > 0 &&  point.fraction < 1) {
                    if (edge.cost >= 0) {
                        last_cost = deltaFraction * edge.cost;
                        pgr_edge_t new_edge = {edge.id , prev_target, point.vertex_id, last_cost, -1};
                        new_edges.push_back(new_edge);
                        log << "new_edge: (id, source, target, cost, reverse_cost) = ("
                            << new_edge.id << "\t"
                            << new_edge.source << "\t"
                            << new_edge.target << "\t"
                            << new_edge.cost << "\t"
                            << new_edge.reverse_cost << ")\n";
                    }
                    if (edge.reverse_cost >= 0) {
                        last_rcost = deltarFraction * edge.reverse_cost;
                        pgr_edge_t new_edge = {edge.id , prev_target, point.vertex_id, -1, last_rcost};
                        new_edges.push_back(new_edge);
                        log << "new_edge: (id, source, target, cost, reverse_cost) = ("
                            << new_edge.id << "\t"
                            << new_edge.source << "\t"
                            << new_edge.target << "\t"
                            << new_edge.cost << "\t"
                            << new_edge.reverse_cost << ")\n";
                    }
                }
                prev_target = point.vertex_id;
                prev_fraction = point.fraction;
                agg_cost += last_cost;

                prev_rtarget = point.vertex_id;
                prev_rfraction = point.fraction;
                agg_rcost += last_rcost;
                continue;
            }

            assert(edge.cost > 0 &&  edge.reverse_cost > 0);
            assert(point.side != 'b');

            if (driving_side == point.side) {
                log << "two way and driving side is == than the side of the point\n";
                log << "Breaking (source, target) edge only when its not the extreme of the segment\n";
                if (point.fraction > 0 &&  point.fraction < 1) {
                    last_cost = deltaFraction * edge.cost;
                    pgr_edge_t new_edge = {edge.id , prev_target, point.vertex_id, last_cost, -1};
                    new_edges.push_back(new_edge);
                    log << "new_edge: (id, source, target, cost, reverse_cost) = ("
                        << new_edge.id << "\t"
                        << new_edge.source << "\t"
                        << new_edge.target << "\t"
                        << new_edge.cost << "\t"
                        << new_edge.reverse_cost << ")\n";
                }
                prev_target = point.vertex_id;
                prev_fraction = point.fraction;
                agg_cost += last_cost;
                continue;
            }

            log << "two way and driving side is != than the side of the point\n";
            if (point.fraction > 0 &&  point.fraction < 1) {
                last_rcost = deltarFraction * edge.reverse_cost;
                pgr_edge_t new_edge = {
                    edge.id,
                    prev_rtarget,
                    point.vertex_id,
                    -1,
                    last_rcost};
                new_edges.push_back(new_edge);
                log << "new_edge: (id, source, target, cost, reverse_cost) = ("
                    << new_edge.id << "\t"
                    << new_edge.source << "\t"
                    << new_edge.target << "\t"
                    << new_edge.cost << "\t"
                    << new_edge.reverse_cost << ")\n";
            }
            prev_rtarget = point.vertex_id;
            prev_rfraction = point.fraction;
            agg_rcost += last_rcost;
        }

        {  //  the last segments
            pgr_edge_t new_edge = {
                edge.id,
                prev_target,
                edge.target,
                (edge.cost - agg_cost),
                -1};
            new_edges.push_back(new_edge);
            log << "last edge: (id, source, target, cost, reverse_cost) = ("
                << new_edge.id << "\t"
                << new_edge.source << "\t"
                << new_edge.target << "\t"
                << new_edge.cost << "\t"
                << new_edge.reverse_cost << ")\n";

            new_edge = {edge.id , prev_rtarget, edge.target,
                -1, (edge.reverse_cost - agg_rcost)};
            new_edges.push_back(new_edge);
            log << "last edge: (id, source, target, cost, reverse_cost) = ("
                << new_edge.id << "\t"
                << new_edge.source << "\t"
                << new_edge.target << "\t"
                << new_edge.cost << "\t"
                << new_edge.reverse_cost << ")\n";
        }
    }

    points = new_points;
    for (const auto &point : points) {
        log << "point: "
            << point.pid << "\t"
            << point.edge_id << "\t"
            << point.fraction << "\t"
            << point.side << "\t"
            << point.vertex_id << "\n";
    }
    return true;
}
