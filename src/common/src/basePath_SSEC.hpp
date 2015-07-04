/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

#ifndef SRC_COMMON_SRC_BASE_PATH_SSCE_H_
#define SRC_COMMON_SRC_BASE_PATH_SSCE_H_

#include <deque>
#include <iostream>
#include <algorithm>
// #include <fstream>
#include "postgres.h"
#include "./pgr_types.h"

class Path {
 public:
    std::deque< pgr_path_element3_t > path;
    float8 cost;

    Path(): cost(0) {}
    size_t size() const { return path.size();}

    void push_front(pgr_path_element3_t data) ;

    void push_back(pgr_path_element3_t data) ;

    pgr_path_element3_t set_data(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost);

    void push_front(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost);

    void push_back(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost);

    void clear();

    void print_path(std::ostream& log) const;
    void print_path() const;

    Path  getSubpath(unsigned int j) const;


    bool isEqual(const Path &subpath) const;
    void appendPath(const Path &o_path);
    void empty_path(unsigned int d_vertex);

    void dpPrint(
        pgr_path_element3_t **ret_path,
        int &sequence) const;


   void dpPrint(
        pgr_path_element3_t **ret_path,
        int &sequence, int routeId) const;



  friend int collapse_paths(
      pgr_path_element3_t **ret_path,
      const std::deque< Path > &paths) {
   int sequence = 0;
   for (const Path &path : paths) {
   if (path.path.size() > 0)
        path.dpPrint(ret_path, sequence);
   }
   return sequence;
  }



  
  friend Path equi_cost(const Path &p1, const Path &p2) {
    Path result(p1);
    sort(result.path.begin(), result.path.end(), 
      [](const pgr_path_element3_t &e1, const pgr_path_element3_t &e2)->bool { 
         return e1.vertex < e2.vertex; 
      });

    for (auto const &e : p2.path) {
      auto pos = find_if(result.path.begin(), result.path.end(),
                 [&e](const pgr_path_element3_t &e1)->bool { 
                   return e.vertex == e1.vertex; 
                 });
      if (pos != result.path.end()) {
        if (pos->cost > e.cost) {
           (*pos) = e;
        }  
      } else {
        result.push_back(e);
      }
    }
    return result;
  }

  friend Path equi_cost(const std::deque< Path > &paths) {
    Path result;
    for (const auto &p1 : paths) {
      result = equi_cost(result, p1);
    }
    return result;
  }
 
  friend int count_tuples(const std::deque< Path > &paths) {
    int count(0);
    for (const Path &e : paths) {
       count += e.path.size();
    }
    return count;
  }

};


#endif  // SRC_COMMON_SRC_BASE_PATH_SSCE_H_
