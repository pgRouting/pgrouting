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
     Pg_points_graph() = delete;
     Pg_points_graph(const Pg_points_graph &) = delete;
     Pg_points_graph(
             std::vector<Point_on_edge_t> p_points,
             std::vector<pgr_edge_t>      p_edges_to_modify,
             bool p_normal,
             char p_driving_side,
             bool p_directed);

     std::vector<Point_on_edge_t> points() const;
     std::vector<pgr_edge_t> edges_of_points() const;
     std::vector<pgr_edge_t> new_edges() const;
     inline char driving_side() const {return m_driving_side;}


     Path eliminate_details(
             Path path) const;

     void eliminate_details_dd(
             Path &path) const;

     void adjust_pids(
             const std::vector< Point_on_edge_t > &points,
             Path &path);

     friend std::ostream& operator<<(
             std::ostream &os, const Pg_points_graph &g);

 private:
     void adjust_pids(
             const std::vector< Point_on_edge_t > &points,
             const int64_t &start_pid,
             const int64_t &end_pid,
             Path &path);

     void create_new_edges();
     void check_points();
     void reverse_sides();
     int64_t get_edge_id(int64_t pid) const;
     const pgr_edge_t*  get_edge_data(int64_t eid) const;

 private:
     std::vector<Point_on_edge_t> m_points;
     std::vector<Point_on_edge_t> m_o_points;
     std::vector<pgr_edge_t>      m_edges_of_points;
     std::vector<pgr_edge_t>      m_new_edges;
     bool m_normal;
     char m_driving_side;
     bool m_directed;
};

}  // namespace pgrouting

#endif  // INCLUDE_WITHPOINTS_PGR_WITHPOINTS_HPP_
