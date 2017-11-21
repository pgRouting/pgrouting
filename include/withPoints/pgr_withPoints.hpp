/*PGR-GNU*****************************************************************
File: withPoints.hpp

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

#ifndef INCLUDE_WITHPOINTS_PGR_WITHPOINTS_HPP_
#define INCLUDE_WITHPOINTS_PGR_WITHPOINTS_HPP_
#pragma once
#include <vector>

#include "c_types/point_on_edge_t.h"
#include "cpp_common/pgr_messages.h"
#include "cpp_common/basePath_SSEC.hpp"

namespace pgrouting {

class Pg_points_graph : public Pgr_messages {
    struct pointCompare {
        bool operator() (
                const Point_on_edge_t& lhs,
                const Point_on_edge_t& rhs) const
        {return lhs.fraction < rhs.fraction? true : lhs.pid < rhs.pid;}
    };

 public:

     Pg_points_graph() = default;
     Pg_points_graph(const Pg_points_graph &) = default;
     Pg_points_graph(
             std::vector<Point_on_edge_t> p_points,
             std::vector<pgr_edge_t>      p_edges_to_modify,
             bool p_normal,
             char p_driving_side
             );

     std::vector<Point_on_edge_t> points() const;
     std::vector<pgr_edge_t> edges_of_points() const;
     std::vector<pgr_edge_t> new_edges() const;
     inline char driving_side() const {return m_driving_side;}

     int check_points();

     int check_points(
             std::vector< Point_on_edge_t > &points,
             std::ostringstream &log);

     Path eliminate_details(
             Path path);

     void eliminate_details_dd(
             Path &path);

     void eliminate_details(
             Path &path,
             const std::vector< pgr_edge_t > &point_edges);

     void adjust_pids(
             const std::vector< Point_on_edge_t > &points,
             Path &path);

     void create_new_edges();

     bool create_new_edges(
             std::vector< Point_on_edge_t > &points,
             const std::vector< pgr_edge_t > &edges,
             char driving_side,
             std::vector< pgr_edge_t > &new_edges);

     bool create_new_edges(
             std::vector< Point_on_edge_t > &points,
             const std::vector< pgr_edge_t > &edges,
             char driving_side,
             std::vector< pgr_edge_t > &new_edges,
             std::ostringstream &log);
 private:
     void adjust_pids(
             const std::vector< Point_on_edge_t > &points,
             const int64_t &start_pid,
             const int64_t &end_pid,
             Path &path);

     void reverse_sides();

 private:
     std::vector<Point_on_edge_t> m_points;
     std::vector<Point_on_edge_t> m_o_points;
     std::vector<pgr_edge_t>      m_edges_of_points;
     std::vector<pgr_edge_t>      m_new_edges;
     bool m_normal;
     char m_driving_side;
};

}  // namespace pgrouting

#endif  // INCLUDE_WITHPOINTS_PGR_WITHPOINTS_HPP_
