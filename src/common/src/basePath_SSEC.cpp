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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include "basePath_SSEC.hpp"
#include <deque>
#include <iostream>
#include <algorithm>
#include "postgres.h"
#include "./pgr_types.h"


    void Path::push_front(General_path_element_t data) {
        cost += data.cost;
        path.push_back(data);
    }

    void Path::push_back(General_path_element_t data) {
        cost += data.cost;
        path.push_back(data);
    }

    General_path_element_t Path::set_data(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost) {
      General_path_element_t data;
      data.seq = d_seq;
      data.from = d_from;
      data.to = d_to;
      data.vertex = d_vertex;
      data.edge = d_edge;
      data.cost = d_cost;
      data.tot_cost = d_tot_cost;
      return data;
    }

    void Path::push_front(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost) {
      path.push_front(set_data(d_seq, d_from, d_to, d_vertex, d_edge, d_cost, d_tot_cost));
      cost += path[0].cost;
    }

    void Path::push_back(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost) {
      path.push_back(set_data(d_seq, d_from, d_to, d_vertex, d_edge, d_cost, d_tot_cost));
      cost += path[path.size() - 1].cost;
    }

    void Path::clear() {
        path.clear();
        cost = 0;
    }

    void Path::print_path(std::ostream& log) const {
       log << "seq\tfrom\tto\tvertex\tedge\tcost\ttot_Cost\n";
       for (unsigned int i = 0; i < path.size(); ++i)
          log << path[i].seq << "\t"
              << path[i].from << "\t"
              << path[i].to << "\t"
              << path[i].vertex << "\t"
              << path[i].edge << "\t"
              << path[i].cost << "\t"
              << path[i].tot_cost << "\n";
    }

    // for ksp
    void Path::fix_path(int64_t p_from, int64_t p_to) {
       for (auto &row :  path) {
           row.from = p_to;
           row.to = p_from;
       }
    }


    void Path::print_path() const {
       print_path(std::cout);
    }


    Path  Path::getSubpath(unsigned int j) const {
        Path  result;
        if (j == 0)  return result;
        for (auto i = path.begin(); i != path.begin() + j; ++i) result.push_back((*i));
        return result;
    }


    bool Path::isEqual(const Path &subpath) const {
        if (subpath.path.empty()) return true;
        if (subpath.path.size() >= path.size()) return false;
        std::deque<General_path_element_t>::const_iterator i, j;
        for (i = path.begin(),  j = subpath.path.begin();
             j != subpath.path.end();
             ++i, ++j)
             if ((*i).vertex !=  (*j).vertex) return false;
        return true;
    }

    void Path::appendPath(const Path &o_path) {
        path.insert(path.end(), o_path.path.begin(), o_path.path.end());
        cost +=  o_path.cost;
    }

    void Path::empty_path(unsigned int d_vertex) {
        path.push_back(set_data(1, d_vertex, d_vertex, d_vertex, -1, 0, 0));
    }


void Path::generate_postgres_data(
  General_path_element_t **postgres_data,
  size_t &sequence) const{
    for (unsigned int i = 0; i < path.size(); i++) {
      (*postgres_data)[sequence] = path[i];
      sequence++;
    }
}

/* used by driving distance */
   void Path::ddPrint(
        General_path_element_t **ret_path,
        int &sequence, int routeId) const {

    for (unsigned int i = 0; i < path.size(); i++) {
      (*ret_path)[sequence] = path[i];
      (*ret_path)[sequence].from = (uint64_t)routeId;
      sequence++;
    }
  }

/* used by ksp */
   void Path::dpPrint(
        General_path_element_t **ret_path,
        int &sequence, int routeId) const {

    for (unsigned int i = 0; i < path.size(); i++) {
      (*ret_path)[sequence] = path[i];
      (*ret_path)[sequence].seq = i + 1;
      (*ret_path)[sequence].from = (uint64_t)routeId;
      (*ret_path)[sequence].tot_cost = (i == 0)? 0: (*ret_path)[sequence-1].tot_cost +  path[i-1].cost;
      sequence++;
    }
  }

#if 0
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

#endif // 0
