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


#include "withPoints/pgr_withPoints.hpp"

#include <sstream>
#include <deque>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

#include "cpp_common/pgr_assert.h"

namespace pgrouting {


std::ostream& operator<<(
        std::ostream &os, const Pg_points_graph &g) {
    for (const auto p : g.m_points) {
        os << p.pid << "\t"
            << p.edge_id << "\t"
            << p.fraction << "\t"
            << p.side << "\n";
    }
    return os;
}



std::vector<Point_on_edge_t>
Pg_points_graph::points() const {
    return m_points;
}

std::vector<pgr_edge_t>
Pg_points_graph::edges_of_points() const {
    return m_edges_of_points;
}



Pg_points_graph::Pg_points_graph(
        std::vector<Point_on_edge_t> p_points,
        std::vector<pgr_edge_t>      p_edges_of_points,
        bool p_normal,
        char p_driving_side,
        bool p_directed) :
    m_points(p_points),
    m_o_points(p_points),
    m_edges_of_points(p_edges_of_points),
    m_normal(p_normal),
    m_driving_side(p_driving_side),
    m_directed(p_directed) {
    if (!p_normal) {
        reverse_sides();
    }
    if (!m_directed) {
        m_driving_side = 'b';
    }
    check_points();
    create_new_edges();
    log << "constructor";
}

void
Pg_points_graph::reverse_sides() {
    for (auto &point : m_points) {
        if (point.side == 'r') {
            point.side = 'l';
        } else if (point.side == 'l') {
            point.side = 'r';
        }
        point.fraction = 1 - point.fraction;
    }
    if (m_driving_side == 'r') {
        m_driving_side = 'l';
    } else if (m_driving_side == 'l') {
        m_driving_side = 'r';
    }
}


void
Pg_points_graph::check_points() {
    log << "original points" << *this;
    /*
     * deleting duplicate points
     */
    std::sort(m_points.begin(), m_points.end(),
            [](const Point_on_edge_t &a, const Point_on_edge_t &b)
            -> bool {
            if (a.pid != b.pid) return a.pid < b.pid;
            if (a.edge_id != b.edge_id) return a.edge_id < b.edge_id;
            if (a.fraction != b.fraction) return a.fraction < b.fraction;
            return a.side < b.side;
            });
    log << "after sorting" << *this;
    auto last = std::unique(m_points.begin(), m_points.end(),
            [](const Point_on_edge_t &a, const Point_on_edge_t &b) {
            return a.pid == b.pid &&
            a.edge_id == b.edge_id &&
            a.fraction == b.fraction &&
            a.side == b.side;
            });
    m_points.erase(last, m_points.end());
    size_t total_points = m_points.size();

    log << "after deleting repetitions" << *this;
    log << "We have " << total_points << " different points";

    last = std::unique(m_points.begin(), m_points.end(),
            [](const Point_on_edge_t &a, const Point_on_edge_t &b) {
            return a.pid == b.pid;
            });
    m_points.erase(last, m_points.end());
    log << "after deleting points with same id" << *this;

    if (m_points.size() != total_points) {
        error << "Unexpected point(s) with same pid"
            << " but different edge/fraction/side combination found.";
    }
}


int64_t
Pg_points_graph::get_edge_id(int64_t pid) const {
    auto point_ptr = std::find_if(
            m_points.begin(), m_points.end(),
            [&pid](const Point_on_edge_t &point)
            {return pid == -point.pid;});
    return point_ptr != m_points.end() ?
        point_ptr->edge_id :
        -1;
}

const pgr_edge_t*
Pg_points_graph::get_edge_data(int64_t eid) const {
    auto e_itr = std::find_if(
            m_edges_of_points.begin(), m_edges_of_points.end(),
            [&eid](const pgr_edge_t &edge)
            {return eid == edge.id;});
    return e_itr ==  m_edges_of_points.end()?
       nullptr : &(*e_itr);
}


void
Pg_points_graph::eliminate_details_dd(
        Path &path) const {
    /*
     * There is no path nothing to do
     */
    if (path.empty()) return;

    Path newPath(path.start_id(), path.end_id());
    auto edge_id = path.start_id() < 0?
        get_edge_id(path.start_id()) :
        -1;

    for (auto pathstop : path) {
        /*
         * skip points (no details)
         *  except if ithe point its the starting point
         */
        if (!((pathstop.node == path.start_id())
                || (pathstop.node > 0))) {
            continue;
        }

        /*
         * Change costs only when the node is not:
         * - start_id
         * - directly connected to start_id
         */
        if (pathstop.node != path.start_id()) {
            auto edge_ptr = get_edge_data(pathstop.edge);
            /*
             * edge found
             * and its not the edge directly connected to start_id()
             */
            if (edge_ptr
                    && edge_ptr->id != edge_id) {
                pathstop.cost = pathstop.node == edge_ptr->source?
                    edge_ptr->cost :
                        edge_ptr->reverse_cost;
                }
        }

        /*
         * add to the new path
         */
        newPath.push_back(pathstop);
    }

    path = newPath;
}



Path
Pg_points_graph::eliminate_details(
        Path path) const {
    /*
     * There is no path nothing to do
     */
    if (path.empty()) return path;

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
        auto edge_ptr = std::find_if(
                m_edges_of_points.begin(), m_edges_of_points.end(),
                [&edge_to_find](const pgr_edge_t &edge)
                {return edge_to_find == edge.id;});
        if (edge_ptr != m_edges_of_points.end()) {
            newPath[i].cost = edge_ptr->target == newPath[i+1].node ?
                edge_ptr->cost : edge_ptr->reverse_cost;
        }
    }
    newPath[newPath.size()-2].cost += cost;

    return newPath;
}


void
Pg_points_graph::adjust_pids(
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
Pg_points_graph::adjust_pids(
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



std::vector<pgr_edge_t>
Pg_points_graph::new_edges() const {
    return m_new_edges;
}

void
Pg_points_graph::create_new_edges() {
    for (const auto &point : m_points) {
        log << "point: "
            << point.pid << "\t"
            << point.edge_id << "\t"
            << point.fraction << "\t"
            << point.side << "\t"
            << point.vertex_id << "\n";
    }

    int64_t vertex_id = 1;
    std::vector< Point_on_edge_t > new_points;
    for (const auto edge : m_edges_of_points) {
        std::set< Point_on_edge_t, pointCompare> points_on_edge;
        for (const auto point : m_points) {
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
            error << "For some reason didn't find a point belonging to the edge"
                << ", must be an error\n";
            return;
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
        std::vector<Point_on_edge_t> the_points(
                points_on_edge.begin(), points_on_edge.end());

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
            if (point.fraction < 0 ||  point.fraction > 1) {
                error << "For some reason an invalid fraction was accepted,"
                    << " must be an error\n";
                return;
            }
            if (point.fraction == 0) {
                log << "Point's vertex_id = source" << edge.source << "\n";
                point.vertex_id = edge.source;
            }
            if (point.fraction == 1) {
                log << "point's vertex_id = target" << edge.target << "\n";
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
                    || m_driving_side == 'b'
                    || point.side == 'b') {
                log << "Edge is one way "
                    << " or driving side is both or point side is both\n";
                log << "Edge is one way: "
                    << (edge.cost < 0 || edge.reverse_cost < 0)
                    << "\n";
                log << "driving side: " << m_driving_side << "\n";
                log << "point side: " << point.side << "\n";
                if (point.fraction > 0 &&  point.fraction < 1) {
                    if (edge.cost >= 0) {
                        last_cost = deltaFraction * edge.cost;
                        pgr_edge_t new_edge = {
                            edge.id,
                            prev_target,
                            point.vertex_id,
                            last_cost,
                            -1};
                        m_new_edges.push_back(new_edge);
                        log << "new_edge("
                            << "id, source, target, cost, reverse_cost) = ("
                            << new_edge.id << "\t"
                            << new_edge.source << "\t"
                            << new_edge.target << "\t"
                            << new_edge.cost << "\t"
                            << new_edge.reverse_cost << ")\n";
                    }
                    if (edge.reverse_cost >= 0) {
                        last_rcost = deltarFraction * edge.reverse_cost;
                        pgr_edge_t new_edge = {
                            edge.id,
                            prev_target,
                            point.vertex_id,
                            -1,
                            last_rcost};
                        m_new_edges.push_back(new_edge);
                        log << "new_edge("
                            << "id, source, target, cost, reverse_cost) = ("
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

            pgassert(edge.cost > 0 &&  edge.reverse_cost > 0);
            pgassert(point.side != 'b');

            if (m_driving_side == point.side) {
                log << "two way and driving side == the side of the point\n";
                log << "Breaking (source, target) when its not the extreme\n";
                if (point.fraction > 0 &&  point.fraction < 1) {
                    last_cost = deltaFraction * edge.cost;
                    pgr_edge_t new_edge = {
                        edge.id, prev_target, point.vertex_id, last_cost, -1};
                    m_new_edges.push_back(new_edge);
                    log << "new_edge("
                        << "id, source, target, cost, reverse_cost) = ("
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

            log << "\ntwo way and driving side != the side of the point";
            if (point.fraction > 0 &&  point.fraction < 1) {
                last_rcost = deltarFraction * edge.reverse_cost;
                pgr_edge_t new_edge = {
                    edge.id,
                    prev_rtarget,
                    point.vertex_id,
                    -1,
                    last_rcost};
                m_new_edges.push_back(new_edge);
                log << "\nnew_edge(id, source, target, cost, reverse_cost) = ("
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
            m_new_edges.push_back(new_edge);
            log << "last edge: (id, source, target, cost, reverse_cost) = ("
                << new_edge.id << "\t"
                << new_edge.source << "\t"
                << new_edge.target << "\t"
                << new_edge.cost << "\t"
                << new_edge.reverse_cost << ")\n";

            new_edge = {edge.id , prev_rtarget, edge.target,
                -1, (edge.reverse_cost - agg_rcost)};
            m_new_edges.push_back(new_edge);
            log << "last edge: (id, source, target, cost, reverse_cost) = ("
                << new_edge.id << "\t"
                << new_edge.source << "\t"
                << new_edge.target << "\t"
                << new_edge.cost << "\t"
                << new_edge.reverse_cost << ")\n";
        }
    }

    m_points = new_points;
    for (const auto &point : m_points) {
        log << "point: "
            << point.pid << "\t"
            << point.edge_id << "\t"
            << point.fraction << "\t"
            << point.side << "\t"
            << point.vertex_id << "\n";
    }
}

}  // namespace pgrouting
